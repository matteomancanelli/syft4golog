#ifndef COMPOSITIONAL_GOLOG_SYNTHESIZER_H
#define COMPOSITIONAL_GOLOG_SYNTHESIZER_H

#include"base_golog_synthesizer.h"

class CompositionalGologSynthesizer : public BaseGologSynthesizer {

    public:
        Syft::SynthesisResult run() const override;

};

#endif

