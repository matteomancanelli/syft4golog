#include "transitions.h"

gologpp::Expression* F(const gologpp::Instruction* instruction) {
    gologpp::Expression* fin;

    if (const auto* block = dynamic_cast<const gologpp::Block*>(instruction)) {
        if (block->elements().size() == 1) {
            return F(block->elements().front().get());
        }

        fin = new gologpp::Value(gologpp::type<gologpp::BoolType>(), true);
        gologpp::BooleanOperator op = gologpp::BooleanOperator::AND;
        
        for (const auto& instr : block->elements()) {
            gologpp::Expression* instr_F = F(instr.get());
            fin = createBooleanOperation(fin, op, instr_F);
        }
    }
    else if (const auto* choose = dynamic_cast<const gologpp::Choose*>(instruction)) {
        fin = new gologpp::Value(gologpp::type<gologpp::BoolType>(), false);
        gologpp::BooleanOperator op = gologpp::BooleanOperator::OR;

        for (const auto& alternative : choose->alternatives()) {
            gologpp::Expression* branch_F = F(alternative.get());
            fin = createBooleanOperation(fin, op, branch_F);
        }
    }
    else if (const auto* star = dynamic_cast<const gologpp::Star*>(instruction)) {        
        fin = new gologpp::Value(gologpp::type<gologpp::BoolType>(), true);
    }
    else if (const auto* pick = dynamic_cast<const gologpp::Pick*>(instruction)) {
        // TODO (maybe)
    }
    else if (const auto* test = dynamic_cast<const gologpp::Test*>(instruction)) {
        fin = const_cast<gologpp::Expression*>(&test->expression());
    }
    else if (const auto* action_call = dynamic_cast<const gologpp::Reference<gologpp::Action>*>(instruction)) {
        fin = new gologpp::Value(gologpp::type<gologpp::BoolType>(), false);
    }

    return fin;
} 

std::set<std::pair<gologpp::Expression*, gologpp::Instruction*>> T(const gologpp::Instruction* instruction, const gologpp::Reference<gologpp::Action>* action) {
    std::set<std::pair<gologpp::Expression*, gologpp::Instruction*>> trans;

    if (const auto* block = dynamic_cast<const gologpp::Block*>(instruction)) {
        if (block->elements().size() == 1) {
            return T(block->elements().front().get(), action);
        }
        else {
            const gologpp::Instruction* first_instruction = block->elements().front().get();
            std::vector<gologpp::Instruction*> rest_elements;

            for (size_t i = 1; i < block->elements().size(); ++i) {
                gologpp::Instruction* instr = const_cast<gologpp::Instruction*>(block->elements()[i].get());
                rest_elements.push_back(instr);
            }

            gologpp::Scope* parent_scope = const_cast<gologpp::Scope*>(&block->scope().parent_scope());
            const gologpp::Instruction* rest_block = new gologpp::Block(parent_scope, rest_elements);

            for (auto [condition, subprogram] : T(first_instruction, action)) {
                gologpp::Expression* first_expr = new gologpp::Negation(F(first_instruction));
                gologpp::Expression* new_condition = createBooleanOperation(first_expr, gologpp::BooleanOperator::AND, condition);
                gologpp::Scope* parent_scope = const_cast<gologpp::Scope*>(&block->scope().parent_scope());
                gologpp::Instruction* new_subprogram = new gologpp::Block(parent_scope, {const_cast<gologpp::Instruction*>(subprogram), const_cast<gologpp::Instruction*>(rest_block)});

                bool bool_value;
                if (!(getBoolValue(new_condition, bool_value) && !bool_value)) {
                    trans.insert(std::make_pair(new_condition, new_subprogram));
                }
            }

            for (auto [condition, subprogram] : T(rest_block, action)) {
                gologpp::Expression* first_expr = F(first_instruction);
                gologpp::Expression* new_condition = createBooleanOperation(first_expr, gologpp::BooleanOperator::AND, condition);

                bool bool_value;
                if (!(getBoolValue(new_condition, bool_value) && !bool_value)) {
                    trans.insert(std::make_pair(new_condition, subprogram));
                }
            }
        }
    }
    else if (const auto* choose = dynamic_cast<const gologpp::Choose*>(instruction)) {
        for (const auto& alternative : choose->alternatives()) {
            auto branch_T = T(alternative.get(), action);
            trans.insert(branch_T.begin(), branch_T.end());
        }
    }
    else if (const auto* star = dynamic_cast<const gologpp::Star*>(instruction)) {
        const gologpp::Instruction* body = &star->statement();
        
        for (auto [condition, subprogram] : T(body, action)) {
            gologpp::Expression* first_expr = new gologpp::Negation(F(body));
            gologpp::Expression* new_condition = createBooleanOperation(first_expr, gologpp::BooleanOperator::AND, condition);
            gologpp::Scope* parent_scope = const_cast<gologpp::Scope*>(&star->scope().parent_scope());
            gologpp::Instruction* new_subprogram = new gologpp::Block(parent_scope, {const_cast<gologpp::Instruction*>(subprogram), const_cast<gologpp::Instruction*>(instruction)});

            bool bool_value;
            if (!(getBoolValue(new_condition, bool_value) && !bool_value)) {
                trans.insert(std::make_pair(new_condition, new_subprogram));
            }
        }
    }
    else if (const auto* pick = dynamic_cast<const gologpp::Pick*>(instruction)) {
        // TODO (maybe)
    }
    else if (const auto* test = dynamic_cast<const gologpp::Test*>(instruction)) {
        // Do nothing
    }
    else if (const auto* action_call = dynamic_cast<const gologpp::Reference<gologpp::Action>*>(instruction)) {
        if (action_call->to_string("") == action->to_string("")) {
            gologpp::Instruction* nil_instruction = new gologpp::Test(new gologpp::Value(gologpp::type<gologpp::BoolType>(), true));
            gologpp::Action& target_action = *action->target();
            auto action_precondition = &target_action.precondition();

            bool bool_value;
            if (!(getBoolValue(action_precondition, bool_value) && !bool_value)) {
                trans.insert(std::make_pair(action_precondition, nil_instruction));
            }
        }
    }

    return trans;
}
