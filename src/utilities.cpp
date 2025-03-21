#include "utilities.h"

/*******************************************************************************
 * INSTRUCTION SET UTILITIES
 * Functions and types for handling sets of instructions with string comparison
 *******************************************************************************/

/* Custom comparator for gologpp::Instruction pointers */
bool InstructionComparator::operator()(const gologpp::Instruction* lhs, const gologpp::Instruction* rhs) const {
    if (lhs == rhs) return false;
    
    // Compare based on string representation
    std::string lhs_str = lhs->to_string("");
    std::string rhs_str = rhs->to_string("");
    return lhs_str < rhs_str;
}

/* Checks if an instruction is already in the set based on string representation */
bool isInstructionInSet(const InstructionSet& set, const gologpp::Instruction* instr) {
    std::string instr_str = instr->to_string("");
    
    for (const auto* existing : set) {
        if (existing->to_string("") == instr_str) {
            return true;
        }
    }
    
    return false;
}


/*******************************************************************************
 * INSTRUCTION TRANSFORMATION
 *******************************************************************************/

/**
 * Transforms a conditional instruction into an equivalent instruction using choose and test by using the following rule:
 * if (condition) then program_1 else program_2 = (condition ? ; program_1) | (¬condition ? program_2)
 */
const gologpp::Instruction* simplifyConditional(const gologpp::Conditional<gologpp::Instruction>* conditional) {
    // Get components
    const gologpp::Expression* condition = &conditional->condition();
    const gologpp::Instruction* then_branch = &conditional->block_true();
    const gologpp::Instruction* else_branch = &conditional->block_false();

    // Create test for the condition
    const gologpp::Instruction* test_condition = new gologpp::Test(const_cast<gologpp::Expression*>(condition));

    // Create left branch: test(condition) followed by then-branch
    const std::vector<gologpp::Instruction*>& left_seq = {const_cast<gologpp::Instruction*>(test_condition), const_cast<gologpp::Instruction*>(then_branch)};
    gologpp::Scope* left_scope = const_cast<gologpp::Scope*>(&conditional->scope().parent_scope());
    gologpp::Block* left_branch = new gologpp::Block(left_scope, left_seq);

    // Create negated condition
    gologpp::Expression* negated_condition = new gologpp::Negation(const_cast<gologpp::Expression*>(condition));
    const gologpp::Instruction* test_negated = new gologpp::Test(negated_condition);

    // Create right branch: test(¬condition) followed by else-branch
    const std::vector<gologpp::Instruction*>& right_seq = {const_cast<gologpp::Instruction*>(test_negated), const_cast<gologpp::Instruction*>(else_branch)};
    gologpp::Scope* right_scope = const_cast<gologpp::Scope*>(&conditional->scope().parent_scope());
    gologpp::Block* right_branch = new gologpp::Block(right_scope, right_seq);

    // Create choose with both branches as alternatives
    const std::vector<gologpp::Instruction*>& alternatives = {left_branch, right_branch};
    gologpp::Scope* parent_scope = const_cast<gologpp::Scope*>(&conditional->scope().parent_scope());
    const gologpp::Choose* choose = new gologpp::Choose(parent_scope, alternatives);

    return choose;
}

/** 
 * Transforms a while loop into an equivalent instruction using star and test by using the following rule:
 * while(condition) program = (condition? ; program)* ; ¬condition
 */
const gologpp::Instruction* simplifyWhile(const gologpp::While* while_loop) {
    // Get components
    const gologpp::Expression* condition = &while_loop->expression();
    const gologpp::Instruction* body = &while_loop->statement();

    // Create test for the loop condition
    const gologpp::Instruction* test_condition = new gologpp::Test(const_cast<gologpp::Expression*>(condition));

    // Create the block to be iterated: test(condition) followed by body
    const std::vector<gologpp::Instruction*>& loop_seq = {const_cast<gologpp::Instruction*>(test_condition), const_cast<gologpp::Instruction*>(body)};
    gologpp::Scope* loop_scope = const_cast<gologpp::Scope*>(&while_loop->scope().parent_scope());
    gologpp::Block* loop_block = new gologpp::Block(loop_scope, loop_seq);

    // Create star iteration
    gologpp::Instruction* star = new gologpp::Star(loop_block);

    // Create test for negated condition (loop termination)
    gologpp::Expression* negated_condition = new gologpp::Negation(const_cast<gologpp::Expression*>(condition));
    gologpp::Instruction* test_negated = new gologpp::Test(negated_condition);

    // Create final block: star iteration followed by test(¬condition)
    const std::vector<gologpp::Instruction*>& seq = {star, test_negated};
    gologpp::Scope* parent_scope = const_cast<gologpp::Scope*>(&while_loop->scope().parent_scope());
    const gologpp::Block* block = new gologpp::Block(parent_scope, seq);

    return block;
}

/*******************************************************************************
 * BOOLEAN EXPRESSION UTILITIES
 *******************************************************************************/

/* Attempts to extract a boolean value from an expression */
bool getBoolValue(const gologpp::Expression* expression, bool& result) {
    // Handle direct boolean values
    if (const gologpp::Value* value = dynamic_cast<const gologpp::Value*>(expression)) {
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

/* Creates a simplified boolean operation between two expressions */
gologpp::Expression* createBooleanOperation(gologpp::Expression* lhs, gologpp::BooleanOperator op, gologpp::Expression* rhs) {
    bool left_bool;
    bool right_bool;

    switch (op) {
        case gologpp::BooleanOperator::AND:
            // If either side is false, the result is false
            if ((getBoolValue(lhs, left_bool) && !left_bool) || (getBoolValue(rhs, right_bool) && !right_bool)) {
                return new gologpp::Value(gologpp::type<gologpp::BoolType>(), false);
            }
            // If left side is true, return right side
            else if ((getBoolValue(lhs, left_bool) && left_bool)) {
                return rhs;
            }
            // If right side is true, return left side
            else if ((getBoolValue(rhs, right_bool) && right_bool)) {
                return lhs;
            }
            // Cannot simplify, create normal boolean operation
            else {
                return new gologpp::BooleanOperation(lhs, op, rhs);
            }
            break;
        
        case gologpp::BooleanOperator::OR:
            // If either side is true, the result is true
            if ((getBoolValue(lhs, left_bool) && left_bool) || (getBoolValue(rhs, right_bool) && right_bool)) {
                return new gologpp::Value(gologpp::type<gologpp::BoolType>(), true);
            }
            // If left side is false, return right side
            else if ((getBoolValue(lhs, left_bool) && !left_bool)) {
                return rhs;
            }
            // If right side is false, return left side
            else if ((getBoolValue(rhs, right_bool) && !right_bool)) {
                return lhs;
            }
            // Cannot simplify, create normal boolean operation
            else {
                return new gologpp::BooleanOperation(lhs, op, rhs);
            }
            break;
        
        default:
            break;
    }

    // Default case (should never be reached in normal operation)
    return new gologpp::BooleanOperation(lhs, op, rhs);
}
