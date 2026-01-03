#include"compositional_golog_synthesizer.h"

Syft::SynthesisResult CompositionalGologSynthesizer::run() const {

    // transform domain and Golog program into DFA
    auto domain_dfa = domain_->to_symbolic();

    auto golog_program_graph = ExplicitStateProgramGraph::from_golog_program(var_mgr_, *golog_program_);
    auto golog_dfa = golog_program_graph.to_symbolic();
    
    // construct game arena via composition
    auto game_arena = compose(domain_dfa, golog_dfa);

    // solve game and return result
    auto synthesizer = Syft::Reachability(
        game_arena,
        Syft::Player::Agent, // starting player
        Syft::Player::Agent, // protagonist player
        game_arena.final_states(),
        var_mgr_->cudd_mgr()->bddOne() // state space = true
    );
    auto result = synthesizer.run();

    return result;
} 