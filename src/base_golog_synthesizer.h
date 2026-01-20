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
        std::shared_ptr<Syft::SymbolicStateDfa> game_arena_;
        std::shared_ptr<Syft::SynthesisResult> result_;
        std::vector<double> running_times_;        

        virtual Syft::SymbolicStateDfa compose(
            const Syft::SymbolicStateDfa& domain_dfa,
            const Syft::SymbolicStateDfa& golog_program_dfa
        ) const = 0;

    public:
        BaseGologSynthesizer(
            const std::string& domain_file,
            const std::string& init_file,
            const std::string& golog_file
        );

        std::vector<double> running_times() const {return running_times_;}

        virtual std::shared_ptr<Syft::SynthesisResult> run() = 0;
        virtual void interactive() const = 0; 
};

#endif