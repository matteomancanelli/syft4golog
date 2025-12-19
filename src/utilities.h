#pragma once

#include <set>
#include <string>
#include <vector>
#include <utility>
#include <iostream>

#include "model/expressions.h"
#include "model/value.h"
#include "model/formula.h"
#include "model/procedural.h"

#include "node_factory.h"
#include "sem_cache.h"
#include "transitions.h"

// ---------- InstructionSet: pointer identity ----------
struct PtrLess {
    bool operator()(const gologpp::Instruction* a, const gologpp::Instruction* b) const {
        return std::less<const gologpp::Instruction*>()(a, b);
    }
};

using InstructionSet = std::set<const gologpp::Instruction*, PtrLess>;

// ---------- helpers ----------
bool elementIsAlreadyPresent(const InstructionSet& set, const gologpp::Instruction* instruction);

// returns {true/false, value} if expr is boolean constant
bool getBoolValue(const gologpp::Expression* expression, bool& result);

// builds boolean op and constant-folds if possible
gologpp::Expression* createBooleanOperation(
    gologpp::BooleanOperator operation,
    gologpp::Expression* left,
    gologpp::Expression* right,
    NodeFactory& factory);

/*******************************************************************************
 * INSTRUCTION TRANSFORMATION
 * Functions for converting high-level instructions to simpler forms
 *******************************************************************************/

/**
 * Transforms a conditional instruction into an equivalent instruction using choose and test by using the following rule:
 * if (condition) then program_1 else program_2 = (condition ? ; program_1) | (¬condition ? program_2)
 */
//const gologpp::Instruction* simplifyConditional(const gologpp::Conditional<gologpp::Instruction>* conditional);

/** 
 * Transforms a while loop into an equivalent instruction using star and test by using the following rule:
 * while(condition) program = (condition? ; program)* ; ¬condition
 */
//const gologpp::Instruction* simplifyWhile(const gologpp::While* while_loop);