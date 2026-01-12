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
        GologProgramNode* result;
        GologDriver() {};
        virtual ~GologDriver();
        GologProgramNode* get_result() {return result;}
        void parse(const char* const filename);
        void parse(std::istream& iss);

        PropositionalLogicNode* add_PropositionalLogicTrue() const;
        PropositionalLogicNode* add_PropositionalLogicFalse() const;
        PropositionalLogicNode* add_PropositionalLogicAtom(std::string s) const;
        PropositionalLogicNode* add_PropositionalLogicNegation(
            PropositionalLogicNode& arg
        ) const;
        PropositionalLogicNode* add_PropositionalLogicConjunction(
            PropositionalLogicNode& lhs,
            PropositionalLogicNode& rhs
        ) const;
        PropositionalLogicNode* add_PropositionalLogicDisjunction(
            PropositionalLogicNode& lhs,
            PropositionalLogicNode& rhs
        ) const;
        PropositionalLogicNode* add_PropositionalLogicImplication(
            PropositionalLogicNode& lhs,
            PropositionalLogicNode& rhs
        ) const;
        PropositionalLogicNode* add_PropositionalLogicEquivalence(
            PropositionalLogicNode& lhs,
            PropositionalLogicNode& rhs
        ) const;
        
        GologProgramNode* add_GologProgramNil() const;
        GologProgramNode* add_GologProgramUnd() const;
        GologProgramNode* add_GologProgramAction(std::string s) const;
        GologProgramNode* add_GologProgramTest(PropositionalLogicNode& t) const;
        GologProgramNode* add_GologProgramSequence(
            GologProgramNode& fst,
            GologProgramNode& snd
        ) const;
        GologProgramNode* add_GologProgramChoice(
            GologProgramNode& lhs,
            GologProgramNode& rhs
        ) const;
        GologProgramNode* add_GologProgramIteration(
            GologProgramNode& prg
        ) const;
        std::ostream& print(std::ostream& stream) const;
};