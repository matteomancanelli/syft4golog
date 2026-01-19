#include"compositional_golog_synthesizer.h"

Syft::SynthesisResult CompositionalGologSynthesizer::run() const {

    // transform domain and Golog program into DFA
    std::cout << "[syft4golog] Transforming domain to symbolic..." << std::flush;
    auto domain_dfa = domain_->to_symbolic();
    std::cout << "DONE" << std::endl;

    std::cout << "[syft4golog] Transforming Golog to symbolic program graph..." << std::flush;
    auto golog_program_graph = ExplicitStateProgramGraph::from_golog_program(golog_program_, var_mgr_, domain_->get_action_name_to_bdd(), domain_->get_action_name_to_pre_bdd());
    auto golog_dfa = golog_program_graph.to_symbolic();
    // debug
    // golog_program_graph.dump_dot("pg.dot");
    // golog_dfa.dump_dot("pg_sdfa.dot");
    std::cout << "DONE" << std::endl;
    
    // construct game arena via composition
    std::cout << "[syft4golog] Constructing game arena..." << std::flush;
    auto game_arena = compose(domain_dfa, golog_dfa);
    std::cout << "DONE" << std::endl;

    // debug
    // var_mgr_->print_mgr();

    // solve game and return result
    std::cout << "[syft4golog] Reducing synthesis to reachability..." << std::flush;
    auto synthesizer = Syft::Reachability(
        game_arena,
        Syft::Player::Agent, // starting player
        Syft::Player::Agent, // protagonist player
        game_arena.final_states(),
        var_mgr_->cudd_mgr()->bddOne() // state space = true
    );
    auto result = synthesizer.run();
    std::cout << "DONE" << std::endl;

    return result;
} 