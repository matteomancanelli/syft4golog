#ifndef COMPOSITIONAL_GOLOG_SYNTHESIZER_H
#define COMPOSITIONAL_GOLOG_SYNTHESIZER_H

#include"base_golog_synthesizer.h"

class CompositionalGologSynthesizer : public BaseGologSynthesizer {

    protected:
        Syft::SymbolicStateDfa compose(
            const Syft::SymbolicStateDfa& domain_dfa,
            const Syft::SymbolicStateDfa& golog_dfa
        ) const override;

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

