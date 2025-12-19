#pragma once

#include <set>
#include <vector>
#include <utility>

#include "model/procedural.h"
#include "model/formula.h"
#include "model/reference.h"
#include "model/types.h"
#include "model/value.h"
#include "model/action.h"
#include "model/effect_axiom.h"

#include "utilities.h"
#include "node_factory.h"
#include "sem_cache.h"
#include "transitions_type.h"

/** 
 * Computes the final condition (F) of an instruction.
 * The final condition is true in a state iff the program can legally terminate.
 */
gologpp::Expression* F(const gologpp::Instruction* instruction, NodeFactory& factory, SemCache& cache);

/** 
 * Computes the transition relation (T) of an instruction with respect to an action.
 * The transition relation defines the program configurations that can be reached by executing a given action. 
 */
Transitions T(const gologpp::Instruction* instruction, const gologpp::Reference<gologpp::Action>* target_action, NodeFactory& factory, SemCache& cache);