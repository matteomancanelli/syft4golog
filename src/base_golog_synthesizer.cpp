#include"base_golog_synthesizer.h"

BaseGologSynthesizer::BaseGologSynthesizer(
    const std::string& domain_file,
    const std::string& init_file,
    const std::string& golog_file
) {
    var_mgr_ = std::make_shared<Syft::VarMgr>();

    domain_ = std::make_shared<Domain>(var_mgr_, domain_file, init_file);

    gologpp::parser::parse_file(golog_file);
    auto mainproc = gologpp::global_scope().lookup_global<gologpp::Procedure>("main");
    golog_program_ = std::make_shared<gologpp::Procedure>(mainproc);
}

Syft::SymbolicStateDfa BaseGologSynthesizer::compose(
    const Syft::SymbolicStateDfa& domain_dfa,
    const Syft::SymbolicStateDfa& golog_dfa
) const {
  // ASSUMPTION. The Order of the variables is:
  // (F, Act, React, Z_{δ}), where δ is the Golog program
  // i.e.: the DFA of the domain is created before DFA of the Golog program
  std::size_t domain_dfa_id = domain_dfa.automaton_id();
  std::size_t golog_dfa_id = golog_dfa.automaton_id();
  std::vector<std::size_t> automaton_ids = {domain_dfa_id, golog_dfa_id};
  std::vector<int> domain_dfa_initial_state = domain_dfa.initial_state();
  std::vector<int> golog_dfa_initial_state = golog_dfa.initial_state();
  std::vector<CUDD::BDD> domain_dfa_transition_function = domain_dfa.transition_function();
  std::vector<CUDD::BDD> golog_dfa_transition_function = golog_dfa.transition_function();
  
  std::vector<int> initial_state, initial_eval_vector;
  CUDD::BDD final_states = var_mgr_->cudd_mgr()->bddOne();
  std::vector<CUDD::BDD> transition_function;

  // get ID of composed DFA
  std::size_t composed_automaton_id = var_mgr_->create_product_state_space(automaton_ids);

  // 1. initial state
  // a. create initial evaluation vector. Vars: F
  initial_eval_vector.insert(initial_eval_vector.end(), domain_dfa_initial_state.begin(), domain_dfa_initial_state.end());
  // Vars: Act. Can be anything; set to 11...1.
  for (int i = 0; i < var_mgr_->output_variable_count(); ++i)
    initial_eval_vector.push_back(1);
  // Vars React. Can be anything; set to 11...1
  for (int i = 0; i < var_mgr_->input_variable_count(); ++i)
    initial_eval_vector.push_back(1);
  // Vars Z_{δ}
  initial_eval_vector.insert(initial_eval_vector.end(), golog_dfa_initial_state.begin(), golog_dfa_initial_state.end());
  
  // b. construct initial state through initial evaluation
  initial_state.insert(initial_state.end(), domain_dfa_initial_state.begin(), domain_dfa_initial_state.end());
  for (const auto& bdd : golog_dfa_transition_function)
    initial_state.push_back(bdd.Eval(initial_eval_vector.data()).IsOne());

  // 2. creates substitution vector
  std::vector<CUDD::BDD> substitution_vector = var_mgr_->make_compose_vector(domain_dfa.automaton_id(), domain_dfa.transition_function());

  // 3. creates transition function 
  // a. domain BDDs
  for (const auto& bdd : domain_dfa_transition_function) 
    transition_function.push_back(bdd);
  // b. Golog program (composed) BDDs
  for (const auto& bdd : golog_dfa_transition_function) 
    transition_function.push_back(bdd.VectorCompose(substitution_vector));

  // 4. final states
  final_states = final_states * golog_dfa.final_states();

  std::size_t agent_error_index = var_mgr_->get_state_variables(domain_dfa.automaton_id()).size() - 2;
  std::size_t env_error_index = var_mgr_->get_state_variables(domain_dfa.automaton_id()).size() - 1;

  CUDD::BDD agent_error_bdd = var_mgr_->get_state_variables(domain_dfa.automaton_id()).at(agent_error_index);
  CUDD::BDD env_error_bdd = var_mgr_->get_state_variables(domain_dfa.automaton_id()).at(env_error_index);

  final_states = (!agent_error_bdd) * (env_error_bdd + final_states);

  // 5. construct symbolic DFA
  Syft::SymbolicStateDfa composed_dfa(
    var_mgr_,
    composed_automaton_id,
    initial_state,
    transition_function,
    final_states);
  
  return composed_dfa;
}