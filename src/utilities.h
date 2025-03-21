#ifndef UTILITIES_H
#define UTILITIES_H

#include <set>
#include <string>

#include "model/expressions.h"
#include "model/value.h"
#include "model/formula.h"
#include "model/procedural.h"


/*******************************************************************************
 * INSTRUCTION SET UTILITIES
 * Functions and types for handling sets of instructions with string comparison
 *******************************************************************************/

/* Custom comparator for gologpp::Instruction pointers */
struct InstructionComparator {
    bool operator()(const gologpp::Instruction* lhs, const gologpp::Instruction* rhs) const;
};

/* A set type that stores instructions and compares them by their string representation */
using InstructionSet = std::set<const gologpp::Instruction*, InstructionComparator>;

/* Checks if an instruction with the same string representation is already in the set */
bool isInstructionInSet(const InstructionSet& set, const gologpp::Instruction* instr);


/*******************************************************************************
 * INSTRUCTION TRANSFORMATION
 * Functions for converting high-level instructions to simpler forms
 *******************************************************************************/

/**
 * Transforms a conditional instruction into an equivalent instruction using choose and test by using the following rule:
 * if (condition) then program_1 else program_2 = (condition ? ; program_1) | (¬condition ? program_2)
 */
const gologpp::Instruction* simplifyConditional(const gologpp::Conditional<gologpp::Instruction>* conditional);

/** 
 * Transforms a while loop into an equivalent instruction using star and test by using the following rule:
 * while(condition) program = (condition? ; program)* ; ¬condition
 */
const gologpp::Instruction* simplifyWhile(const gologpp::While* while_loop);


/*******************************************************************************
 * BOOLEAN EXPRESSION UTILITIES
 * Functions for evaluating and simplifying boolean expressions
 *******************************************************************************/

/**
 * Attempts to extract a boolean value from an expression
 * 
 * @param expression The expression to evaluate
 * @param result Output parameter where the boolean value will be stored if successful
 * @return true if the expression could be evaluated to a boolean
 */
bool getBoolValue(const gologpp::Expression* expression, bool& result);

/**
 * Creates a simplified boolean operation between two expressions
 * 
 * @param lhs Left-hand side expression
 * @param op Boolean operator (AND or OR)
 * @param rhs Right-hand side expression
 * @return A simplified expression representing the boolean operation
 */
gologpp::Expression* createBooleanOperation(gologpp::Expression* lhs, gologpp::BooleanOperator op, gologpp::Expression* rhs);


#endif // UTILITIES_H