#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <memory>

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
    std::cout << "#########################################" << std::endl;
    std::cout << "##### TESTS FOR PROPOSITIONAL LOGIC #####" << std::endl;
    std::cout << "#########################################" << std::endl;

    // true
    std::cout << "########################" << std::endl;
    std::cout << "##### TEST 1: true #####" << std::endl;
    std::cout << "########################" << std::endl;
    formula_ptr ptrue = std::make_shared<PropositionalLogicTrue>();

    PropositionalLogicNodeToStringVisitor ptrue_visitor;
    std::string ptrue_s = ptrue_visitor.apply(*ptrue);

    PropositionalLogicNodeToBDDVisitor bdd_ptrue_visitor(var_mgr);
    CUDD::BDD ptrue_bdd = bdd_ptrue_visitor.apply(*ptrue);

    std::cout << "STRING: " << ptrue_s << std::endl;
    std::cout << "BDD: " << ptrue_bdd << std::endl;

    std::cout << "#########################" << std::endl;

    // // false
    std::cout << "#########################" << std::endl;
    std::cout << "##### TEST 2: false #####" << std::endl;
    std::cout << "#########################" << std::endl;
    formula_ptr pfalse = std::make_shared<PropositionalLogicFalse>();

    PropositionalLogicNodeToStringVisitor pfalse_visitor;
    std::string pfalse_s = pfalse_visitor.apply(*pfalse);

    PropositionalLogicNodeToBDDVisitor bdd_pfalse_visitor(var_mgr);
    CUDD::BDD pfalse_bdd = bdd_pfalse_visitor.apply(*pfalse);

    std::cout << "STRING: " << pfalse_s << std::endl;
    std::cout << "BDD: " << pfalse_bdd << std::endl;

    std::cout << "#########################" << std::endl;

    // atoms
    std::cout << "#########################" << std::endl;
    std::cout << "##### TEST 3: atoms #####" << std::endl;
    std::cout << "#########################" << std::endl;
    formula_ptr atom = std::make_shared<PropositionalLogicAtom>("on_table_b1");

    PropositionalLogicNodeToStringVisitor atom_visitor;
    std::string atom_s = atom_visitor.apply(*atom);

    PropositionalLogicNodeToBDDVisitor bdd_atom_visitor(var_mgr);
    CUDD::BDD atom_bdd = bdd_atom_visitor.apply(*atom);

    std::cout << "STRING: " << atom_s << std::endl;
    std::cout << "BDD: " << atom_bdd << std::endl;

    std::cout << "#########################" << std::endl;

    // negation
    std::cout << "########################" << std::endl;
    std::cout << "### TEST 4: negation ###" << std::endl;
    std::cout << "########################" << std::endl;

    formula_ptr natom = std::make_shared<PropositionalLogicNegation>(atom);

    PropositionalLogicNodeToStringVisitor natom_visitor;
    std::string natom_s = natom_visitor.apply(*natom);

    PropositionalLogicNodeToBDDVisitor bdd_natom_visitor(var_mgr);
    CUDD::BDD natom_bdd = bdd_natom_visitor.apply(*natom);

    std::cout << "STRING: " << natom_s << std::endl;
    std::cout << "BDD: " << natom_bdd << std::endl;
    std::cout << "########################" << std::endl;

    // // conjunction
    std::cout << "##############################" << std::endl;
    std::cout << "### TEST 5: conjunction #####" << std::endl;
    std::cout << "##############################" << std::endl;

    formula_ptr atom2 = std::make_shared<PropositionalLogicAtom>("on_table_b2");
    formula_set conj_args = {atom, atom2}; 
    formula_ptr conj = std::make_shared<PropositionalLogicConjunction>(conj_args);

    PropositionalLogicNodeToStringVisitor conj_visitor;
    std::string conj_s = conj_visitor.apply(*conj);

    PropositionalLogicNodeToBDDVisitor bdd_conj_visitor(var_mgr);
    CUDD::BDD conj_bdd = bdd_conj_visitor.apply(*conj);

    std::cout << "STRING: " << conj_s << std::endl;
    std::cout << "BDD: " << conj_bdd << std::endl;
    std::cout << "##############################" << std::endl;

    // // disjunction
    std::cout << "##############################" << std::endl;
    std::cout << "### TEST 6: disjunction #####" << std::endl;
    std::cout << "##############################" << std::endl;

    formula_set disj_args = {atom, atom2}; 
    formula_ptr disj = std::make_shared<PropositionalLogicConjunction>(disj_args);

    PropositionalLogicNodeToStringVisitor disj_visitor;
    std::string disj_s = disj_visitor.apply(*disj);

    PropositionalLogicNodeToBDDVisitor bdd_disj_visitor(var_mgr);
    CUDD::BDD disj_bdd = bdd_disj_visitor.apply(*disj);

    std::cout << "STRING: " << disj_s << std::endl;
    std::cout << "BDD: " << disj_bdd << std::endl;
    std::cout << "##############################" << std::endl;

    std::cout << "####################################" << std::endl;
    std::cout << "#### TEST 7: compound formula 1 ####" << std::endl;
    std::cout << "####################################" << std::endl;

    formula_ptr a1 = std::make_shared<PropositionalLogicAtom>("on_table_b1");
    formula_ptr a2 = std::make_shared<PropositionalLogicAtom>("on_table_b2");
    formula_ptr a3 = std::make_shared<PropositionalLogicAtom>("on_b1_b2");
    formula_ptr a4 = std::make_shared<PropositionalLogicAtom>("on_b2_b1");

    formula_set conj1_args = {a1, a2};
    formula_ptr conj1 = std::make_shared<PropositionalLogicConjunction>(conj1_args); 
    formula_set disj1_args = {conj1, a3, a4};
    formula_ptr disj1 = std::make_shared<PropositionalLogicDisjunction>(disj1_args);

    PropositionalLogicNodeToStringVisitor visitor1;
    std::cout << "STRING: " << visitor1.apply(*disj1) << std::endl;

    PropositionalLogicNodeToBDDVisitor bdd_visitor1(var_mgr);
    CUDD::BDD disj1_bdd = bdd_visitor1.apply(*disj1);
    std::cout << "BDD: " << disj1_bdd << std::endl;

    std::cout << "####################################" << std::endl;

    std::cout << "####################################" << std::endl;
    std::cout << "#### TEST 8: compound formula 2 ####" << std::endl;
    std::cout << "####################################" << std::endl;

    formula_ptr not_a3 = std::make_shared<PropositionalLogicNegation>(a3);

    formula_set conj_args2 = {a1, a2};
    formula_ptr conj2 = std::make_shared<PropositionalLogicConjunction>(conj_args2);
    
    formula_set disj_args2 = {conj2, not_a3, a4};
    formula_ptr disj2 = std::make_shared<PropositionalLogicDisjunction>(disj_args2);

    PropositionalLogicNodeToStringVisitor visitor2;
    std::cout << "STRING: " << visitor1.apply(*disj2) << std::endl;

    PropositionalLogicNodeToBDDVisitor bdd_visitor2(var_mgr);
    CUDD::BDD disj2_bdd = bdd_visitor2.apply(*disj2);
    std::cout << "BDD: " << disj2_bdd << std::endl;
    std::cout << "####################################" << std::endl;

    // test for Golog programs
    std::cout << "#########################################" << std::endl;
    std::cout << "######## TEST FOR GOLOG PROGRAMS ########" << std::endl;
    std::cout << "#########################################" << std::endl;

    GologProgramNodeToString v;

    std::cout << "TEST 1: nil" << std::endl;
    golog_ptr nil = std::make_shared<const GologProgramNil>();
    std::cout << "STRING: " << v.apply(*nil) << std::endl;
    std::cout << "##########################" << std::endl;

    std::cout << "TEST 2: und" << std::endl;
    golog_ptr und = std::make_shared<const GologProgramUnd>();
    std::cout << "STRING: " << v.apply(*und) << std::endl;
    std::cout << "##########################" << std::endl;

    std::cout << "TEST 3: action" << std::endl;
    golog_ptr nop = std::make_shared<const GologProgramAction>("nop");
    std::cout << "STRING: " << v.apply(*nop) << std::endl;
    std::cout << "##########################" << std::endl;

    std::cout << "TEST 4: test" << std::endl;
    formula_ptr at1 = std::make_shared<const PropositionalLogicAtom>("on_table_b1");
    formula_ptr at2 = std::make_shared<const PropositionalLogicAtom>("on_table_b2");
    formula_set c = {at1, at2};
    formula_ptr t = std::make_shared<const PropositionalLogicConjunction>(c);
    golog_ptr test = std::make_shared<const GologProgramTest>(t);
    std::cout << "STRING: " << v.apply(*test) << std::endl;
    std::cout << "##########################" << std::endl;
    
    std::cout << "TEST 5: sequence" << std::endl;
    golog_vec vv = {nop, test};
    golog_ptr seq = std::make_shared<const GologProgramSequence>(vv);
    std::cout << "STRING: " << v.apply(*seq) << std::endl;
    std::cout << "##########################" << std::endl;

    std::cout << "TEST 6: choice" << std::endl;
    golog_ptr nop2 = std::make_shared<GologProgramAction>("nop");
    golog_set ss = {nop, nop2, test};
    golog_ptr ch = std::make_shared<const GologProgramChoice>(ss);
    std::cout << "STRING: " << v.apply(*ch) << std::endl;
    std::cout << "##########################" << std::endl;

    std::cout << "TEST 7: iteration" << std::endl;
    golog_ptr it = std::make_shared<const GologProgramIteration>(nop);
    std::cout << "STRING: " << v.apply(*it) << std::endl;
    std::cout << "##########################" << std::endl;

    // PARSER TESTS
    std::cout << "###### PARSER TESTS ######" << std::endl;
    GologProgramNodeToString v2;
    auto driver = std::make_shared<GologDriver>(); 
 
    std::string program = "pick_up_from_table_b1;(nil|put_down_b1);[on_table_b1]?";
    std::stringstream program_stream(program);
    driver->parse(program_stream);
    golog_ptr parsed_program = driver->get_result();

    std::cout << "STRING: " << v2.apply(*parsed_program) << std::endl;
    std::cout << "##########################" << std::endl;

    return 0;
}