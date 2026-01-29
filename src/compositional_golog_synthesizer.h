#ifndef COMPOSITIONAL_GOLOG_SYNTHESIZER_H
#define COMPOSITIONAL_GOLOG_SYNTHESIZER_H

#include"base_golog_synthesizer.h"

class CompositionalGologSynthesizer : public BaseGologSynthesizer {

    protected:
        std::shared_ptr<ExplicitStateProgramGraph> pg_;

    public:
        using BaseGologSynthesizer::BaseGologSynthesizer; // inherits construcotr

        std::shared_ptr<Syft::SynthesisResult> run() override;
        void dump_ts() const override {pg_->dump_dot("pg.dot");}
};

#endif

