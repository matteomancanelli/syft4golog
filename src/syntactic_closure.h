#ifndef SYNTACTIC_CLOSURE_H
#define SYNTACTIC_CLOSURE_H

#include <set>
#include <vector>
#include <memory>

#include "model/procedural.h"
#include "model/formula.h"
#include "model/reference.h"
#include "model/types.h"
#include "model/value.h"
#include "model/effect_axiom.h"

#include "utilities.h"

/**
 * Simplifies a conditional instruction into an equivalent choose instruction
 * @param conditional The conditional instruction to simplify
 * @return A new instruction representing the simplified form
 */
const gologpp::Instruction* simplifyConditional(const gologpp::Conditional<gologpp::Instruction>* conditional);

/**
 * Simplifies a while loop into an equivalent form using star operator
 * @param while_loop The while loop to simplify
 * @return A new instruction representing the simplified form
 */
const gologpp::Instruction* simplifyWhile(const gologpp::While* while_loop);

/**
 * Computes the syntactic closure of a given program instruction
 * @param instruction The program instruction for which to compute the syntactic closure
 * @return A set containing all instructions in the syntactic closure
 */
std::set<const gologpp::Instruction*> getSyntacticClosure(const gologpp::Instruction* instruction);

#endif // SYNTACTIC_CLOSURE_H