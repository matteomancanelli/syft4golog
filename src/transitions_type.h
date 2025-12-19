#pragma once

#include <vector>
#include <utility>

#include "model/formula.h"
#include "model/procedural.h"

using Transition  = std::pair<gologpp::Expression*, gologpp::Instruction*>;
using Transitions = std::vector<Transition>;