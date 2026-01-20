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

        std::shared_ptr<Syft::SynthesisResult> run() override;
        void interactive() const override;
};

#endif

