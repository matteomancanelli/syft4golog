#ifndef COMPOSITIONAL_GOLOG_SYNTHESIZER_H
#define COMPOSITIONAL_GOLOG_SYNTHESIZER_H

#include"base_golog_synthesizer.h"

class CompositionalGologSynthesizer : public BaseGologSynthesizer {

    protected:
        Syft::SymbolicStateDfa compose(
            const Syft::SymbolicStateDfa& domain_dfa,
            const Syft::SymbolicStateDfa& golog_dfa
        ) const override;

        std::shared_ptr<ExplicitStateProgramGraph> pg_;

    public:
        using BaseGologSynthesizer::BaseGologSynthesizer; // inherits construcotr

        std::shared_ptr<Syft::SynthesisResult> run() override;
        void interactive() const override;
        void dump_ts() const override {pg_->dump_dot("pg.dot");}
        void dump_domain() const override {domain_sdfa_->dump_dot("domain.dot");}
        void print_domain() const override {domain_->print_domain();}
        void dump_strategy() const override {result_->transducer->dump_dot("strategy.dot");}
};

#endif

