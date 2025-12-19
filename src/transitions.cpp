#include "transitions.h"

static gologpp::Expression* F_impl(const gologpp::Instruction* instruction, NodeFactory& factory, SemCache& cache) {
    factory.initSingletons();

    // Test
    if (const auto* test = dynamic_cast<const gologpp::Test*>(instruction)) {
        return const_cast<gologpp::Expression*>(&test->expression());
    }

    // Action
    if (dynamic_cast<const gologpp::Reference<gologpp::Action>*>(instruction)) {
        return factory.makeExpr<gologpp::Value>(gologpp::type<gologpp::BoolType>(), false);
    }

    // Block (sequence)
    if (const auto* block = dynamic_cast<const gologpp::Block*>(instruction)) {
        if (block->elements().empty()) 
            return factory.makeExpr<gologpp::Value>(gologpp::type<gologpp::BoolType>(), true);

        gologpp::Instruction* first = block->elements().front().get();
        gologpp::BooleanOperator op = gologpp::BooleanOperator::AND;

        gologpp::Expression* fin = F(first, factory, cache);
        for (std::size_t i = 1; i < block->elements().size(); ++i) {
            gologpp::Expression* instr_F = F(block->elements()[i].get(), factory, cache);
            fin = createBooleanOperation(op, fin, instr_F, factory);        
        }
        
        return fin;
    }

    // Choose
    if (const auto* c = dynamic_cast<const gologpp::Choose*>(instruction)) {
        const auto& alternatives = c->alternatives();
        if (alternatives.empty()) 
            return factory.makeExpr<gologpp::Value>(gologpp::type<gologpp::BoolType>(), false);

        gologpp::BooleanOperator op = gologpp::BooleanOperator::OR;
        gologpp::Expression* fin = F(alternatives[0].get(), factory, cache);
        for (std::size_t i = 1; i < alternatives.size(); ++i) {
            gologpp::Expression* instr_F = F(alternatives[i].get(), factory, cache);
            fin = createBooleanOperation(op, fin, instr_F, factory);
        }

        return fin;
    }

    // Star
    if (dynamic_cast<const gologpp::Star*>(instruction)) {
        return factory.makeExpr<gologpp::Value>(gologpp::type<gologpp::BoolType>(), true);
    }

    // Pick (not supported for now)
    if (dynamic_cast<const gologpp::Pick*>(instruction)) {
        throw std::runtime_error("Pick is not supported in this implementation.");
    }

    throw std::runtime_error("F: Unsupported instruction type.");
}

gologpp::Expression* F(const gologpp::Instruction* instruction, NodeFactory& factory, SemCache& cache) {
    auto it = cache.Fcache.find(instruction);
    if (it != cache.Fcache.end()) return it->second;
    
    auto* res = F_impl(instruction, factory, cache);
    cache.Fcache[instruction] = res;
    return res;
}


static Transitions T_impl(const gologpp::Instruction* instruction, const gologpp::Reference<gologpp::Action>* target_action, NodeFactory& factory, SemCache& cache) {
    factory.initSingletons();

    Transitions out;

    // Test: no action transitions
    if (dynamic_cast<const gologpp::Test*>(instruction)) {
        return out;
    }

    // Action
    if (const auto* act = dynamic_cast<const gologpp::Reference<gologpp::Action>*>(instruction)) {
        if (act->to_string("") == target_action->to_string("")) {
            gologpp::Action& target = *target_action->target();
            auto action_precondition = &target.precondition();

            bool bool_value;
            if (!(getBoolValue(action_precondition, bool_value) && !bool_value)) {
                out.emplace_back(action_precondition, factory.nil);
            }
        }

        return out;
    }
    
    // Block: sequence
    if (const auto* block = dynamic_cast<const gologpp::Block*>(instruction)) {
        if (block->elements().empty()) 
            return out;

        if (block->elements().size() == 1)
            return T(block->elements().front().get(), target_action, factory, cache);
        
        gologpp::Instruction* first = block->elements().front().get();
        auto* F_first = F(first, factory, cache);
        auto* not_F_first = factory.makeExpr<gologpp::Negation>(F_first);

        //std::vector<const gologpp::Instruction*> rest_elems(block->elements().begin() + 1, block->elements().end());

        std::vector<gologpp::Instruction*> rest_elems;
        rest_elems.reserve(block->elements().size() - 1);
        for (size_t i = 1; i < block->elements().size(); ++i) {
            rest_elems.push_back(const_cast<gologpp::Instruction*>(block->elements()[i].get()));
        }

        gologpp::Scope* parent_scope = const_cast<gologpp::Scope*>(&block->scope().parent_scope());
        gologpp::Instruction* rest_block = factory.makeInstr<gologpp::Block>(parent_scope, rest_elems);

        // Part 1: transitions from first, when not final
        for (auto [condition, subprogram] : T(first, target_action, factory, cache)) {

            gologpp::Expression* new_condition = createBooleanOperation(gologpp::BooleanOperator::AND, not_F_first, condition, factory);

            std::vector<gologpp::Instruction*> seq{subprogram, rest_block};
            gologpp::Instruction* new_subprogram = factory.makeInstr<gologpp::Block>(parent_scope, seq);

            bool bool_value;
            if (!(getBoolValue(new_condition, bool_value) && !bool_value)) {
                out.emplace_back(new_condition, new_subprogram);
            }
        }

        // Part 2: transitions from rest, when first is final
        for (auto [condition, subprogram] : T(rest_block, target_action, factory, cache)) {
            gologpp::Expression* new_condition = createBooleanOperation(gologpp::BooleanOperator::AND, F_first, condition, factory);

            bool bool_value;
            if (!(getBoolValue(new_condition, bool_value) && !bool_value)) {
                out.emplace_back(new_condition, subprogram);
            }
        }

        return out;
    }

    // Choose
    if (const auto* choose = dynamic_cast<const gologpp::Choose*>(instruction)) {
        for (const auto& alternative : choose->alternatives()) {
            auto branch_T = T(alternative.get(), target_action, factory, cache);
            out.insert(out.end(), branch_T.begin(), branch_T.end());
        }
        return out;
    }

    // Star
    if (const auto* star = dynamic_cast<const gologpp::Star*>(instruction)) {
        const gologpp::Instruction* body = &star->statement();

        for (auto [condition, subprogram] : T(body, target_action, factory, cache)) {
            gologpp::Expression* first_expr = factory.makeExpr<gologpp::Negation>(F(body, factory, cache));
            gologpp::Expression* new_condition = createBooleanOperation(gologpp::BooleanOperator::AND, first_expr, condition, factory);
            gologpp::Scope* parent_scope = const_cast<gologpp::Scope*>(&star->scope().parent_scope());
            
            std::vector<gologpp::Instruction*> seq;
            seq.push_back(subprogram);
            seq.push_back(const_cast<gologpp::Instruction*>(instruction));
            gologpp::Instruction* new_subprogram = factory.makeInstr<gologpp::Block>(parent_scope, seq);
            
            bool bool_value;
            if (!(getBoolValue(new_condition, bool_value) && !bool_value)) {
                out.emplace_back(new_condition, new_subprogram);
            }
        }

        return out;
    }

    // Pick (not supported for now)
    if (dynamic_cast<const gologpp::Pick*>(instruction)) {
        throw std::runtime_error("Pick is not supported in this implementation.");
    }

    throw std::runtime_error("T: Unsupported instruction type.");
}

Transitions T(const gologpp::Instruction* instruction, const gologpp::Reference<gologpp::Action>* target_action, NodeFactory& factory, SemCache& cache) {
    SemCache::Key key{instruction, target_action};
    auto it = cache.Tcache.find(key);
    if (it != cache.Tcache.end())
        return it->second;

    auto res = T_impl(instruction, target_action, factory, cache);
    cache.Tcache.emplace(key, res);
    return res;
}