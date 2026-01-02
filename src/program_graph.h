#ifndef PROGRAM_GRAPH_H
#define PROGRAM_GRAPH_H

#include<memory>

#include "model/action.h"
#include "model/formula.h"
#include "model/reference.h"
#include "model/procedural.h"
#include "model/types.h"
#include "model/value.h"
#include "model/effect_axiom.h"

#include "synthesis/header/VarMgr.h"
/**
 * \brief A program graph with explicit states and symbolic transitions
 */
class ExplicitStateProgramGraph {

    private:
        std::shared_ptr<Syft::VarMgr> var_mgr_;

        std::size_t initial_state_;
        std::size_t state_count_;
        std::vector<std::size_t> final_states_;
        std::vector<CUDD::ADD> transition_function_;

        ExplicitStateProgramGraph(std::shared_ptr<Syft::VarMgr> var_mgr);

    public:
        /**
         * \brief Construct an explicit-state program graph from a Golog program
         * 
         * \param var_mgr The variable manager for managing transition variables
         * \param golog_program The golog program 
         */
        static ExplicitStateProgramGraph from_golog_program(
            std::shared_ptr<Syft::VarMgr> var_mgr,
            const gologpp::Procedure& golog_program);

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
         * \brief Returns the list of indices of final states of the program graph
         */
        std::vector<std::size_t> final_states() const;

        /**
         * \brief Returns the transition function of the program graph as a vector of ADDs
         *
         * The ADD in index \a i represents the transition function for state \a i
         */
        std::vector<CUDD::ADD> transition_function() const;

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