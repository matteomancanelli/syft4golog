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
 * Computes the syntactic closure of a given program instruction
 * @param instruction The program instruction for which to compute the syntactic closure
 * @return A set containing all instructions in the syntactic closure
 */
InstructionSet computeSyntacticClosure(const gologpp::Instruction* instruction);

#endif // SYNTACTIC_CLOSURE_H