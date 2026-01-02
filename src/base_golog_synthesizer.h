#ifndef GOLOG_SYNTHESIZER_H
#define GOLOG_SYNTHESIZER_H

#include<memory>

#include "parser/parser.h"

#include "model/action.h"
#include "model/formula.h"
#include "model/reference.h"
#include "model/procedural.h"
#include "model/types.h"
#include "model/value.h"
#include "model/effect_axiom.h"

#include "domain.h"
#include "synthesis/header/VarMgr.h"
#include "synthesis/header/automata/SymbolicStateDfa.h"
#include "synthesis/header/Synthesizer.h"

class BaseGologSynthesizer{

    private:
        std::shared_ptr<Syft::VarMgr> var_mgr_;
        std::shared_ptr<Domain> domain_;
        std::shared_ptr<gologpp::Procedure> golog_program_;

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

        virtual Syft::SynthesisResult run() const = 0;
};

#endif