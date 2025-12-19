#pragma once

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
#include "node_factory.h"

/**
 * Computes the syntactic closure of a given program instruction.
 * The syntactic closure is the set of all subprograms that can be reached during execution.
 */
void computeSyntacticClosure(const gologpp::Instruction* instruction, InstructionSet& syntactic_closure, NodeFactory& factory);