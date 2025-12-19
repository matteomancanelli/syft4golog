#include "syntactic_closure.h"

static void insert_and_recurse(const gologpp::Instruction* instruction, InstructionSet& closure, NodeFactory& factory) {
    if (!instruction) return;

    if (closure.find(instruction) != closure.end())
        return;
    
    //std::cout << "Inserting instruction into syntactic closure: " << instruction->to_string("") << std::endl;
    closure.insert(instruction);

    std::vector<gologpp::Instruction*> nil_seq = {factory.nil, const_cast<gologpp::Instruction*>(instruction)};
    gologpp::Scope* nil_scope = const_cast<gologpp::Scope*>(&instruction->scope().parent_scope());
    gologpp::Instruction* nil_subprog = factory.makeInstr<gologpp::Block>(nil_scope, nil_seq);
    closure.insert(nil_subprog);
    
    if (const auto* conditional = dynamic_cast<const gologpp::Conditional<gologpp::Instruction>*>(instruction)) {
        const gologpp::Expression* condition = &conditional->condition();
        const gologpp::Instruction* then_branch = &conditional->block_true();
        const gologpp::Instruction* else_branch = &conditional->block_false();

        // Create test for the condition
        const gologpp::Instruction* test_condition = factory.makeInstr<gologpp::Test>(const_cast<gologpp::Expression*>(condition));

        // Create left branch: test(condition) followed by then-branch
        std::vector<gologpp::Instruction*> left_seq = {const_cast<gologpp::Instruction*>(test_condition), const_cast<gologpp::Instruction*>(then_branch)};
        gologpp::Scope* left_scope = const_cast<gologpp::Scope*>(&conditional->scope().parent_scope());
        gologpp::Block* left_branch = factory.makeInstr<gologpp::Block>(left_scope, left_seq);

        // Create negated condition
        gologpp::Expression* negated_condition = factory.makeExpr<gologpp::Negation>(const_cast<gologpp::Expression*>(condition));
        const gologpp::Instruction* test_negated = factory.makeInstr<gologpp::Test>(negated_condition);
        
        // Create right branch: test(¬condition) followed by else-branch
        std::vector<gologpp::Instruction*> right_seq = {const_cast<gologpp::Instruction*>(test_negated), const_cast<gologpp::Instruction*>(else_branch)};
        gologpp::Scope* right_scope = const_cast<gologpp::Scope*>(&conditional->scope().parent_scope());
        gologpp::Block* right_branch = factory.makeInstr<gologpp::Block>(right_scope, right_seq);

        // Create choose with both branches as alternatives
        std::vector<gologpp::Instruction*> alternatives = {left_branch, right_branch};
        gologpp::Scope* parent_scope = const_cast<gologpp::Scope*>(&conditional->scope().parent_scope());
        const gologpp::Choose* choose = factory.makeInstr<gologpp::Choose>(parent_scope, alternatives);

        insert_and_recurse(choose, closure, factory);
        return;
    }

    if (const auto* while_loop = dynamic_cast<const gologpp::While*>(instruction)) {
        // Get components
        const gologpp::Expression* condition = &while_loop->expression();
        const gologpp::Instruction* body = &while_loop->statement();

        // Create test for the loop condition
        const gologpp::Instruction* test_condition = factory.makeInstr<gologpp::Test>(const_cast<gologpp::Expression*>(condition));

        // Create the block to be iterated: test(condition) followed by body
        std::vector<gologpp::Instruction*> loop_seq = {const_cast<gologpp::Instruction*>(test_condition), const_cast<gologpp::Instruction*>(body)};
        gologpp::Scope* loop_scope = const_cast<gologpp::Scope*>(&while_loop->scope().parent_scope());
        gologpp::Block* loop_block = factory.makeInstr<gologpp::Block>(loop_scope, loop_seq);

        // Create star iteration
        gologpp::Instruction* star = factory.makeInstr<gologpp::Star>(loop_block);

        // Create test for negated condition (loop termination)
        gologpp::Expression* negated_condition = factory.makeExpr<gologpp::Negation>(const_cast<gologpp::Expression*>(condition));
        gologpp::Instruction* test_negated = factory.makeInstr<gologpp::Test>(negated_condition);

        // Create final block: star iteration followed by test(¬condition)
        std::vector<gologpp::Instruction*> seq = {star, test_negated};
        gologpp::Scope* parent_scope = const_cast<gologpp::Scope*>(&while_loop->scope().parent_scope());
        const gologpp::Block* block = factory.makeInstr<gologpp::Block>(parent_scope, seq);

        insert_and_recurse(block, closure, factory);
        return;
    }

    // Block
    if (const auto* block = dynamic_cast<const gologpp::Block*>(instruction)) {
        std::vector<gologpp::Instruction*> elements;

        for (size_t i = 0; i < block->elements().size(); ++i) {
            gologpp::Instruction* instr = const_cast<gologpp::Instruction*>(block->elements()[i].get());
            elements.push_back(instr);
        }

        for (const auto* elem : elements) 
            insert_and_recurse(elem, closure, factory);
        return;
    }

    // Choose
    if (const auto* choose = dynamic_cast<const gologpp::Choose*>(instruction)) {
        for (const auto& alternative : choose->alternatives()) 
            insert_and_recurse(alternative.get(), closure, factory);
        return;
    }

    // Star
    if (const auto* star = dynamic_cast<const gologpp::Star*>(instruction)) {
        const gologpp::Instruction* body = &star->statement();
        insert_and_recurse(body, closure, factory);
        return;
    }

    // Test / Action: done
    if (const auto* test = dynamic_cast<const gologpp::Test*>(instruction)) return;
    if (const auto* action = dynamic_cast<const gologpp::Reference<gologpp::Action>*>(instruction)) return;

    // Pick (skip for now)
    if (dynamic_cast<const gologpp::Pick*>(instruction)) {
        throw std::runtime_error("Pick is not supported in this implementation.");
    }

    // default
    throw std::runtime_error("Syntactic closure: Unsupported instruction type.");
}

void computeSyntacticClosure(const gologpp::Instruction* instruction, InstructionSet& syntactic_closure, NodeFactory& factory) {
    factory.initSingletons();
    syntactic_closure.insert(factory.nil);
    insert_and_recurse(instruction, syntactic_closure, factory);
}