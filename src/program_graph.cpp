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

std::vector<std::size_t> ExplicitStateProgramGraph::final_states() const {
    return final_states_;
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