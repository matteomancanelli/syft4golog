#include"ldlf_golog_synthesizer.h"

std::shared_ptr<Syft::SynthesisResult> LDLfGologSynthesizer::run() {

    Stopwatch domain2dfa;
    domain2dfa.start();

    // transform domain and Golog program into DFA
    std::cout << "[syft4golog] Transforming domain to symbolic..." << std::flush;

    domain_sdfa_ = std::make_shared<Syft::SymbolicStateDfa>(std::move(domain_->to_symbolic()));

    double t_domain2dfa = domain2dfa.stop().count() / 1000.0;
    running_times_.push_back(t_domain2dfa);

    std::cout << "DONE [" << t_domain2dfa << " s]" << std::endl;

    // transform Golog into LDLf and get DFA
    Stopwatch golog2dfa;
    golog2dfa.start();

    std::cout << "[syft4golog] Transforming Golog to LDLf and DFA..." << std::flush;

    LydiaAstManager ast_mgr;
    ldlf_ptr ldlf_formula = to_ldlf(ast_mgr, golog_program_, domain_->get_action_name_to_symbols());

    auto pdfa_mona = Syft::ExplicitStateDfa::dfa_of_ldlf_formula(*ldlf_formula);
    pdfa_ = std::make_shared<Syft::ExplicitStateDfaAdd>(std::move(Syft::ExplicitStateDfaAdd::from_dfa_mona(var_mgr_, pdfa_mona)));
    auto golog_sdfa = Syft::SymbolicStateDfa::from_explicit(*pdfa_);

    running_data_.push_back(pdfa_->state_count());

    double t_golog2dfa = golog2dfa.stop().count() / 1000.0;
    running_times_.push_back(t_golog2dfa);

    std::cout << "DONE [" << t_golog2dfa << " s]" << std::endl;

    // construct game arena via composition
    Stopwatch game_construction;
    game_construction.start();

    std::cout << "[syft4golog] Constructing game arena..." << std::flush;

    game_arena_ = std::make_shared<Syft::SymbolicStateDfa>(std::move(compose(*domain_sdfa_, golog_sdfa)));

    double t_game_construction = game_construction.stop().count() / 1000.0;
    running_times_.push_back(t_game_construction);

    std::cout << "DONE [" << t_game_construction << " s]" << std::endl;

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
    result_ = std::make_shared<Syft::SynthesisResult>(synthesizer.run());

    double t_game_resolution = game_resolution.stop().count() / 1000.0;
    running_times_.push_back(t_game_resolution);

    std::cout << "DONE [" << t_game_resolution << " s]" << std::endl;

    return result_;
}