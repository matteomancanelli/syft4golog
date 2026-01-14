#pragma once

#include<cassert>
#include<fstream>

#include"golog_program.hpp"
#include"parser.tab.hh"
#include"scanner.hpp"

class GologDriver {

    private:
        void parse_helper(std::istream& stream);

        GologScanner* scanner = nullptr;
        yy::GologParser* parser = nullptr;

    public:
        golog_ptr result;
        GologDriver() {};
        virtual ~GologDriver();
        golog_ptr get_result() {return result;}
        void parse(const char* const filename);
        void parse(std::istream& iss);

        formula_ptr add_PropositionalLogicTrue() const;
        formula_ptr add_PropositionalLogicFalse() const;
        formula_ptr add_PropositionalLogicAtom(std::string s) const;
        formula_ptr add_PropositionalLogicNegation(
            formula_ptr& arg
        ) const;
        formula_ptr add_PropositionalLogicConjunction(
            formula_ptr& lhs,
            formula_ptr& rhs
        ) const;
        formula_ptr add_PropositionalLogicDisjunction(
            formula_ptr& lhs,
            formula_ptr& rhs
        ) const;
        formula_ptr add_PropositionalLogicImplication(
            formula_ptr& lhs,
            formula_ptr& rhs
        ) const;
        formula_ptr add_PropositionalLogicEquivalence(
            formula_ptr& lhs,
            formula_ptr& rhs
        ) const;
        
        golog_ptr add_GologProgramNil() const;
        golog_ptr add_GologProgramUnd() const;
        golog_ptr add_GologProgramAction(std::string s) const;
        golog_ptr add_GologProgramTest(formula_ptr& t) const;
        golog_ptr add_GologProgramSequence(
            golog_ptr& fst,
            golog_ptr& snd
        ) const;
        golog_ptr add_GologProgramChoice(
            golog_ptr& lhs,
            golog_ptr& rhs
        ) const;
        golog_ptr add_GologProgramIteration(
            golog_ptr& prg
        ) const;
        std::ostream& print(std::ostream& stream) const;
};