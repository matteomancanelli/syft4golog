#ifndef LDLF_GOLOG_SYNTHESIZER_H
#define LDLF_GOLOG_SYNTHESIZER_H

#include"base_golog_synthesizer.h"
#include"lydia/logic/ldlf/base.hpp"
#include"lydia/utils/print.hpp"

class LDLfGologSynthesizer : public BaseGologSynthesizer {

    protected:
        std::shared_ptr<Syft::ExplicitStateDfaAdd> pdfa_;

        Syft::SymbolicStateDfa compose(
            const Syft::SymbolicStateDfa& domain_dfa,
            const Syft::SymbolicStateDfa& golog_program_dfa
        ) const;

    public:
        using BaseGologSynthesizer::BaseGologSynthesizer;

        std::shared_ptr<Syft::SynthesisResult> run() override;
        void dump_ts() const override {pdfa_->dump_dot("pdfa.dot");}
};

#endif