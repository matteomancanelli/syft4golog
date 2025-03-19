#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <memory>

#include "parser/parser.h"

#include "model/formula.h"
#include "model/reference.h"
#include "model/procedural.h"
#include "model/types.h"
#include "model/value.h"

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


int main() {
    try {
        //std::string filename = "../examples/robot_program.gpp";
        //std::string filename = "../examples/minimal_example.gpp";
        std::string filename = "../examples/star_example.gpp";

        std::cout << "Parsing program from file..." << std::endl;
        gologpp::parser::parse_file(filename);
        std::cout << "Parsed program successfully!" << std::endl;
        
        // Get the main procedure
        auto mainproc = gologpp::global_scope().lookup_global<gologpp::Procedure>("main");
        if (!mainproc) {
            std::cerr << "No procedure main() found in program!" << std::endl;
            return 1;
        }
        
        const gologpp::Instruction& definition = mainproc->definition();
        std::set<const gologpp::Instruction*> syntactic_closure = getSyntacticClosure(&definition);

        std::cout << "Syntactic closure of the main procedure:" << std::endl;
        std::cout << std::endl;

        int i = 0;
        for (const auto* instruction : syntactic_closure) {
            std::cout << "Subprogram " << i << std::endl;
            std::cout << instruction->to_string("") << std::endl;
            std::cout << "----------" << std::endl;
            i++;
        }

        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}