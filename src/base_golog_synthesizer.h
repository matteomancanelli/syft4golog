#ifndef GOLOG_SYNTHESIZER_H
#define GOLOG_SYNTHESIZER_H

#include<memory>
#include<fstream>

#include "domain.h"
#include "synthesis/header/VarMgr.h"
#include "synthesis/header/automata/SymbolicStateDfa.h"
#include "synthesis/header/Synthesizer.h"
#include "synthesis/header/game/Reachability.hpp"
#include "synthesis/header/Stopwatch.h"

#include "program_graph.h"
#include "golog_driver.hpp"

using Stopwatch = Syft::Stopwatch;

class BaseGologSynthesizer{

    protected:
        std::shared_ptr<Syft::VarMgr> var_mgr_;
        std::shared_ptr<Domain> domain_;
        golog_ptr golog_program_;
        std::shared_ptr<Syft::SymbolicStateDfa> domain_sdfa_;
        std::shared_ptr<Syft::SymbolicStateDfa> game_arena_;
        std::shared_ptr<Syft::SynthesisResult> result_;
        std::vector<double> running_times_;
        std::vector<double> running_data_;        

        Syft::SymbolicStateDfa compose(
            const Syft::SymbolicStateDfa& domain_dfa,
            const Syft::SymbolicStateDfa& golog_program_dfa
        ) const;

    public:
        BaseGologSynthesizer(
            const std::string& domain_file,
            const std::string& init_file,
            const std::string& golog_file
        );

        std::vector<double> running_times() const {return running_times_;}
        std::vector<double> running_data() const {return running_data_;}

        virtual std::shared_ptr<Syft::SynthesisResult> run() = 0;
        void interactive() const; 
        virtual void dump_ts() const = 0;
        void dump_domain() const {domain_sdfa_->dump_dot("domain.dot");}
        void print_domain() const {domain_->print_domain();}
        void dump_strategy() const {result_->transducer->dump_dot("strategy.dot");}
};

#endif