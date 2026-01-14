#include"program_graph.h"

#include<algorithm>
#include<fstream>

ExplicitStateProgramGraph::ExplicitStateProgramGraph(std::shared_ptr<Syft::VarMgr> var_mgr) :
    var_mgr_(std::move(var_mgr)) {}

std::shared_ptr<Syft::VarMgr> ExplicitStateProgramGraph::var_mgr() const {
    return var_mgr_;
} 

std::size_t ExplicitStateProgramGraph::initial_state() const {
    return initial_state_;
}

std::size_t ExplicitStateProgramGraph::state_count() const {
    return state_count_;
}

std::vector<CUDD::BDD> ExplicitStateProgramGraph::final_states() const {
    return final_states_;
}

std::vector<CUDD::BDD> ExplicitStateProgramGraph::continuation_function() const {
    return continuation_function_;
}

std::vector<CUDD::ADD> ExplicitStateProgramGraph::transition_function() const {
    return transition_function_;
}

void ExplicitStateProgramGraph::dump_dot(const std::string& filename) const {
    std::vector<std::string> function_labels(2*state_count_); // state_count_ entries for transition function + state_count_ entries for final states functions

    // transition functions
    for (std::size_t i = 0; i < state_count_; ++i)
        function_labels[i] = "S" + std::to_string(i);

    // final states functions
    for (std::size_t i = 0; i < state_count_; ++i)
        function_labels[i] = "F" + std::to_string(i);

    // add_vector contains both transitions and final states
    std::vector<CUDD::ADD> add_vector;
    add_vector.reserve(transition_function_.size() + final_states_.size());
    add_vector.insert(add_vector.end(), transition_function_.begin(), transition_function_.end());
    add_vector.insert(add_vector.end(), final_states_.begin(), final_states_.end());

    var_mgr_->dump_dot(add_vector, function_labels, filename);    
}

std::pair<std::size_t, std::size_t> ExplicitStateProgramGraph::create_state_variables() const {
    std::size_t max_state = state_count_ - 1;
    std::size_t bit_count = 0;

    // number of iterations is log in max state
    while (max_state > 0) {
        ++bit_count;
        max_state >>= 1;
    }

    std::size_t automaton_id = var_mgr_->create_state_variables(bit_count);

    return std::make_pair(bit_count, automaton_id);
}

std::vector<int> ExplicitStateProgramGraph::state_to_binary(
    std::size_t state,
    std::size_t bit_count
) const {
    std::vector<int> binary_representation;

    while (state != 0) {
        // Add the least significant bit of the state to the binary representation
        binary_representation.push_back(state & 1);

        // Shift right
        state >>= 1;
    }

    // Fill rest of the vector with zeroes up to bit_count
    binary_representation.resize(bit_count);

    // Note that the binary representation goes from least to most significant bit

    return binary_representation;
}

CUDD::BDD ExplicitStateProgramGraph::state_to_bdd(
    std::size_t automaton_id,
    std::size_t state 
) const {
    std::size_t bit_count = var_mgr_->state_variable_count(automaton_id);
    std::vector<int> binary_representation = state_to_binary(state, bit_count);

    return var_mgr_->state_vector_to_bdd(automaton_id, binary_representation);
}

CUDD::BDD ExplicitStateProgramGraph::final_states_to_bdd(
    std::size_t automaton_id
) const {
    CUDD::BDD bdd = var_mgr_->cudd_mgr()->bddZero();

    for (std::size_t i = 0; i < final_states_.size(); ++i) {
        bdd |= (final_states_[i] & state_to_bdd(automaton_id, i));
    }

    return bdd;
}

std::vector<CUDD::BDD> ExplicitStateProgramGraph::symbolic_transition_function(
    std::size_t automaton_id
) const {
    std::size_t bit_count = var_mgr_->state_variable_count(automaton_id);
    std::vector<CUDD::BDD> symbolic_transition_function(
        bit_count, var_mgr_->cudd_mgr()->bddZero());
    
    for (std::size_t j = 0; j < transition_function_.size(); ++j) {

        CUDD::BDD state_bdd = state_to_bdd(automaton_id, j);

        for (std::size_t i = 0; i < bit_count; ++i) {
            // BddIthBit counts from the least-significant bit
            CUDD::BDD jth_component = state_bdd & transition_function_[j].BddIthBit(i);

            symbolic_transition_function[i] |= jth_component;
        }
    }

    return symbolic_transition_function;
}

Syft::SymbolicStateDfa ExplicitStateProgramGraph::to_symbolic() const {
    std::shared_ptr<Syft::VarMgr> var_mgr = this->var_mgr_;

    // create state vars and automaton id in variable manager 
    auto count_and_id = create_state_variables();
    std::size_t bit_count = count_and_id.first;
    std::size_t automaton_id = count_and_id.second;

    // generate binary vector corresponding to initial state
    std::vector<int> initial_state = state_to_binary(
        this->initial_state_,
        bit_count);

    // create function final_states_to_bdd
    CUDD::BDD final_states = final_states_to_bdd(
        automaton_id
    );

    // construct symbolic transition function for explicit-state program graph
    std::vector<CUDD::BDD> transition_function = symbolic_transition_function(
        automaton_id
    );
    
    // construct symbolic DFA
    Syft::SymbolicStateDfa symbolic_dfa(
        var_mgr,
        automaton_id,
        initial_state,
        transition_function,
        final_states
    );

    return symbolic_dfa;
}