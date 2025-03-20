#include "utilities.h"

bool getBoolValue(const gologpp::Expression* expression, bool& result) {

    if (const gologpp::Value* value = dynamic_cast<const gologpp::Value*>(expression)) {
        result = boost::get<bool>(value->representation());
        return true;
    }
    else if (const gologpp::Negation* negation = dynamic_cast<const gologpp::Negation*>(expression)) {
        const gologpp::Expression* neg_expr = &negation->expression();
        bool neg_result;

        if (getBoolValue(neg_expr, neg_result)) {
            result = !neg_result;
            return true;
        }
    }
    
    return false;
}

gologpp::Expression* createBooleanOperation(gologpp::Expression* lhs, gologpp::BooleanOperator op, gologpp::Expression* rhs) {
    bool left_bool;
    bool right_bool;

    switch (op) {
        case gologpp::BooleanOperator::AND:
            if ((getBoolValue(lhs, left_bool) && !left_bool) || (getBoolValue(rhs, right_bool) && !right_bool)) {
                return new gologpp::Value(gologpp::type<gologpp::BoolType>(), false);
            }
            else if ((getBoolValue(lhs, left_bool) && left_bool)) {
                return rhs;
            }
            else if ((getBoolValue(rhs, right_bool) && right_bool)) {
                return lhs;
            }
            else {
                return new gologpp::BooleanOperation(lhs, op, rhs);
            }
            break;
        
        case gologpp::BooleanOperator::OR:
            if ((getBoolValue(lhs, left_bool) && left_bool) || (getBoolValue(rhs, right_bool) && right_bool)) {
                return new gologpp::Value(gologpp::type<gologpp::BoolType>(), true);
            }
            else if ((getBoolValue(lhs, left_bool) && !left_bool)) {
                return rhs;
            }
            else if ((getBoolValue(rhs, right_bool) && !right_bool)) {
                return lhs;
            }
            else {
                return new gologpp::BooleanOperation(lhs, op, rhs);
            }
            break;
        
        default:
            break;
    }

    return NULL;
}

const gologpp::Instruction* simplifyConditional(const gologpp::Conditional<gologpp::Instruction>* conditional) {
    const gologpp::Expression* condition = &conditional->condition();
    const gologpp::Instruction* then_branch = &conditional->block_true();
    const gologpp::Instruction* else_branch = &conditional->block_false();

    const gologpp::Instruction* test_condition = new gologpp::Test(const_cast<gologpp::Expression*>(condition));

    const std::vector<gologpp::Instruction*>& left_seq = {const_cast<gologpp::Instruction*>(test_condition), const_cast<gologpp::Instruction*>(then_branch)};
    gologpp::Scope* left_scope = const_cast<gologpp::Scope*>(&conditional->scope().parent_scope());
    gologpp::Block* left_branch = new gologpp::Block(left_scope, left_seq);

    gologpp::Expression* negated_condition = new gologpp::Negation(const_cast<gologpp::Expression*>(condition));
    const gologpp::Instruction* test_negated = new gologpp::Test(negated_condition);

    const std::vector<gologpp::Instruction*>& right_seq = {const_cast<gologpp::Instruction*>(test_negated), const_cast<gologpp::Instruction*>(else_branch)};
    gologpp::Scope* right_scope = const_cast<gologpp::Scope*>(&conditional->scope().parent_scope());
    gologpp::Block* right_branch = new gologpp::Block(right_scope, right_seq);

    const std::vector<gologpp::Instruction*>& alternatives = {left_branch, right_branch};
    gologpp::Scope* parent_scope = const_cast<gologpp::Scope*>(&conditional->scope().parent_scope());
    const gologpp::Choose* choose = new gologpp::Choose(parent_scope, alternatives);

    return choose;
}

const gologpp::Instruction* simplifyWhile(const gologpp::While* while_loop) {
    const gologpp::Expression* condition = &while_loop->expression();
    const gologpp::Instruction* body = &while_loop->statement();

    const gologpp::Instruction* test_condition = new gologpp::Test(const_cast<gologpp::Expression*>(condition));

    const std::vector<gologpp::Instruction*>& loop_seq = {const_cast<gologpp::Instruction*>(test_condition), const_cast<gologpp::Instruction*>(body)};
    gologpp::Scope* loop_scope = const_cast<gologpp::Scope*>(&while_loop->scope().parent_scope());
    gologpp::Block* loop_block = new gologpp::Block(loop_scope, loop_seq);

    gologpp::Instruction* star = new gologpp::Star(loop_block);

    gologpp::Expression* negated_condition = new gologpp::Negation(const_cast<gologpp::Expression*>(condition));
    gologpp::Instruction* test_negated = new gologpp::Test(negated_condition);

    const std::vector<gologpp::Instruction*>& seq = {star, test_negated};
    gologpp::Scope* parent_scope = const_cast<gologpp::Scope*>(&while_loop->scope().parent_scope());
    const gologpp::Block* block = new gologpp::Block(parent_scope, seq);

    return block;
}