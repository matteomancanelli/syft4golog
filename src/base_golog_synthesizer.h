#ifndef GOLOG_SYNTHESIZER_H
#define GOLOG_SYNTHESIZER_H

#include<memory>
#include<fstream>

#include "domain.h"
#include "synthesis/header/VarMgr.h"
#include "synthesis/header/automata/SymbolicStateDfa.h"
#include "synthesis/header/Synthesizer.h"
#include "synthesis/header/game/Reachability.hpp"

#include "program_graph.h"
#include "golog_driver.hpp"

class BaseGologSynthesizer{

    protected:
        std::shared_ptr<Syft::VarMgr> var_mgr_;
        std::shared_ptr<Domain> domain_;
        golog_ptr golog_program_;        

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

        virtual Syft::SynthesisResult run() const = 0;
};

#endif