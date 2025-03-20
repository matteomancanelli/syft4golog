#ifndef TRANSITIONS_H
#define TRANSITIONS_H

#include <set>
#include <utility>

#include "model/procedural.h"
#include "model/formula.h"
#include "model/reference.h"
#include "model/types.h"
#include "model/value.h"
#include "model/action.h"
#include "model/effect_axiom.h"

#include "utilities.h"

/**
 * Computes the final condition (F) of an instruction
 * @param instruction The instruction to evaluate
 * @return An expression representing the final condition
 */
gologpp::Expression* F(const gologpp::Instruction* instruction);

/**
 * Computes the transition relation (T) of an instruction with respect to an action
 * @param instruction The instruction to evaluate
 * @param action The action with respect to which transitions are computed
 * @return A set of condition/instruction pairs representing possible transitions
 */
std::set<std::pair<gologpp::Expression*, gologpp::Instruction*>> T(
    const gologpp::Instruction* instruction, 
    const gologpp::Reference<gologpp::Action>* action
);

#endif // TRANSITIONS_H