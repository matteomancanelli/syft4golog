#include "golog_program.h"
#include "trans.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <istream>
#include <set>
#include <CLI/CLI.hpp>
#include "VarMgr.h"
#include "Domain.h"
#include "Stopwatch.h"

#include "lydia/logic/to_ldlf.hpp"
#include "lydia/dfa/mona_dfa.hpp"
#include "lydia/parser/ldlf/driver.hpp"
#include "lydia/parser/ltlf/driver.hpp"
#include "lydia/to_dfa/core.hpp"
#include "lydia/to_dfa/strategies/compositional/base.hpp"
#include "lydia/utils/print.hpp"
#include "synthesis/syn.h"

enum class Logic : int { ldlf, ltlf };

// Function to initialize the symbol table with predefined symbols
void initialize_symbols() {
    // Adding actions
    create_symbol(ACTION, "A");
    create_symbol(ACTION, "B");
    create_symbol(FLUENT, "C");
}

bool compare_symbols(const Symbol* sym1, const Symbol* sym2) {
    if (sym1 == NULL && sym2 == NULL) return true;
    if (sym1 == NULL || sym2 == NULL) return false;

    return (sym1->sym_type == sym2->sym_type) && (strcmp(sym1->name, sym2->name) == 0);
}

bool compare_programs(const golog_program* p1, const golog_program* p2) {
    if (p1 == NULL && p2 == NULL) return true;
    if (p1 == NULL || p2 == NULL) return false;

    if (p1->constr_type != p2->constr_type) return false;
    if (!compare_symbols(p1->symbol, p2->symbol)) return false;

    return compare_programs(p1->left, p2->left) && compare_programs(p1->right, p2->right);
}

struct golog_program_comparator {
    bool operator() (const golog_program* p1, const golog_program* p2) const {
        return !compare_programs(p1, p2);
    }
};

std::set<golog_program*, golog_program_comparator> getSyntacticClosure(golog_program* root) {
    std::set<golog_program*, golog_program_comparator> syntactic_closure;
    
    Symbol* true_sym = (Symbol*) malloc(sizeof(Symbol));
    if (true_sym) {
        true_sym->name = strdup("True");
        true_sym->sym_type = FLUENT;
    }
    golog_program* nil = create_program(TEST, add_symbol(true_sym), NULL);
    
    syntactic_closure.insert(root);
    syntactic_closure.insert(nil);

    std::set<golog_program*, golog_program_comparator> sc_left_program;
    std::set<golog_program*, golog_program_comparator> sc_right_program;

    switch(root->constr_type) {
        case SEQUENCE:
            sc_left_program = getSyntacticClosure(root->left);
            sc_right_program = getSyntacticClosure(root->right);

            for(golog_program* prog : sc_left_program) {
                golog_program* seq_prog = create_program(SEQUENCE, prog, root->right);
                syntactic_closure.insert(seq_prog);
            }
            
            for(golog_program* prog : sc_right_program)
                syntactic_closure.insert(prog);
            break;
        case BRANCH:
            sc_left_program = getSyntacticClosure(root->left);
            sc_right_program = getSyntacticClosure(root->right);

            for(golog_program* prog : sc_left_program)
                syntactic_closure.insert(prog);
            for(golog_program* prog : sc_right_program)
                syntactic_closure.insert(prog);
            break;
        case ITER:
            golog_program* iter_prog = create_program(SEQUENCE, root->left, root);
            syntactic_closure.insert(iter_prog);
            break;
    }

    return syntactic_closure;
}

int main(int argc, char** argv) {
    const char* input_program = "A;B";

    // Initialize symbol table
    initialize_symbols();

    golog_program* program = getAST(input_program);
    std::cout << "Parsed Golog program: ";
    print_program(program);
    std::cout << std::endl;

    std::cout << "Syntactic Closure: " << std::endl;
    std::set<golog_program*, golog_program_comparator> syntactic_closure = getSyntacticClosure(program);

    for (golog_program* prog : syntactic_closure) {
        print_program(prog);
        std::cout << std::endl;
    }
    

    Logic logic = Logic::ldlf;

    std::string formula = "<true*;(!request + true*;response)>end";
    
    std::string part_file;
    bool starting_player_env = false;
    
    auto dfa_strategy = whitemech::lydia::CompositionalStrategy();
    auto translator = whitemech::lydia::Translator(dfa_strategy);

    std::shared_ptr<whitemech::lydia::AbstractDriver> driver;
    if (logic == Logic::ldlf) {
        driver = std::make_shared<whitemech::lydia::parsers::ldlf::Driver>();
    } else if (logic == Logic::ltlf) {
        driver = std::make_shared<whitemech::lydia::parsers::ltlf::LTLfDriver>();
    }

    std::stringstream formula_stream(formula);
    driver->parse(formula_stream);

    auto parsed_formula = driver->get_result();
    whitemech::lydia::ldlf_ptr ldlf_parsed_formula;
    if (logic == Logic::ltlf) {
        auto ltl_formula = std::static_pointer_cast<const whitemech::lydia::LTLfFormula>(parsed_formula);
        ldlf_parsed_formula = whitemech::lydia::to_ldlf(*ltl_formula);
    }
    else {
        auto ldl_formula = std::static_pointer_cast<const whitemech::lydia::LDLfFormula>(parsed_formula);
        ldlf_parsed_formula = ldl_formula;
    }
    
    std::cout << ldlf_parsed_formula << std::endl;

    
    bool no_empty = false;
    if (no_empty) {
        auto context = driver->context;
        auto end = context->makeLdlfEnd();
        auto not_end = context->makeLdlfNot(end);
        ldlf_parsed_formula = context->makeLdlfAnd({ldlf_parsed_formula, not_end});
    }

    auto t_start = std::chrono::high_resolution_clock::now();

    std::cout << "Transforming to DFA..." << std::endl;
    auto t_dfa_start = std::chrono::high_resolution_clock::now();
    auto my_dfa = translator.to_dfa(*ldlf_parsed_formula);
  
    auto my_mona_dfa = std::dynamic_pointer_cast<whitemech::lydia::mona_dfa>(my_dfa);
    auto t_dfa_end = std::chrono::high_resolution_clock::now();
    std::cout << "Transforming to DFA... done!" << std::endl;
    
    double elapsed_time_dfa = std::chrono::duration<double, std::milli>(t_dfa_end - t_dfa_start).count();
    std::cout << "Time elapsed for DFA construction: " << elapsed_time_dfa << "ms" << std::endl;
    
    bool summary = true;
    bool print_dfa = true;

    if (summary) {
        std::cout << "Number of states: " << std::to_string(my_mona_dfa->get_nb_states()) << std::endl;
    }

    if (print_dfa) {
        std::cout << "Computed automaton: " << std::endl;
        whitemech::lydia::dfaPrint(my_mona_dfa->get_dfa(), my_mona_dfa->get_nb_variables(), my_mona_dfa->names, my_mona_dfa->indices.data());
    }
    
    std::string graphviz_path = "./test";
    if (true) {
        std::cout << "Printing the automaton..." << std::endl;
        if (my_mona_dfa) {
            whitemech::lydia::print_mona_dfa(my_mona_dfa->get_dfa(), graphviz_path, my_mona_dfa->get_nb_variables());
        } else {
            std::cout << "Failed." << std::endl;
        }
    }

    std::cout << "THAT'S ALL FOLKS!" << std::endl;
    
    return 0;
}
