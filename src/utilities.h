#ifndef UTILITIES_H
#define UTILITIES_H

#include <set>
#include <string>

#include "model/expressions.h"
#include "model/value.h"
#include "model/formula.h"
#include "model/procedural.h"

// Declaration of the comparator (implementation in cpp file)
struct InstructionComparator {
    bool operator()(const gologpp::Instruction* lhs, const gologpp::Instruction* rhs) const;
};

// Define the custom instruction set type
using InstructionSet = std::set<const gologpp::Instruction*, InstructionComparator>;


bool isInstructionInSet(const InstructionSet& set, const gologpp::Instruction* instr);

const gologpp::Instruction* simplifyConditional(const gologpp::Conditional<gologpp::Instruction>* conditional);
const gologpp::Instruction* simplifyWhile(const gologpp::While* while_loop);

bool getBoolValue(const gologpp::Expression* expression, bool& result);
gologpp::Expression* createBooleanOperation(gologpp::Expression* lhs, gologpp::BooleanOperator op, gologpp::Expression* rhs);

#endif // UTILITIES_H