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
    std::vector<std::string> function_labels(3*state_count_); // state_count_ entries for transition function + state_count_ entries for final states functions

    // transition functions
    for (std::size_t i = 0; i < state_count_; ++i)
        function_labels[i] = "T" + std::to_string(i);

    // final states functions
    for (std::size_t i = 0; i < state_count_; ++i)
        function_labels[state_count_+i] = "F" + std::to_string(i);

    // continuation functions
    for (std::size_t i = 0; i < state_count_; ++i)
        function_labels[2*state_count_+i] = "C" + std::to_string(i);

    // add_vector contains transitions, final states, and continuation functions
    std::vector<CUDD::ADD> add_vector;
    add_vector.reserve(transition_function_.size() + final_states_.size() + continuation_function_.size());
    add_vector.insert(add_vector.end(), transition_function_.begin(), transition_function_.end());

    // transform BDDs in final states and continuation function to BDDs for dumping
    for (const CUDD::BDD& bdd : final_states_)
        add_vector.push_back(bdd.Add());

    for (const CUDD::BDD& bdd : continuation_function_)
        add_vector.push_back(bdd.Add());

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

ExplicitStateProgramGraph ExplicitStateProgramGraph::from_golog_program(
    const golog_ptr& golog_program,
    const std::shared_ptr<Syft::VarMgr>& var_mgr,
    const std::unordered_map<std::string, CUDD::BDD>& action_name_to_bdd,
    const std::unordered_map<std::string, CUDD::BDD>& action_name_to_pre_bdd) {
        
        // compute function T, F and C for the input golog program
        TFCResult tfc = get_tfc(golog_program, var_mgr, action_name_to_bdd, action_name_to_pre_bdd);

        // debug
        tfc.print();

        const auto& t = tfc.transitions_;
        const auto& f = tfc.final_functions_;
        const auto& c = tfc.continuation_functions_;

        // number of states is number of subprograms + 1
        // the latter state is the sink for determinization
        const auto& state_count = f.size() + 1; 
        const auto sink_state = state_count - 1;
        CUDD::ADD add_zero = var_mgr->cudd_mgr()->addZero();
        CUDD::BDD bdd_zero = var_mgr->cudd_mgr()->bddZero();

        // initialize vectors of BDDs/ADDs for program graph
        // each BDD/ADD is default initialized to 0
        std::vector<CUDD::BDD> final_states(state_count, bdd_zero);
        std::vector<CUDD::BDD> continuation_function(state_count, bdd_zero);
        std::vector<CUDD::ADD> transition_function(state_count, add_zero);

        // map golog programs to indexes of ADDs
        golog_map program_id;

        std::cout << std::endl;
        std::cout << "State count: " << state_count << std::endl;
        for (const auto& [program, _] : f) {
            // assign state id to program
            program_id[program] = program_id.size();
            std::cout << "Subprogram: " << to_string(program) << ". Assigned ID: " << program_id[program]  << std::endl;
            // set final states function for program
            final_states.at(program_id.at(program)) = f.at(program);
            std::cout << "Final states function for ID " << program_id[program] << ": "  << final_states.at(program_id.at(program)) << std::endl;;
            // set unique continuation function for program
            bdd_set cfs = c.at(program);
            CUDD::BDD cf = var_mgr->cudd_mgr()->bddZero();
            for (const auto& b : cfs) cf += b;
            continuation_function.at(program_id.at(program)) = std::move(cf);
            std::cout << "Continuation function for ID " << program_id[program] << ": "  << continuation_function.at(program_id.at(program)) << std::endl;
        }
        std::cout << "Subprogram: und. Assigned ID: " << sink_state << std::endl;
        std::cout << "Final states function for ID " << sink_state << ": "  << final_states[sink_state] << std::endl;;
        std::cout << "Continuation function for ID " << sink_state << ": "  << continuation_function[sink_state] << std::endl;

        // transition function

        // used for determiniziation
        std::unordered_map<golog_ptr, CUDD::BDD, GologProgramHash, GologProgramEquals> dmap;
        for (const auto& [program, _]: program_id) {dmap[program] = var_mgr->cudd_mgr()->bddZero();}

        for (const auto& p : t) {
            // program_action_ptr p.first -> in turn, (program, action_name)
            // transition set p.second -> transition set, each obj being (guard, successor program)
            for (const auto& ts : p.second) {
                CUDD::ADD tf = var_mgr->cudd_mgr()->constant(program_id[ts->successor_program_]) 
                    * (ts->guard_.Add() * (action_name_to_bdd.at(p.first->action_)).Add() * continuation_function.at(program_id[ts->successor_program_]).Add());
                transition_function[program_id.at(p.first->program_)] += tf;
                dmap[p.first->program_] += (ts->guard_ * (action_name_to_bdd.at(p.first->action_)));
            }
        }

        // determinize undefined transitions by redirecting them to sink state
        for (const auto& [program, _ ]: program_id) {
            transition_function[program_id.at(program)] += 
                (var_mgr->cudd_mgr()->constant(sink_state) * (!dmap[program]).Add());
        }

        // sink state is a rejecting sink
        transition_function[sink_state] =
            var_mgr->cudd_mgr()->constant(sink_state);

        std::size_t initial_state = program_id[golog_program];
        std::cout << "Initial state: " << initial_state << std::endl; 

        ExplicitStateProgramGraph pg(var_mgr);
        pg.initial_state_ = std::move(initial_state);
        pg.state_count_ = std::move(state_count);
        pg.transition_function_ = std::move(transition_function);
        pg.final_states_ = std::move(final_states);
        pg.continuation_function_ = std::move(continuation_function);

        return pg;
    }