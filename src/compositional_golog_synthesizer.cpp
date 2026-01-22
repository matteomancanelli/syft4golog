#include"compositional_golog_synthesizer.h"

std::shared_ptr<Syft::SynthesisResult> CompositionalGologSynthesizer::run() {

    Stopwatch domain2dfa;
    domain2dfa.start();

    // transform domain and Golog program into DFA
    std::cout << "[syft4golog] Transforming domain to symbolic..." << std::flush;

    domain_sdfa_ = std::make_shared<Syft::SymbolicStateDfa>(std::move(domain_->to_symbolic()));

    double t_domain2dfa = domain2dfa.stop().count() / 1000.0;
    running_times_.push_back(t_domain2dfa);

    std::cout << "DONE [" << t_domain2dfa << " s]" << std::endl;

    Stopwatch golog2pg;
    golog2pg.start();

    std::cout << "[syft4golog] Transforming Golog to symbolic program graph..." << std::flush;
    pg_ = std::make_shared<ExplicitStateProgramGraph>(ExplicitStateProgramGraph::from_golog_program(golog_program_, var_mgr_, domain_->get_action_name_to_bdd(), domain_->get_action_name_to_pre_bdd()));

    auto golog_dfa = pg_->to_symbolic();
    // debug
    // golog_program_graph.dump_dot("pg.dot");
    // golog_dfa.dump_dot("pg_sdfa.dot");
    double t_golog2pg = golog2pg.stop().count() / 1000.0;
    running_times_.push_back(t_golog2pg);

    std::cout << "DONE [" << t_golog2pg << " s]" << std::endl;
    
    // construct game arena via composition
    Stopwatch game_construction;
    game_construction.start();

    std::cout << "[syft4golog] Constructing game arena..." << std::flush;
    game_arena_ = std::make_shared<Syft::SymbolicStateDfa>(std::move(compose(*domain_sdfa_, golog_dfa)));

    double t_game_construction = game_construction.stop().count() / 1000.0;
    running_times_.push_back(t_game_construction);

    std::cout << "DONE [" << t_golog2pg << " s]" << std::endl;

    // debug
    // var_mgr_->print_mgr();

    // solve game and return result
    Stopwatch game_resolution;
    game_resolution.start();

    std::cout << "[syft4golog] Reducing synthesis to reachability..." << std::flush;
    auto synthesizer = Syft::Reachability(
        *game_arena_,
        Syft::Player::Agent, // starting player
        Syft::Player::Agent, // protagonist player
        game_arena_->final_states(),
        var_mgr_->cudd_mgr()->bddOne() // state space = true
    );
    result_ = std::make_shared<Syft::SynthesisResult>(std::move(synthesizer.run()));

    double t_game_resolution = game_resolution.stop().count() / 1000.0;
    running_times_.push_back(t_game_resolution);

    std::cout << "DONE [" << t_game_resolution << " s]" << std::endl;

    return result_;
}

Syft::SymbolicStateDfa CompositionalGologSynthesizer::compose(
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

void CompositionalGologSynthesizer::interactive() const {
    // keep in mind order of variables
    // (F, Act, React, Z)
    std::vector<int> state = game_arena_->initial_state();
    std::vector<CUDD::BDD> transition_function = game_arena_->transition_function();
    CUDD::BDD final_states = game_arena_->final_states();
    int number_of_fluents = domain_->get_vars().size() + 2;
    bool is_reaction_valid = false;

    // debug
    // var_mgr_->print_mgr();
    // domain_->print_domain();

    std::cout << "##################################" << std::endl;
    std::cout << "[INTERACTIVE] Interactive strategy debugging mode" << std::endl;

    // Obtain the indexes for the agent error state and the environment error state
    std::vector<std::string> vars_ = domain_->get_vars();
    std::size_t agent_error_index = vars_.size();
    std::size_t env_error_index = vars_.size() + 1;

    // Obtain the agent error bdd and the environment error bdd from the transition function
    CUDD::BDD agent_error_bdd = game_arena_->transition_function()[agent_error_index];
    CUDD::BDD env_error_bdd = game_arena_->transition_function()[env_error_index];

    CUDD::BDD winning_states =  result_->winning_states;
    std::unordered_map<int, CUDD::BDD> output_function = result_->transducer.get()->get_output_function();
    std::unordered_map<int, std::string> id_to_var = var_mgr_->get_index_to_name_map(); 
    std::map<int, std::string> action_id_to_action_name = domain_->get_id_to_action_name();

    while (true) {
        std::vector<int> transition;
        std::vector<int> Z_fluents;
        std::vector<int> actions_bit;
        std::vector<int> reactions_bit;
        std::vector<int> goal_ltlf;
		std::vector<int> valid_actions;
		std::vector<int> valid_reactions;
		std::vector<int> legal_reactions;
        std::vector<int> state_eval;
        is_reaction_valid = false;

        // std::cout << "[INTERACTIVE] State vector: ";
        // for (const auto& v : state) std::cout << v;
        // std::cout << std::endl;
        std::string string_state = "{";
        for (int i = 0; i < vars_.size(); ++i)
            if (state[i] == 1) string_state += vars_[i] + ", ";
        string_state = string_state.substr(0, string_state.size() - 2) + "}";
        std::cout << "[INTERACTIVE] State vars: " << string_state << std::endl;
        // std::cout << "[INTERACTIVE] Final states: " << final_states << std::endl;

        // construct input to strategy
        state_eval.insert(state_eval.end(), state.begin(), state.begin() + number_of_fluents);
        for(int i = 0; i < var_mgr_->output_variable_count(); ++i)
          state_eval.push_back(1);
        for(int i = 0; i < var_mgr_->input_variable_count(); ++i)
          state_eval.push_back(1);
        state_eval.insert(state_eval.end(), state.begin() + number_of_fluents, state.end());

        std::cout << "[INTERACTIVE] The current state is: ";
        if (var_mgr_->state_variable(game_arena_->automaton_id(), vars_.size()).Eval(state_eval.data()).IsOne()){ //agent_error_bdd.Eval(state_eval.data()).IsOne()){//
            std::cout << "- AGENT ERROR STATE -";
            std::cout << "Termination" << std::endl;
            return;
        }
        else if (var_mgr_->state_variable(game_arena_->automaton_id(), vars_.size()+1).Eval(state_eval.data()).IsOne()){ //env_error_bdd.Eval(state_eval.data()).IsOne()){//
            std::cout << "- ENVIRONMENT ERROR STATE -";
            std::cout << "Termination" << std::endl;
            return;
        } 
        else if (final_states.Eval(state_eval.data()).IsOne()){
            std::cout << "- FINAL -"<< std::endl;
            std::cout << "[INTERACTIVE] The goal has been reached. Termination" << std::endl;
            return;
        } 
        else std::cout << "- NOT FINAL -";
        std::cout << std::endl;

        Z_fluents.insert(Z_fluents.end(), state.begin(), state.begin() + number_of_fluents);
        goal_ltlf.insert(goal_ltlf.end(), state.begin() + number_of_fluents, state.end());
        transition.insert(transition.end(), Z_fluents.begin(), Z_fluents.end());

        // action selection
        for (int i = 0; i < id_to_var.size(); ++i) {
            std::string var = id_to_var[i];
            int agent_eval;
            if (var_mgr_->is_output_variable(var)) {
                std::cout << "Variable: " << var << std::flush;
                std::cout << ". Agent output (0 = false, 1 = true): " << std::flush;
                agent_eval = output_function[i].Eval(state_eval.data()).IsOne();
                std::cout << agent_eval << std::endl;
                actions_bit.push_back(agent_eval);
            }
        }

        // retrieve action name
        int selected_act_id = -1;
        for (const auto& act_id : action_id_to_action_name) {
            std::vector<int> iter_action;
            iter_action = domain_->to_bits(act_id.first, var_mgr_->output_variable_count());
            if(std::equal(iter_action.begin(), iter_action.end(), actions_bit.begin())) {
                std::cout << "[INTERACTIVE] ID: " << act_id.first << " - Action: " << act_id.second << std::endl;
                selected_act_id = act_id.first;
                for (auto b : actions_bit) transition.push_back(b);
                break;
            } else continue;          
        }

        // reaction selection
        auto id_to_reaction_name = domain_->get_id_to_reaction_name();
        int react_id;
        while(!is_reaction_valid) {
            valid_reactions.clear();
            std::cout << "[INTERACTIVE] Valid reactions:" << std::endl;
          	for(const auto& id_react : id_to_reaction_name) {
          		std::vector<int> check_reaction;
          		check_reaction.insert(check_reaction.end(), Z_fluents.begin(), Z_fluents.end());
                check_reaction.insert(check_reaction.end(), actions_bit.begin(), actions_bit.end());
          		for (const auto& b : domain_->to_bits(id_react.first, var_mgr_->input_variable_count())) 
                    check_reaction.push_back(b);
                check_reaction.insert(check_reaction.end(), goal_ltlf.begin(), goal_ltlf.end());
              	if(!(env_error_bdd.Eval(check_reaction.data()).IsOne())) {
                    valid_reactions.push_back(id_react.first);
              		std::cout << "ID: " << id_react.first << " - Reaction: " << id_react.second << std::endl;
              	}
            }
            std::cout << "[INTERACTIVE] Insert ID of environment action: ";
            std::cin >> react_id;
            if(std::count(valid_reactions.begin(), valid_reactions.end(), react_id) > 0) {
            	is_reaction_valid = true;
            	for (const auto& b : domain_->to_bits(react_id, var_mgr_->input_variable_count())) 
                    transition.push_back(b);
            } else if (react_id == -1){
                std::cout << "[INTERACTIVE] STOPPING INTERACTIVE STRATEGY." << std::endl;
                return;
            } else {
              	std::cout << "[INTERACTIVE] Chosen Rection is not valid. Choose again." << std::endl;
            }
        }

        // state transition
        transition.insert(transition.end(), goal_ltlf.begin(), goal_ltlf.end());
        std::vector<int> new_state;
        for (int i = 0; i < transition_function.size(); ++i) {
            new_state.push_back(transition_function[i].Eval(transition.data()).IsOne());
        }
        state = new_state;
        std::cout << std::endl;
    }
    return;
}