#include"base_golog_synthesizer.h"

BaseGologSynthesizer::BaseGologSynthesizer(
    const std::string& domain_file,
    const std::string& init_file,
    const std::string& golog_file
) {
    std::cout << "[syft4golog] Parsing domain...";
    var_mgr_ = std::make_shared<Syft::VarMgr>();

    domain_ = std::make_shared<Domain>(var_mgr_, domain_file, init_file);
    std::cout << "DONE" << std::endl;

    std::cout << "[syft4golog] Parsing Golog program..." << std::flush;

    std::ifstream in(golog_file);
    std::string program_str = "";
    std::getline(in, program_str);
 
    auto driver = std::make_shared<GologDriver>(); 

    std::stringstream program_stream(program_str);
    driver->parse(program_stream);
    golog_program_ = driver -> get_result();

    std::cout << "DONE" << std::endl;
}

Syft::SymbolicStateDfa BaseGologSynthesizer::compose(
    const Syft::SymbolicStateDfa& domain_dfa,
    const Syft::SymbolicStateDfa& golog_dfa
) const {
  // var manager
  std::shared_ptr<Syft::VarMgr> var_mgr = domain_dfa.var_mgr();

  std::vector<std::size_t> automaton_ids = {domain_dfa.automaton_id(), golog_dfa.automaton_id()};

  // initial state
  std::vector<int> initial_state;
  std::vector<int> domain_initial_state = domain_dfa.initial_state();
  std::vector<int> golog_initial_state = golog_dfa.initial_state();
  initial_state.insert(initial_state.end(), domain_initial_state.begin(), domain_initial_state.end());
  initial_state.insert(initial_state.end(), golog_initial_state.begin(), golog_initial_state.end());

  // transition function
  std::vector<CUDD::BDD> transition_function;
  std::vector<CUDD::BDD> domain_transition_function = domain_dfa.transition_function();
  std::vector<CUDD::BDD> golog_transition_function = golog_dfa.transition_function();
  transition_function.insert(transition_function.end(), domain_transition_function.begin(), domain_transition_function.end());
  transition_function.insert(transition_function.end(), golog_transition_function.begin(), golog_transition_function.end());

  // final states 
  std::size_t agent_error_index = var_mgr_->state_variables(domain_dfa.automaton_id()).size() - 2;
  std::size_t env_error_index = var_mgr_->state_variables(domain_dfa.automaton_id()).size() - 1;

  CUDD::BDD agent_error_bdd = var_mgr_->state_variables(domain_dfa.automaton_id()).at(agent_error_index);
  CUDD::BDD env_error_bdd = var_mgr_->state_variables(domain_dfa.automaton_id()).at(env_error_index);

  CUDD::BDD final_states = (!agent_error_bdd) * (env_error_bdd + golog_dfa.final_states());

  // automaton id
  std::size_t composed_automaton_id = var_mgr->create_product_state_space(automaton_ids);

  // return
  Syft::SymbolicStateDfa composed_dfa(
    var_mgr_,
    composed_automaton_id,
    initial_state,
    transition_function,
    final_states);
  return composed_dfa;
}