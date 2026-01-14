#ifndef PROGRAM_GRAPH_H
#define PROGRAM_GRAPH_H

#include<memory>

#include "synthesis/header/VarMgr.h"
#include "synthesis/header/automata/SymbolicStateDfa.h"

#include "golog_program.hpp"

/**
 * \brief A program graph with explicit states and symbolic transitions
 */
class ExplicitStateProgramGraph {

    private:
        std::shared_ptr<Syft::VarMgr> var_mgr_;

        std::size_t initial_state_;
        std::size_t state_count_;
        std::vector<CUDD::BDD> final_states_;
        std::vector<CUDD::BDD> continuation_function_;
        std::vector<CUDD::ADD> transition_function_;

        ExplicitStateProgramGraph(std::shared_ptr<Syft::VarMgr> var_mgr);

        /**
         * \brief create state variables and ID for symbolic DFA of program graph
         */
        std::pair<std::size_t, std::size_t> create_state_variables() const;


        /**
         * \brief transform an explicit state into binary representation
         * 
         * \param state the state to be transformed into binary
         * \param bit_count the number of bits in its binary representation
         */
        std::vector<int> state_to_binary(
            std::size_t state,
            std::size_t bit_count
        ) const;

        /**
         * \brief transform an explicit state into the BDD of its binary representation
         * 
         * \param automaton_id the ID of the automaton of the program graph
         * \param state the state whose BDD is being computed
         */
        CUDD::BDD state_to_bdd(
            std::size_t automaton_id,
            std::size_t state
        ) const;

        /**
         * \brief construct a BDD that decides when the program graph is in a final configuration
         * 
         * \param automaton_id the ID of the automaton of the program graph
         */
        CUDD::BDD final_states_to_bdd(
            std::size_t automaton_id
        ) const;

        /** 
         * \brief construct the transition function of the symbolic representation of the program graph
         * 
         * \param automaton_id the ID of the automaton of the program graph
         */
        std::vector<CUDD::BDD> symbolic_transition_function(
            std::size_t automaton_id
        ) const; 


    public:
        /**
         * \brief Construct an explicit-state program graph from a Golog program
         * 
         * \param var_mgr The variable manager for managing transition variables
         * \param golog_program The golog program 
         */
        static ExplicitStateProgramGraph from_golog_program(
            std::shared_ptr<Syft::VarMgr> var_mgr,
            const GologProgramNode& golog_program);

        /**
         * \brief Returns the variable manager
         */
        std::shared_ptr<Syft::VarMgr> var_mgr() const;

        /**
         * \brief Returns the initial state of the program graph
         */
        std::size_t initial_state() const;

        /**
         * \brief Returns the number of states in the program graph
         */
        std::size_t state_count() const;

        /**
         * \brief Returns the final states condition for each state in the program graph
         */
        std::vector<CUDD::BDD> final_states() const;

        /**
         * \brief Returns the transition function of the program graph as a vector of ADDs
         *
         * The ADD in index \a i represents the transition function for state \a i
         */
        std::vector<CUDD::ADD> transition_function() const;

        /**
         * \brief Returns the continutation function for each state in the program graph
         */
        std::vector<CUDD::BDD> continuation_function() const; 

        /**
         * \brief Saves transition function and final states of the program graph in a .dot file
         * 
         * \param filename The name of the file to save the transition function to
         */
        void dump_dot(const std::string& filename) const; 

        /**
         * \brief Constructs symbolic-state DFA corresponding to program graph
         */
        Syft::SymbolicStateDfa to_symbolic() const;
}; 

#endif