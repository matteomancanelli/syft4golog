#ifndef LDLF_COMPOSITIONAL_GOLOG_SYNTHESIZER_H
#define LDLF_COMPOSITIONAL_GOLOG_SYNTHESIZER_H

#include"base_golog_synthesizer.h"
#include"lydia/logic/ldlf/base.hpp"

class LDLfCompositionalGologSynthesizer : public BaseGologSynthesizer {

    private:
        whitemech::lydia::ldlf_ptr golog_to_ldlf() const;

    public:
        Syft::SynthesisResult run() const override;

};

#endif