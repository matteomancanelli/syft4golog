#include "utilities.h"

bool elementIsAlreadyPresent(const InstructionSet& set, const gologpp::Instruction* instruction) {
  return set.find(instruction) != set.end();
}

/*******************************************************************************
 * BOOLEAN EXPRESSION UTILITIES
 *******************************************************************************/

/* Attempts to extract a boolean value from an expression */
bool getBoolValue(const gologpp::Expression* expression, bool& result) {
    // Handle direct boolean values
    if (const gologpp::Value* value = dynamic_cast<const gologpp::Value*>(gologpp::type<gologpp::BoolType>(), expression)) {
        result = boost::get<bool>(value->representation());
        return true;
    }
    // Handle negations
    else if (const gologpp::Negation* negation = dynamic_cast<const gologpp::Negation*>(expression)) {
        const gologpp::Expression* neg_expr = &negation->expression();
        bool neg_result;

        if (getBoolValue(neg_expr, neg_result)) {
            result = !neg_result;
            return true;
        }
    }
    
    // Cannot determine a boolean value
    return false;
}

gologpp::Expression* createBooleanOperation(gologpp::BooleanOperator operation, gologpp::Expression* left, gologpp::Expression* right, NodeFactory& factory) {
    bool left_val;
    bool right_val;

    bool l_is_const = getBoolValue(left, left_val);
    bool r_is_const = getBoolValue(right, right_val);

    if (l_is_const && r_is_const) {
    	if (operation == gologpp::BooleanOperator::AND)	return factory.makeExpr<gologpp::Value>(gologpp::type<gologpp::BoolType>(), left_val && right_val);
    	if (operation == gologpp::BooleanOperator::OR)	return factory.makeExpr<gologpp::Value>(gologpp::type<gologpp::BoolType>(), left_val || right_val);
    }

    if (operation == gologpp::BooleanOperator::AND) {
    	if (l_is_const && !left_val)    return factory.makeExpr<gologpp::Value>(gologpp::type<gologpp::BoolType>(), false);
    	if (r_is_const && !right_val)   return factory.makeExpr<gologpp::Value>(gologpp::type<gologpp::BoolType>(), false);
    	if (l_is_const && left_val)     return right;
    	if (r_is_const && right_val)    return left;
    }
    if (operation == gologpp::BooleanOperator::OR) {
    	if (l_is_const && left_val)     return factory.makeExpr<gologpp::Value>(gologpp::type<gologpp::BoolType>(), true);
    	if (r_is_const && right_val)    return factory.makeExpr<gologpp::Value>(gologpp::type<gologpp::BoolType>(), true);
    	if (l_is_const && !left_val)    return right;
    	if (r_is_const && !right_val)   return left;
    }

    return factory.makeExpr<gologpp::BooleanOperation>(left, operation, right);
}
