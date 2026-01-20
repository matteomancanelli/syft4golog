#ifndef COMPOSITIONAL_GOLOG_SYNTHESIZER_H
#define COMPOSITIONAL_GOLOG_SYNTHESIZER_H

#include"base_golog_synthesizer.h"

class CompositionalGologSynthesizer : public BaseGologSynthesizer {

    public:
        using BaseGologSynthesizer::BaseGologSynthesizer; // inherits construcotr

        Syft::SynthesisResult run() const override;
        void interactive(
            const Domain& domain, 
            const Syft::SymbolicStateDfa& game_arena,
            const Syft::SynthesisResult& result
        ) const;
};

#endif

