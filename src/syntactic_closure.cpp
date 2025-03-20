#include "syntactic_closure.h"

std::set<const gologpp::Instruction*> getSyntacticClosure(const gologpp::Instruction* instruction) {
    if (const auto* conditional = dynamic_cast<const gologpp::Conditional<gologpp::Instruction>*>(instruction)) {
        const gologpp::Instruction* simplified = simplifyConditional(conditional);
        return getSyntacticClosure(simplified);
    }
    else if (const auto* while_loop = dynamic_cast<const gologpp::While*>(instruction)) {
        const gologpp::Instruction* simplified = simplifyWhile(while_loop);
        return getSyntacticClosure(simplified);
    }

    const gologpp::Instruction* nil_instruction = new gologpp::Test(new gologpp::Value(gologpp::type<gologpp::BoolType>(), true));
    std::set<const gologpp::Instruction*> syntactic_closure = {instruction, nil_instruction};    

    // Check the actual type of the instruction
    if (const auto* block = dynamic_cast<const gologpp::Block*>(instruction)) {
        if (block->elements().size() == 1) {
            return getSyntacticClosure(block->elements().front().get());
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

            std::set<const gologpp::Instruction*> sc_first = getSyntacticClosure(first_instruction);
            for (const auto* first_sub : sc_first) {
                const std::vector<gologpp::Instruction*>& new_seq = {const_cast<gologpp::Instruction*>(first_sub), const_cast<gologpp::Instruction*>(rest_block)};
                gologpp::Scope* new_scope = const_cast<gologpp::Scope*>(&block->scope().parent_scope());
                gologpp::Block* new_block = new gologpp::Block(new_scope, new_seq);
                
                syntactic_closure.insert(new_block);
            }

            std::set<const gologpp::Instruction*> sc_rest = getSyntacticClosure(rest_block);
            syntactic_closure.insert(sc_rest.begin(), sc_rest.end());
        }
    }
    else if (const auto* choose = dynamic_cast<const gologpp::Choose*>(instruction)) {
        for (const auto& alternative : choose->alternatives()) {
            std::set<const gologpp::Instruction*> sc_alt = getSyntacticClosure(alternative.get());
            syntactic_closure.insert(sc_alt.begin(), sc_alt.end());
        }
    }
    else if (const auto* star = dynamic_cast<const gologpp::Star*>(instruction)) {        
        const gologpp::Instruction* body = &star->statement();
        std::set<const gologpp::Instruction*> sc_body = getSyntacticClosure(body);
        
        for (const auto* body_sub : sc_body) {
            std::vector<gologpp::Instruction*> new_seq = {const_cast<gologpp::Instruction*>(body_sub), const_cast<gologpp::Instruction*>(instruction)};
            gologpp::Scope* new_scope = const_cast<gologpp::Scope*>(&star->scope().parent_scope());
            gologpp::Block* new_block = new gologpp::Block(new_scope, new_seq);
            
            syntactic_closure.insert(new_block);
        }
    }
    else if (const auto* pick = dynamic_cast<const gologpp::Pick*>(instruction)) {        
        const gologpp::Instruction* body = &pick->statement();
        std::set<const gologpp::Instruction*> sc_body = getSyntacticClosure(body);
        syntactic_closure.insert(sc_body.begin(), sc_body.end());
    }
    else if (const auto* test = dynamic_cast<const gologpp::Test*>(instruction)) {
        // Do nothing
    }
    else if (const auto* action_call = dynamic_cast<const gologpp::Reference<gologpp::Action>*>(instruction)) {
        // Do nothing
    }

    return syntactic_closure;
}

