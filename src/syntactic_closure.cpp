#include "syntactic_closure.h"

/** 
 * Computes the syntactic closure of a given program instruction.
 * The syntactic closure is the set of all subprograms that can be reached during execution.
 */
InstructionSet computeSyntacticClosure(const gologpp::Instruction* instruction) {
    // First handle complex constructs by transforming them into equivalent basic constructs
    if (const auto* conditional = dynamic_cast<const gologpp::Conditional<gologpp::Instruction>*>(instruction)) {
        // Transform conditionals if-then-else
        const gologpp::Instruction* simplified = simplifyConditional(conditional);
        return computeSyntacticClosure(simplified);
    }
    else if (const auto* while_loop = dynamic_cast<const gologpp::While*>(instruction)) {
        // Transform while loops
        const gologpp::Instruction* simplified = simplifyWhile(while_loop);
        return computeSyntacticClosure(simplified);
    }

    // Begin with the instruction itself and nil (i.e., True?)
    InstructionSet syntactic_closure = {instruction};
    
    const gologpp::Instruction* nil_instruction = new gologpp::Test(new gologpp::Value(gologpp::type<gologpp::BoolType>(), true));
    syntactic_closure.insert(nil_instruction);

    // Handle basic Golog constructs
    if (const auto* block = dynamic_cast<const gologpp::Block*>(instruction)) {
        // For a single-element block, return the syntactic closure of that element
        if (block->elements().size() == 1) {
            return computeSyntacticClosure(block->elements().front().get());
        }
        
        // Extract the first instruction from a sequence and store the rest as a new sequence
        const gologpp::Instruction* first_instruction = block->elements().front().get();
        std::vector<gologpp::Instruction*> rest_elements;

        for (size_t i = 1; i < block->elements().size(); ++i) {
            gologpp::Instruction* instr = const_cast<gologpp::Instruction*>(block->elements()[i].get());
            rest_elements.push_back(instr);
        }

        gologpp::Scope* parent_scope = const_cast<gologpp::Scope*>(&block->scope().parent_scope());
        const gologpp::Instruction* rest_block = new gologpp::Block(parent_scope, rest_elements);

        // Computes the subprogram of the first instruction and add all combinations of first_sub;rest_block to the syntactic closure
        InstructionSet sc_first = computeSyntacticClosure(first_instruction);
        for (const auto* first_sub : sc_first) {
            const std::vector<gologpp::Instruction*>& new_seq = {const_cast<gologpp::Instruction*>(first_sub), const_cast<gologpp::Instruction*>(rest_block)};
            gologpp::Scope* new_scope = const_cast<gologpp::Scope*>(&block->scope().parent_scope());
            gologpp::Block* new_block = new gologpp::Block(new_scope, new_seq);
            
            syntactic_closure.insert(new_block);
        }

        // Add the syntactic closure of the rest_block
        InstructionSet sc_rest = computeSyntacticClosure(rest_block);
        for (const auto* rest_sub : sc_rest) {
            syntactic_closure.insert(rest_sub);
        }

    }
    else if (const auto* choose = dynamic_cast<const gologpp::Choose*>(instruction)) {
        // For nondeterministic branch, include the closure of each alternative
        for (const auto& alternative : choose->alternatives()) {
            InstructionSet sc_alt = computeSyntacticClosure(alternative.get());
            
            for (const auto* alt_sub : sc_alt) {
                syntactic_closure.insert(alt_sub);
            }
        }
    }
    else if (const auto* star = dynamic_cast<const gologpp::Star*>(instruction)) {
        // For nondeterministic iteration (star), include body_sub;star for each subprogram in the body's syntactic closure
        const gologpp::Instruction* body = &star->statement();
        InstructionSet sc_body = computeSyntacticClosure(body);
        
        for (const auto* body_sub : sc_body) {
            std::vector<gologpp::Instruction*> new_seq = {const_cast<gologpp::Instruction*>(body_sub), const_cast<gologpp::Instruction*>(instruction)};
            gologpp::Scope* new_scope = const_cast<gologpp::Scope*>(&star->scope().parent_scope());
            gologpp::Block* new_block = new gologpp::Block(new_scope, new_seq);
            
            syntactic_closure.insert(new_block);
        }
    }
    else if (const auto* pick = dynamic_cast<const gologpp::Pick*>(instruction)) {       
        // For pick, include the syntactic closure of the program 
        const gologpp::Instruction* body = &pick->statement();
        InstructionSet sc_body = computeSyntacticClosure(body);

        for (const auto* body_sub : sc_body) {
            syntactic_closure.insert(body_sub);
        }
    }
    else if (const auto* test = dynamic_cast<const gologpp::Test*>(instruction)) {
        // Do nothing
    }
    else if (const auto* action_call = dynamic_cast<const gologpp::Reference<gologpp::Action>*>(instruction)) {
        // Do nothing
    }

    return syntactic_closure;
}

