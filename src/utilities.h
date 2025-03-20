#include <iostream>

#include "model/expressions.h"
#include "model/value.h"
#include "model/formula.h"
#include "model/procedural.h"


bool getBoolValue(const gologpp::Expression* expression, bool& result);
gologpp::Expression* createBooleanOperation(gologpp::Expression* lhs, gologpp::BooleanOperator op, gologpp::Expression* rhs);
const gologpp::Instruction* simplifyConditional(const gologpp::Conditional<gologpp::Instruction>* conditional);
const gologpp::Instruction* simplifyWhile(const gologpp::While* while_loop);