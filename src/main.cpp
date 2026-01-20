#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <memory>
#include <cassert>

#include"synthesis/header/VarMgr.h"

#include "domain.h"
#include"propositional_logic.hpp"
#include"golog_program.hpp"
#include"golog_driver.hpp"
#include"scanner_internal.hpp"
#include"program_graph.h"
#include"compositional_golog_synthesizer.h"

extern "C" int yylex() { return 0; } ;

double sumVec(const std::vector<double>& vec) {
    double result;
    for (const auto& v: vec) result += v;
    return result;
}

int main(int argc, char ** argv) {

    // std::cout << "Parsing and transforming domain to DFA..." << std::flush;
    // std::shared_ptr<Syft::VarMgr> var_mgr = std::make_shared<Syft::VarMgr>(); 

    // std::string domain_file = "../examples/fond/domain.pddl";
    // std::string init_file = "../examples/fond/test1.pddl";
    // Domain domain(var_mgr, domain_file, init_file);

    // auto domain_dfa = domain.to_symbolic();
    // domain.print_domain();

    // var_mgr->print_mgr();

    // tests for propositional formulas
    // std::cout << "#########################################" << std::endl;
    // std::cout << "##### TESTS FOR PROPOSITIONAL LOGIC #####" << std::endl;
    // std::cout << "#########################################" << std::endl;

    // // true
    // std::cout << "########################" << std::endl;
    // std::cout << "##### TEST 1: true #####" << std::endl;
    // std::cout << "########################" << std::endl;
    // formula_ptr ptrue = std::make_shared<PropositionalLogicTrue>();

    // PropositionalLogicNodeToStringVisitor ptrue_visitor;
    // std::string ptrue_s = ptrue_visitor.apply(*ptrue);

    // PropositionalLogicNodeToBDDVisitor bdd_ptrue_visitor(var_mgr);
    // CUDD::BDD ptrue_bdd = bdd_ptrue_visitor.apply(*ptrue);

    // std::cout << "STRING: " << ptrue_s << std::endl;
    // std::cout << "BDD: " << ptrue_bdd << std::endl;

    // std::cout << "#########################" << std::endl;

    // // // false
    // std::cout << "#########################" << std::endl;
    // std::cout << "##### TEST 2: false #####" << std::endl;
    // std::cout << "#########################" << std::endl;
    // formula_ptr pfalse = std::make_shared<PropositionalLogicFalse>();

    // PropositionalLogicNodeToStringVisitor pfalse_visitor;
    // std::string pfalse_s = pfalse_visitor.apply(*pfalse);

    // PropositionalLogicNodeToBDDVisitor bdd_pfalse_visitor(var_mgr);
    // CUDD::BDD pfalse_bdd = bdd_pfalse_visitor.apply(*pfalse);

    // std::cout << "STRING: " << pfalse_s << std::endl;
    // std::cout << "BDD: " << pfalse_bdd << std::endl;

    // std::cout << "#########################" << std::endl;

    // // atoms
    // std::cout << "#########################" << std::endl;
    // std::cout << "##### TEST 3: atoms #####" << std::endl;
    // std::cout << "#########################" << std::endl;
    // formula_ptr atom = std::make_shared<PropositionalLogicAtom>("on_table_b1");

    // PropositionalLogicNodeToStringVisitor atom_visitor;
    // std::string atom_s = atom_visitor.apply(*atom);

    // PropositionalLogicNodeToBDDVisitor bdd_atom_visitor(var_mgr);
    // CUDD::BDD atom_bdd = bdd_atom_visitor.apply(*atom);

    // std::cout << "STRING: " << atom_s << std::endl;
    // std::cout << "BDD: " << atom_bdd << std::endl;

    // std::cout << "#########################" << std::endl;

    // // negation
    // std::cout << "########################" << std::endl;
    // std::cout << "### TEST 4: negation ###" << std::endl;
    // std::cout << "########################" << std::endl;

    // formula_ptr natom = std::make_shared<PropositionalLogicNegation>(atom);

    // PropositionalLogicNodeToStringVisitor natom_visitor;
    // std::string natom_s = natom_visitor.apply(*natom);

    // PropositionalLogicNodeToBDDVisitor bdd_natom_visitor(var_mgr);
    // CUDD::BDD natom_bdd = bdd_natom_visitor.apply(*natom);

    // std::cout << "STRING: " << natom_s << std::endl;
    // std::cout << "BDD: " << natom_bdd << std::endl;
    // std::cout << "########################" << std::endl;

    // // // conjunction
    // std::cout << "##############################" << std::endl;
    // std::cout << "### TEST 5: conjunction #####" << std::endl;
    // std::cout << "##############################" << std::endl;

    // formula_ptr atom2 = std::make_shared<PropositionalLogicAtom>("on_table_b2");
    // formula_set conj_args = {atom, atom2}; 
    // formula_ptr conj = std::make_shared<PropositionalLogicConjunction>(conj_args);

    // PropositionalLogicNodeToStringVisitor conj_visitor;
    // std::string conj_s = conj_visitor.apply(*conj);

    // PropositionalLogicNodeToBDDVisitor bdd_conj_visitor(var_mgr);
    // CUDD::BDD conj_bdd = bdd_conj_visitor.apply(*conj);

    // std::cout << "STRING: " << conj_s << std::endl;
    // std::cout << "BDD: " << conj_bdd << std::endl;
    // std::cout << "##############################" << std::endl;

    // // // disjunction
    // std::cout << "##############################" << std::endl;
    // std::cout << "### TEST 6: disjunction #####" << std::endl;
    // std::cout << "##############################" << std::endl;

    // formula_set disj_args = {atom, atom2}; 
    // formula_ptr disj = std::make_shared<PropositionalLogicConjunction>(disj_args);

    // PropositionalLogicNodeToStringVisitor disj_visitor;
    // std::string disj_s = disj_visitor.apply(*disj);

    // PropositionalLogicNodeToBDDVisitor bdd_disj_visitor(var_mgr);
    // CUDD::BDD disj_bdd = bdd_disj_visitor.apply(*disj);

    // std::cout << "STRING: " << disj_s << std::endl;
    // std::cout << "BDD: " << disj_bdd << std::endl;
    // std::cout << "##############################" << std::endl;

    // std::cout << "####################################" << std::endl;
    // std::cout << "#### TEST 7: compound formula 1 ####" << std::endl;
    // std::cout << "####################################" << std::endl;

    // formula_ptr a1 = std::make_shared<PropositionalLogicAtom>("on_table_b1");
    // formula_ptr a2 = std::make_shared<PropositionalLogicAtom>("on_table_b2");
    // formula_ptr a3 = std::make_shared<PropositionalLogicAtom>("on_b1_b2");
    // formula_ptr a4 = std::make_shared<PropositionalLogicAtom>("on_b2_b1");

    // formula_set conj1_args = {a1, a2};
    // formula_ptr conj1 = std::make_shared<PropositionalLogicConjunction>(conj1_args); 
    // formula_set disj1_args = {conj1, a3, a4};
    // formula_ptr disj1 = std::make_shared<PropositionalLogicDisjunction>(disj1_args);

    // PropositionalLogicNodeToStringVisitor visitor1;
    // std::cout << "STRING: " << visitor1.apply(*disj1) << std::endl;

    // PropositionalLogicNodeToBDDVisitor bdd_visitor1(var_mgr);
    // CUDD::BDD disj1_bdd = bdd_visitor1.apply(*disj1);
    // std::cout << "BDD: " << disj1_bdd << std::endl;

    // std::cout << "####################################" << std::endl;

    // std::cout << "####################################" << std::endl;
    // std::cout << "#### TEST 8: compound formula 2 ####" << std::endl;
    // std::cout << "####################################" << std::endl;

    // formula_ptr not_a3 = std::make_shared<PropositionalLogicNegation>(a3);

    // formula_set conj_args2 = {a1, a2};
    // formula_ptr conj2 = std::make_shared<PropositionalLogicConjunction>(conj_args2);
    
    // formula_set disj_args2 = {conj2, not_a3, a4};
    // formula_ptr disj2 = std::make_shared<PropositionalLogicDisjunction>(disj_args2);

    // PropositionalLogicNodeToStringVisitor visitor2;
    // std::cout << "STRING: " << visitor1.apply(*disj2) << std::endl;

    // PropositionalLogicNodeToBDDVisitor bdd_visitor2(var_mgr);
    // CUDD::BDD disj2_bdd = bdd_visitor2.apply(*disj2);
    // std::cout << "BDD: " << disj2_bdd << std::endl;
    // std::cout << "####################################" << std::endl;

    // // test for Golog programs
    // std::cout << "#########################################" << std::endl;
    // std::cout << "######## TEST FOR GOLOG PROGRAMS ########" << std::endl;
    // std::cout << "#########################################" << std::endl;

    // GologProgramNodeToString v;

    // std::cout << "TEST 1: nil" << std::endl;
    // golog_ptr nil = std::make_shared<const GologProgramNil>();
    // std::cout << "STRING: " << v.apply(*nil) << std::endl;
    // std::cout << "##########################" << std::endl;

    // std::cout << "TEST 2: und" << std::endl;
    // golog_ptr und = std::make_shared<const GologProgramUnd>();
    // std::cout << "STRING: " << v.apply(*und) << std::endl;
    // std::cout << "##########################" << std::endl;

    // std::cout << "TEST 3: action" << std::endl;
    // golog_ptr nop = std::make_shared<const GologProgramAction>("nop");
    // std::cout << "STRING: " << v.apply(*nop) << std::endl;
    // std::cout << "##########################" << std::endl;

    // std::cout << "TEST 4: test" << std::endl;
    // formula_ptr at1 = std::make_shared<const PropositionalLogicAtom>("on_table_b1");
    // formula_ptr at2 = std::make_shared<const PropositionalLogicAtom>("on_table_b2");
    // formula_set c = {at1, at2};
    // formula_ptr t = std::make_shared<const PropositionalLogicConjunction>(c);
    // golog_ptr test = std::make_shared<const GologProgramTest>(t);
    // std::cout << "STRING: " << v.apply(*test) << std::endl;
    // std::cout << "##########################" << std::endl;
    
    // std::cout << "TEST 5: sequence" << std::endl;
    // golog_vec vv = {nop, test};
    // golog_ptr seq = std::make_shared<const GologProgramSequence>(vv);
    // std::cout << "STRING: " << v.apply(*seq) << std::endl;
    // std::cout << "##########################" << std::endl;

    // std::cout << "TEST 6: choice" << std::endl;
    // golog_ptr nop2 = std::make_shared<GologProgramAction>("nop");
    // golog_set ss = {nop, nop2, test};
    // golog_ptr ch = std::make_shared<const GologProgramChoice>(ss);
    // std::cout << "STRING: " << v.apply(*ch) << std::endl;
    // std::cout << "##########################" << std::endl;

    // std::cout << "TEST 7: iteration" << std::endl;
    // golog_ptr it = std::make_shared<const GologProgramIteration>(nop);
    // std::cout << "STRING: " << v.apply(*it) << std::endl;
    // std::cout << "##########################" << std::endl;

    // // PARSER TESTS
    // std::cout << "###### PARSER TESTS ######" << std::endl;
    // GologProgramNodeToString v2;
    // auto driver = std::make_shared<GologDriver>(); 
 
    // std::string program = "pick_up_from_table_b1;(nil|put_down_b1);[on_table_b1]?";
    // std::stringstream program_stream(program);
    // driver->parse(program_stream);
    // golog_ptr parsed_program = driver->get_result();

    // std::cout << "STRING: " << to_string(parsed_program) << std::endl;
    // std::cout << "##########################" << std::endl;

    // equality tests
    // std::cout << "##### Equality tests #####" << std::endl;

    // std::cout << "##### TEST 1: True #####" << std::endl;
    // formula_ptr pl_true1 = std::make_shared<PropositionalLogicTrue>();
    // formula_ptr pl_true2 = std::make_shared<PropositionalLogicTrue>();

    // std::cout << to_string(pl_true1) << " and " << to_string(pl_true2) << " are equal? " << pl_true1 ->equals(pl_true2) << std::endl;
    // std::cout << "########################" << std::endl;

    // std::cout << "##### TEST 2: False #####" << std::endl;
    // formula_ptr pl_false1 = std::make_shared<PropositionalLogicFalse>();
    // formula_ptr pl_false2 = std::make_shared<PropositionalLogicFalse>();

    // std::cout << to_string(pl_false1) << " and " << to_string(pl_false2) << " are equal? " << pl_false1 ->equals(pl_false2) << std::endl;
    // std::cout << to_string(pl_true1) << " and " << to_string(pl_false2) << " are equal? " << pl_true1 ->equals(pl_false2) << std::endl;
    // std::cout << "########################" << std::endl;

    // std::cout << "##### TEST 3: Atoms #####" << std::endl;
    // formula_ptr atom1 = std::make_shared<PropositionalLogicAtom>("on_table_b1");
    // formula_ptr atom2 = std::make_shared<PropositionalLogicAtom>("on_table_b1");
    // formula_ptr atom3 = std::make_shared<PropositionalLogicAtom>("on_table_b2");

    // std::cout << to_string(atom1) << " and " << to_string(atom2) << " are equal? " << atom1->equals(atom2) << std::endl;
    // std::cout << to_string(atom1) << " and " << to_string(atom3) << " are equal? " << atom1->equals(atom3) << std::endl;
    // std::cout << "########################" << std::endl;

    // std::cout << "##### TEST 4: Negation #####" << std::endl;
    // formula_ptr natom1 = std::make_shared<PropositionalLogicNegation>(atom1);
    // formula_ptr natom2 = std::make_shared<PropositionalLogicNegation>(atom2);
    // formula_ptr natom3 = std::make_shared<PropositionalLogicNegation>(atom3);

    // std::cout << to_string(natom1) << " and " << to_string(natom2) << " are equal? " << natom1->equals(natom2) << std::endl;
    // std::cout << to_string(natom1) << " and " << to_string(natom3) << " are equal? " << natom1->equals(natom3) << std::endl;
    // std::cout << "########################" << std::endl;

    // std::cout << "##### TEST 5: Conjunction #####" << std::endl; 
    // formula_ptr atom4 = std::make_shared<PropositionalLogicAtom>("emptyhand");

    // formula_set set1 = {atom1, atom3};
    // formula_set set2 = {atom2, atom3};
    // formula_set set3 = {atom1, atom4};
    // formula_set set4 = {atom3, atom1};

    // formula_ptr conj1 = std::make_shared<PropositionalLogicConjunction>(set1);
    // formula_ptr conj2 = std::make_shared<PropositionalLogicConjunction>(set2);
    // formula_ptr conj3 = std::make_shared<PropositionalLogicConjunction>(set3);
    // formula_ptr conj4 = std::make_shared<PropositionalLogicConjunction>(set4);

    // std::cout << to_string(conj1) << " and " << to_string(conj2) << " are equal? " << conj1->equals(conj2) << std::endl;
    // std::cout << to_string(conj1) << " and " << to_string(conj3) << " are equal? " << conj1->equals(conj3) << std::endl;
    // std::cout << to_string(conj1) << " and " << to_string(conj4) << " are equal? " << conj1->equals(conj4) << std::endl;
    // std::cout << "########################" << std::endl;

    // std::cout << "##### TEST 5: Disjunction #####" << std::endl; 
    // formula_ptr disj1 = std::make_shared<PropositionalLogicDisjunction>(set1);
    // formula_ptr disj2 = std::make_shared<PropositionalLogicDisjunction>(set2);
    // formula_ptr disj3 = std::make_shared<PropositionalLogicDisjunction>(set3);

    // std::cout << to_string(disj1) << " and " << to_string(disj2) << " are equal? " << disj1->equals(disj2) << std::endl;
    // std::cout << to_string(disj1) << " and " << to_string(disj3) << " are equal? " << disj1->equals(disj3) << std::endl;
    // std::cout << "########################" << std::endl;

    // std::cout << "##### TEST 6: nil #####" << std::endl;
    // golog_ptr nil1 = std::make_shared<GologProgramNil>();
    // golog_ptr nil2 = std::make_shared<GologProgramNil>();

    // std::cout << to_string(nil1) << " and " << to_string(nil2) << " are equal? " << nil1->equals(nil2) << std::endl;
    // std::cout << "########################" << std::endl;

    // std::cout << "##### TEST 7: und #####" << std::endl;
    // golog_ptr und1 = std::make_shared<GologProgramUnd>();
    // golog_ptr und2 = std::make_shared<GologProgramUnd>();

    // std::cout << to_string(und1) << " and " << to_string(und2) << " are equal? " << und1->equals(und2) << std::endl;
    // std::cout << to_string(nil1) << " and " << to_string(und1) << " are equal? " << nil1->equals(und1) << std::endl;
    // std::cout << "########################" << std::endl;

    // std::cout << "##### TEST 8: action #####" << std::endl;
    // golog_ptr act1 = std::make_shared<GologProgramAction>("nop");
    // golog_ptr act2 = std::make_shared<GologProgramAction>("nop");
    // golog_ptr act3 = std::make_shared<GologProgramAction>("put_down_b1");

    // std::cout << to_string(act1) << " and " << to_string(act2) << " are equal? " << act1->equals(act2) << std::endl;
    // std::cout << to_string(act1) << " and " << to_string(act3) << " are equal? " << act1->equals(act3) << std::endl;
    // std::cout << "########################" << std::endl;

    // std::cout << "##### TEST 9: test #####" << std::endl;
    // golog_ptr t1 = std::make_shared<GologProgramTest>(conj1);
    // golog_ptr t2 = std::make_shared<GologProgramTest>(conj2);
    // golog_ptr t3 = std::make_shared<GologProgramTest>(conj3);
    // golog_ptr t4 = std::make_shared<GologProgramTest>(disj1);

    // std::cout << to_string(t1) << " and " << to_string(t2) << " are equal? " << t1->equals(t2) << std::endl;
    // std::cout << to_string(t1) << " and " << to_string(t3) << " are equal? " << t1->equals(t3) << std::endl;
    // std::cout << to_string(t3) << " and " << to_string(t4) << " are equal? " << t3->equals(t4) << std::endl;
    // std::cout << "########################" << std::endl; 

    // std::cout << "##### TEST 10: sequence #####" << std::endl;
    // golog_ptr act4 = std::make_shared<GologProgramAction>("put_down_b2");

    // golog_vec v1 = {act1, act3};
    // golog_vec v2 = {act1, act3};
    // golog_vec v3 = {act3, act1};
    // golog_vec v4 = {act1, act4};

    // golog_ptr seq1 = std::make_shared<GologProgramSequence>(v1);
    // golog_ptr seq2 = std::make_shared<GologProgramSequence>(v2);
    // golog_ptr seq3 = std::make_shared<GologProgramSequence>(v3);
    // golog_ptr seq4 = std::make_shared<GologProgramSequence>(v4);

    // std::cout << to_string(seq1) << " and " << to_string(seq2) << " are equal? " << seq1->equals(seq2) << std::endl;
    // std::cout << to_string(seq1) << " and " << to_string(seq3) << " are equal? " << seq1->equals(seq3) << std::endl;
    // std::cout << to_string(seq1) << " and " << to_string(seq4) << " are equal? " << seq1->equals(seq4) << std::endl;
    // std::cout << "########################" << std::endl;

    // std::cout << "##### TEST 11: choice #####" << std::endl;
    // golog_set s1 = {act1, act3};
    // golog_set s2 = {act3, act1};
    // golog_set s3 = {act1, act4}; 

    // golog_ptr ch1 = std::make_shared<GologProgramChoice>(s1);
    // golog_ptr ch2 = std::make_shared<GologProgramChoice>(s2);
    // golog_ptr ch3 = std::make_shared<GologProgramChoice>(s3);

    // std::cout << to_string(ch1) << " and " << to_string(ch2) << " are equal? " << ch1->equals(ch2) << std::endl;
    // std::cout << to_string(ch1) << " and " << to_string(ch3) << " are equal? " << ch1->equals(ch3) << std::endl;
    // std::cout << "########################" << std::endl;

    // std::cout << "##### TEST 12: iteration #####" << std::endl;
    // golog_ptr i1 = std::make_shared<GologProgramIteration>(ch1);
    // golog_ptr i2 = std::make_shared<GologProgramIteration>(ch2);
    // golog_ptr i3 = std::make_shared<GologProgramIteration>(ch3);

    // std::cout << to_string(i1) << " and " << to_string(i2) << " are equal? " << i1->equals(i2) << std::endl;
    // std::cout << to_string(i1) << " and " << to_string(i3) << " are equal? " << i1->equals(i3) << std::endl;
    // std::cout << "########################" << std::endl;

    // TFCVisitor tests
    // std::cout << "###### TFCVisitor tests #####" << std::endl;
    
    // auto driver = std::make_shared<GologDriver>(); 

    // std::string main_program = "nil";
    // std::string main_program = "put_down_b1";
    // std::string main_program = "[on_table_b1 && on_table_b2]?";
    // std::string main_program = "put_down_b1 | [on_table_b1 && on_table_b2]?";
    // std::string main_program = "(put_down_b1 | put_down_b2 | ([on_table_b1 && on_table_b2]?))";
    // std::string main_program = "(nop*)";
    // std::string main_program = "(put_down_b1 | put_down_b2)*";
    // std::string main_program = "(((nop)*)|(put_down_b1)*)";
    // std::string main_program = "(put_down_b1)*|[on_table_b1 && on_table_b2]?";
    // std::string main_program = "(((nop|put_down_b1)*))|[on_table_b1]?";
    // std::string main_program = "pick_up_from_table_b1;put_down_b1";
    // std::string main_program = "(pick_up_from_table_b1;put_down_b1)*";
    // std::string main_program = "nil;put_down_b1";
    // std::string main_program = "(nil);(nop*)";
    // std::string main_program = "(nil;(nop*))*";
    // std::string main_program =
    //     "(((pick_up_from_table_b1;(nil | put_down_b1)) | (pick_up_from_table_b2;(nil | put_down_b2)))*;[on_table_b1 && on_table_b2]?)";
    // std::string main_program =
    //     "(pick_up_from_table_b1;(nil | put_down_b1));[on_table_b1]?";


    // std::stringstream program_stream(main_program);
    // driver->parse(program_stream);
    // golog_ptr parsed_program = driver -> get_result();

    // // TFCVisitor tfc(var_mgr, domain.get_action_name_to_bdd(), domain.get_action_name_to_pre_bdd());

    // std::cout << "Testing program: " << to_string(parsed_program) << std::endl;

    // TFCResult tfc_result = get_tfc(parsed_program, var_mgr, domain.get_action_name_to_bdd(), domain.get_action_name_to_pre_bdd());

    // auto transitions = tfc_result.transitions_;
    // auto final_states = tfc_result.final_functions_;
    // auto continuation_functions = tfc_result.continuation_functions_;

    // std::cout << std::endl;
    // std::cout << "Transitions (T):" << std::endl;
    // for (const auto& [pa_pair, prog_transitions]: transitions) {
    //     std::cout << "T(" << to_string(pa_pair->program_) << ", " << pa_pair -> action_ << ") = {" << std::flush;
    //     for (const auto& t : prog_transitions) {
    //         std::cout  << "- (" << t->guard_ << ", " << to_string(t->successor_program_) << ") - " << std::flush;
    //     }
    //     std::cout << "}" << std::endl;
    // } 

    // std::cout << std::endl;
    // std::cout << "Final states (F):" << std::endl;
    // for (const auto& [subprog, f] : final_states) {
    //     std::cout << "F(" << to_string(subprog) << ") = " << f << std::endl;
    // }

    // std::cout << std::endl;
    // std::cout << "Continuation functions (C): " << std::endl;
    // for (const auto& [subprog, f_set] : continuation_functions) {
    //     std::cout << "C(" << to_string(subprog) << ") = {" << std::flush;
    //     for (const auto& f: f_set) {
    //         std::cout << " - " << f << " - " << std::flush; 
    //     }
    //     std::cout << "}" << std::endl;
    // }

    // std::cout << "##### TEST program graph construction #####" << std::endl;

    // std::string main_program =
    //     "(pick_up_from_table_b1;(nil | put_down_b1));[on_table_b1]?";
    // std::string main_program =
    //     "(((pick_up_from_table_b1;(nil | put_down_b1)) | (pick_up_from_table_b2;(nil | put_down_b2)))*;[on_table_b1 && on_table_b2]?)";
    // std::string main_program =
    //     "(pick_up_from_table_b1;nop)|(pick_up_from_table_b1;put_down_b1)"; // not SQSD
    // std::string main_program = "nil";

    // auto driver = std::make_shared<GologDriver>(); 

    // std::stringstream program_stream(main_program);
    // driver->parse(program_stream);
    // golog_ptr parsed_program = driver -> get_result();

    // std::cout << "Testing program: " << to_string(parsed_program) << std::endl;

    // // std::cout << "Transforming to program graph..." << std::flush;
    // auto pg = ExplicitStateProgramGraph::from_golog_program(
    //     parsed_program,
    //     var_mgr,
    //     domain.get_action_name_to_bdd(),
    //     domain.get_action_name_to_pre_bdd()
    // );
    // // std::cout << "DONE" << std::endl;

    // pg.dump_dot("pg.dot");

    // auto sdfa = pg.to_symbolic();

    // sdfa.dump_dot("pg_sdfa.dot");

    // var_mgr->print_mgr();

    std::cout << "##### Test Golog synthesis #####" << std::endl;

    std::cout << "Testing synthesis for golog programs" << std::endl;

    std::string domain_file = "../examples/fond/domain.pddl";
    std::string init_file = "../examples/fond/test1.pddl";
    std::string golog_file = "./../../examples/fond/golog1.golog";

    auto golog_synthesizer = CompositionalGologSynthesizer(domain_file, init_file, golog_file);
    auto result = golog_synthesizer.run();

    if (result->realizability) {
        std::cout << "[syft4golog] Synthesis is REALIZABLE [" << sumVec(golog_synthesizer.running_times()) << " s]" << std::endl;
        // golog_synthesizer.interactive();
    }
    else std::cout << "[syft4golog] synthesis is UNREALIZABLE [" << sumVec(golog_synthesizer.running_times()) << " s]" << std::endl;

    return 0;
}