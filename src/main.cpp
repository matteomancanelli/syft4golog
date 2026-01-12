#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <memory>

#include "parser/parser.h"

#include "model/action.h"
#include "model/formula.h"
#include "model/reference.h"
#include "model/procedural.h"
#include "model/types.h"
#include "model/value.h"
#include "model/effect_axiom.h"

#include"synthesis/header/VarMgr.h"
#include "domain.h"

#include"propositional_logic.hpp"
#include"golog_program.hpp"
#include"golog_driver.hpp"
#include"scanner_internal.hpp"

extern "C" int yylex() { return 0; } ;

int main(int argc, char ** argv) {

    std::cout << "Parsing and transforming domain to DFA..." << std::flush;
    std::shared_ptr<Syft::VarMgr> var_mgr = std::make_shared<Syft::VarMgr>(); 

    std::string domain_file = "../examples/fond/domain.pddl";
    std::string init_file = "../examples/fond/test1.pddl";
    Domain domain(var_mgr, domain_file, init_file);

    auto domain_dfa = domain.to_symbolic();
    domain.print_domain();

    var_mgr->print_mgr();

    // tests for propositional formulas
    // std::cout << "#########################################" << std::endl;
    // std::cout << "##### TESTS FOR PROPOSITIONAL LOGIC #####" << std::endl;
    // std::cout << "#########################################" << std::endl;

    // // true
    // std::cout << "########################" << std::endl;
    // std::cout << "##### TEST 1: true #####" << std::endl;
    // std::cout << "########################" << std::endl;
    // PropositionalLogicTrue ptrue;

    // PropositionalLogicNodeToStringVisitor ptrue_visitor;
    // std::string ptrue_s = ptrue_visitor.apply(ptrue);

    // PropositionalLogicNodeToBDDVisitor bdd_ptrue_visitor(var_mgr);
    // CUDD::BDD ptrue_bdd = bdd_ptrue_visitor.apply(ptrue);

    // std::cout << "STRING: " << ptrue_s << std::endl;
    // std::cout << "BDD: " << ptrue_bdd << std::endl;

    // std::cout << "#########################" << std::endl;

    // // false
    // std::cout << "#########################" << std::endl;
    // std::cout << "##### TEST 2: false #####" << std::endl;
    // std::cout << "#########################" << std::endl;
    // PropositionalLogicFalse pfalse;

    // PropositionalLogicNodeToStringVisitor pfalse_visitor;
    // std::string pfalse_s = pfalse_visitor.apply(pfalse);

    // PropositionalLogicNodeToBDDVisitor bdd_pfalse_visitor(var_mgr);
    // CUDD::BDD pfalse_bdd = bdd_pfalse_visitor.apply(pfalse);

    // std::cout << "STRING: " << pfalse_s << std::endl;
    // std::cout << "BDD: " << pfalse_bdd << std::endl;

    // std::cout << "#########################" << std::endl;

    // // atoms
    // std::cout << "#########################" << std::endl;
    // std::cout << "##### TEST 3: atoms #####" << std::endl;
    // std::cout << "#########################" << std::endl;
    // PropositionalLogicAtom atom("on_table_b1");

    // PropositionalLogicNodeToStringVisitor atom_visitor;
    // std::string atom_s = atom_visitor.apply(atom);

    // PropositionalLogicNodeToBDDVisitor bdd_atom_visitor(var_mgr);
    // CUDD::BDD atom_bdd = bdd_atom_visitor.apply(atom);

    // std::cout << "STRING: " << atom_s << std::endl;
    // std::cout << "BDD: " << atom_bdd << std::endl;

    // std::cout << "#########################" << std::endl;

    // // negation
    // std::cout << "########################" << std::endl;
    // std::cout << "### TEST 4: negation ###" << std::endl;
    // std::cout << "########################" << std::endl;

    // PropositionalLogicNegation natom(&atom);

    // PropositionalLogicNodeToStringVisitor natom_visitor;
    // std::string natom_s = natom_visitor.apply(natom);

    // PropositionalLogicNodeToBDDVisitor bdd_natom_visitor(var_mgr);
    // CUDD::BDD natom_bdd = bdd_natom_visitor.apply(natom);

    // std::cout << "STRING: " << natom_s << std::endl;
    // std::cout << "BDD: " << natom_bdd << std::endl;
    // std::cout << "########################" << std::endl;

    // // conjunction
    // std::cout << "##############################" << std::endl;
    // std::cout << "### TEST 5: conjunction #####" << std::endl;
    // std::cout << "##############################" << std::endl;

    // PropositionalLogicAtom atom2("on_table_b2");
    // std::unordered_set<PropositionalLogicNode*, PropositionalLogicHash, PropositionalLogicEquals> conj_args = { &atom, &atom2 };
    // PropositionalLogicConjunction conj(&conj_args);

    // PropositionalLogicNodeToStringVisitor conj_visitor;
    // std::string conj_s = conj_visitor.apply(conj);

    // PropositionalLogicNodeToBDDVisitor bdd_conj_visitor(var_mgr);
    // CUDD::BDD conj_bdd = bdd_conj_visitor.apply(conj);

    // std::cout << "STRING: " << conj_s << std::endl;
    // std::cout << "BDD: " << conj_bdd << std::endl;
    // std::cout << "##############################" << std::endl;

    // // disjunction
    // std::cout << "##############################" << std::endl;
    // std::cout << "### TEST 6: disjunction #####" << std::endl;
    // std::cout << "##############################" << std::endl;

    // std::unordered_set<PropositionalLogicNode*, PropositionalLogicHash, PropositionalLogicEquals> disj_args = { &atom, &atom2 };
    // PropositionalLogicDisjunction disj(&disj_args);

    // PropositionalLogicNodeToStringVisitor disj_visitor;
    // std::string disj_s = disj_visitor.apply(disj);

    // PropositionalLogicNodeToBDDVisitor bdd_disj_visitor(var_mgr);
    // CUDD::BDD disj_bdd = bdd_disj_visitor.apply(disj);

    // std::cout << "STRING: " << disj_s << std::endl;
    // std::cout << "BDD: " << disj_bdd << std::endl;
    // std::cout << "##############################" << std::endl;

    // std::cout << "####################################" << std::endl;
    // std::cout << "#### TEST 7: compound formula 1 ####" << std::endl;
    // std::cout << "####################################" << std::endl;

    // PropositionalLogicAtom a1("on_table_b1"), a2("on_table_b2"), a3("on_b2_b1"), a4("on_b1_b2");

    // std::unordered_set<PropositionalLogicNode*, PropositionalLogicHash, PropositionalLogicEquals> conj_args1 = { &a1, &a2 };
    // PropositionalLogicConjunction conj1(&conj_args1);
    // std::unordered_set<PropositionalLogicNode*, PropositionalLogicHash, PropositionalLogicEquals> disj_args1 = { &conj1, &a3, &a4 };
    // PropositionalLogicDisjunction disj1(&disj_args1);

    // PropositionalLogicNodeToStringVisitor visitor1;
    // std::cout << "STRING: " << visitor1.apply(disj1) << std::endl;

    // PropositionalLogicNodeToBDDVisitor bdd_visitor1(var_mgr);
    // CUDD::BDD disj1_bdd = bdd_visitor1.apply(disj1);
    // std::cout << "BDD: " << disj1_bdd << std::endl;

    // std::cout << "####################################" << std::endl;

    // std::cout << "####################################" << std::endl;
    // std::cout << "#### TEST 8: compound formula 2 ####" << std::endl;
    // std::cout << "####################################" << std::endl;

    // PropositionalLogicNegation not_a3(&a3);

    // std::unordered_set<PropositionalLogicNode*, PropositionalLogicHash, PropositionalLogicEquals> conj_args2 = { &a1, &a2 };
    // PropositionalLogicConjunction conj2(&conj_args2);

    // std::unordered_set<PropositionalLogicNode*, PropositionalLogicHash, PropositionalLogicEquals> disj_args2 = { &conj2, &not_a3, &a4 };
    // PropositionalLogicDisjunction disj2(&disj_args2);

    // PropositionalLogicNodeToStringVisitor visitor2;
    // std::cout << "STRING: " << visitor1.apply(disj2) << std::endl;

    // PropositionalLogicNodeToBDDVisitor bdd_visitor2(var_mgr);
    // CUDD::BDD disj2_bdd = bdd_visitor2.apply(disj2);
    // std::cout << "BDD: " << disj2_bdd << std::endl;
    // std::cout << "####################################" << std::endl;

    // test for Golog programs
    // std::cout << "#########################################" << std::endl;
    // std::cout << "######## TEST FOR GOLOG PROGRAMS ########" << std::endl;
    // std::cout << "#########################################" << std::endl;

    // GologProgramNodeToString v;

    // std::cout << "TEST 1: nil" << std::endl;
    // GologProgramNil nil;
    // std::cout << "STRING: " << v.apply(nil) << std::endl;
    // std::cout << "##########################" << std::endl;

    // std::cout << "TEST 2: und" << std::endl;
    // GologProgramUnd und;
    // std::cout << "STRING: " << v.apply(und) << std::endl;
    // std::cout << "##########################" << std::endl;

    // std::cout << "TEST 3: action" << std::endl;
    // GologProgramAction act("nop");
    // std::cout << "STRING: " << v.apply(act) << std::endl;
    // std::cout << "##########################" << std::endl;

    // std::cout << "TEST 4: test" << std::endl;
    // PropositionalLogicAtom a1("on_table_b1"), a2("on_table_b2");
    // std::unordered_set<PropositionalLogicNode*, PropositionalLogicHash, PropositionalLogicEquals> c = { &a1, &a2 };
    // PropositionalLogicConjunction t(&c);
    // GologProgramTest test(&t);
    // std::cout << "STRING: " << v.apply(test) << std::endl;
    // std::cout << "##########################" << std::endl;
    
    // std::cout << "TEST 5: sequence" << std::endl;
    // std::vector<GologProgramNode*> vv = {&act, &test};
    // GologProgramSequence seq(&vv);
    // std::cout << "STRING: " << v.apply(seq) << std::endl;
    // std::cout << "##########################" << std::endl;

    // std::cout << "TEST 6: choice" << std::endl;
    // std::unordered_set<GologProgramNode*, GologProgramHash, GologProgramEquals> ss = {&act, &test};
    // GologProgramChoice ch(&ss);
    // std::cout << "STRING: " << v.apply(ch) << std::endl;
    // std::cout << "##########################" << std::endl;

    // std::cout << "TEST 7: iteration" << std::endl;
    // GologProgramIteration it(&act);
    // std::cout << "STRING: " << v.apply(it) << std::endl;
    // std::cout << "##########################" << std::endl;

    // PARSER TESTS
    GologProgramNodeToString v;
    auto driver = std::make_shared<GologDriver>(); 
 
    // std::string program = "nop;[on_table_b1 && on_table_b2]?";
    std::string program = "pick_up_from_table_b1;(nop|put_down_b1);[on_table_b1]?";
    std::stringstream program_stream(program);
    driver->parse(program_stream);
    auto parsed_program = driver->get_result();

    std::cout << "STRING: " << v.apply(*parsed_program) << std::endl;

    // OLD TESTS
    // try {
    //     std::string filename = "../examples/star_example.gpp";

    //     std::cout << "Parsing program from file..." << std::endl;
    //     gologpp::parser::parse_file(filename);
    //     std::cout << "Parsed program successfully!" << std::endl;
        

    //     // Get the main procedure in the program
    //     auto mainproc = gologpp::global_scope().lookup_global<gologpp::Procedure>("main");
    //     if (!mainproc) {
    //         std::cerr << "No procedure main() found in program!" << std::endl;
    //         return 1;
    //     }
        
    //     NodeFactory factory;
    //     factory.initSingletons();
    //     SemCache cache;

    //     std::cerr << "Computing the syntactic closure..." << std::endl;
    //     // Compute the syntactic closure
    //     const gologpp::Instruction& definition = mainproc->definition();
    //     InstructionSet syntactic_closure;
    //     computeSyntacticClosure(&definition, syntactic_closure, factory);
    //     std::cerr << "Syntactic closure computed!" << std::endl;

    //     //std::vector<const gologpp::Instruction*> nodes(syntactic_closure.begin(), syntactic_closure.end());
    //     std::vector<const gologpp::Instruction*> nodes;
    //     nodes.reserve(syntactic_closure.size());
    //     for (const auto* instruction : syntactic_closure) {
    //         nodes.push_back(instruction);
    //     }

    //     std::unordered_map<const gologpp::Instruction*, int> id;
    //     id.reserve(nodes.size());
    //     for (int i = 0; i < (int) nodes.size(); ++i) 
    //         id[nodes[i]] = i;

    //     // Choose a target action for demo: "A"
    //     //auto actions = gpp.actions(); // if your API differs, adjust
    //     //gologpp::Action* A = findActionByName(actions, "A");
    //     //if (!A) {
    //     //    std::cout << "Action A not found; transitions demo will be empty.\n";
    //     //}

    //     // Create or find action 'A' for testing transition relations
    //     std::cout << "Creating action A..." << std::endl;
    //     const gologpp::Reference<gologpp::Action>* action_ref = nullptr;
        
    //     auto existing_action = gologpp::global_scope().lookup_global<gologpp::Action>("A");
    //     if (existing_action) {
    //         std::cout << "Found existing action A in global scope" << std::endl;
    //         action_ref = existing_action->make_ref(std::vector<gologpp::Expression*>{});
    //     }
    //     else {
    //         std::cout << "No existing action found, creating a new one" << std::endl;
            
    //         auto action = std::make_shared<gologpp::Action>(
    //             &gologpp::global_scope(),
    //             gologpp::type<gologpp::VoidType>(),
    //             "A",
    //             std::vector<std::shared_ptr<gologpp::Variable>>{}
    //         );
            
    //         std::cout << "Action created successfully: " << action->to_string("") << std::endl;
    //         action_ref = action->make_ref(std::vector<gologpp::Expression*>{});
    //     }
        
    //     // Iterate through each instruction in the syntactic closure
    //     int i = 0;
    //     std::cout << "Iterating through syntactic closure with " << syntactic_closure.size() << " instructions." << std::endl;

    //     for (const auto* instruction : syntactic_closure) {
    //         // Print the instruction
    //         std::cout << "Subprogram " << i << std::endl;
    //         std::cout << instruction->to_string("") << std::endl;

    //         // Compute and print its final condition
    //         std::cout << "Final " << i << std::endl;
    //         std::cout << F(instruction, factory, cache)->to_string("") << std::endl;
    //         std::cout << std::endl;

    //         // Compute and print all possible transitions
    //         std::cout << "Trans " << i << std::endl;
    //         auto transitions = T(instruction, action_ref, factory, cache);
    //         for (const auto& transition : transitions) {
    //             std::cout << "\tTransition: " << std::endl;
    //             std::cout << "\t\tCondition: " << transition.first->to_string("") << std::endl;
    //             std::cout << "\t\tSubprogram: " << transition.second->to_string("") << std::endl;
    //         }

    //         std::cout << "----------" << std::endl;
    //         i++;
    //     }
        
    //     return 0;
        
    // } catch (const std::exception& e) {
    //     std::cerr << "Error: " << e.what() << std::endl;
    //     return 1;
    // }
}