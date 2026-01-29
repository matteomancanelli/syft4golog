#ifndef LDLF_GOLOG_SYNTHESIZER_H
#define LDLF_GOLOG_SYNTHESIZER_H

#include"base_golog_synthesizer.h"
#include"lydia/logic/ldlf/base.hpp"

class LDLfGologSynthesizer : public BaseGologSynthesizer {

    protected:
        std::shared_ptr<Syft::ExplicitStateDfaAdd> pdfa_;

    public:
        using BaseGologSynthesizer::BaseGologSynthesizer;

        std::shared_ptr<Syft::SynthesisResult> run() override;
        void dump_ts() const override {pdfa_->dump_dot("pdfa.dot");}
};

#endif