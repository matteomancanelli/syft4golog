#include"ldlf_compositional_golog_synthesizer.h"

Syft::SynthesisResult LDLfCompositionalGologSynthesizer::run() const {

    // transform domain into DFA
    auto domain_dfa = domain_->to_symbolic();

    // transform Golog into LDLf and get DFA
    auto ldlf_formula = golog_to_ldlf();

    // TODO. Will the transformation in line below work or do we need to add dfa_of_ldlf_formula to LydiaSyft?
    auto explicit_dfa = Syft::ExplicitStateDfa::dfa_of_ldlf_formula(*ldlf_formula);
    auto explicit_dfa_add = Syft::ExplicitStateDfaAdd::from_dfa_mona(var_mgr_, explicit_dfa);
    auto golog_dfa = Syft::SymbolicStateDfa::from_explicit(explicit_dfa_add);

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