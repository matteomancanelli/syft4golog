#include"compositional_golog_synthesizer.h"

std::shared_ptr<Syft::SynthesisResult> CompositionalGologSynthesizer::run() {

    Stopwatch domain2dfa;
    domain2dfa.start();

    // transform domain and Golog program into DFA
    std::cout << "[syft4golog] Transforming domain to symbolic..." << std::flush;

    domain_sdfa_ = std::make_shared<Syft::SymbolicStateDfa>(std::move(domain_->to_symbolic()));

    double t_domain2dfa = domain2dfa.stop().count() / 1000.0;
    running_times_.push_back(t_domain2dfa);

    std::cout << "DONE [" << t_domain2dfa << " s]" << std::endl;

    Stopwatch golog2pg;
    golog2pg.start();

    std::cout << "[syft4golog] Transforming Golog to symbolic program graph..." << std::flush;
    pg_ = std::make_shared<ExplicitStateProgramGraph>(ExplicitStateProgramGraph::from_golog_program(golog_program_, var_mgr_, domain_->get_action_name_to_bdd(), domain_->get_action_name_to_pre_bdd()));

    running_data_.push_back(pg_->state_count());

    auto golog_dfa = pg_->to_symbolic();
    // debug
    // golog_program_graph.dump_dot("pg.dot");
    // golog_dfa.dump_dot("pg_sdfa.dot");
    double t_golog2pg = golog2pg.stop().count() / 1000.0;
    running_times_.push_back(t_golog2pg);

    std::cout << "DONE [" << t_golog2pg << " s]" << std::endl;
    
    // construct game arena via composition
    Stopwatch game_construction;
    game_construction.start();

    std::cout << "[syft4golog] Constructing game arena..." << std::flush;
    game_arena_ = std::make_shared<Syft::SymbolicStateDfa>(std::move(compose(*domain_sdfa_, golog_dfa)));

    double t_game_construction = game_construction.stop().count() / 1000.0;
    running_times_.push_back(t_game_construction);

    std::cout << "DONE [" << t_game_construction << " s]" << std::endl;

    // debug
    // var_mgr_->print_mgr();

    // solve game and return result
    Stopwatch game_resolution;
    game_resolution.start();

    std::cout << "[syft4golog] Reducing synthesis to reachability..." << std::flush;
    auto synthesizer = Syft::Reachability(
        *game_arena_,
        Syft::Player::Agent, // starting player
        Syft::Player::Agent, // protagonist player
        game_arena_->final_states(),
        var_mgr_->cudd_mgr()->bddOne() // state space = true
    );
    result_ = std::make_shared<Syft::SynthesisResult>(std::move(synthesizer.run()));

    double t_game_resolution = game_resolution.stop().count() / 1000.0;
    running_times_.push_back(t_game_resolution);

    std::cout << "DONE [" << t_game_resolution << " s]" << std::endl;

    return result_;
}