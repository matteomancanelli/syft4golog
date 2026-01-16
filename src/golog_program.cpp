#include"golog_program.hpp"

// nil
std::size_t GologProgramNil::hash() const {return type_id_;}

std::size_t GologProgramNil::get_type_id() const {return type_id_;}

bool GologProgramNil::equals(const golog_ptr& x) const {
    return type_id_ == x->get_type_id();
}

void GologProgramNil::accept(GologProgramNodeVisitor& v) const {v.visit(*this);}

// undefined
std::size_t GologProgramUnd::hash() const {return type_id_;}

std::size_t GologProgramUnd::get_type_id() const {return type_id_;}

bool GologProgramUnd::equals(const golog_ptr& x) const {
    return type_id_ == x->get_type_id();
}

void GologProgramUnd::accept(GologProgramNodeVisitor& v) const {v.visit(*this);}

// action
std::size_t GologProgramAction::get_type_id() const {return type_id_;}

std::size_t GologProgramAction::hash() const {
    return type_id_ * std::hash<std::string>{}(action_name_);
}

bool GologProgramAction::equals(const golog_ptr& x) const {
    return type_id_ == x->get_type_id() && (action_name_ == dynamic_cast<const GologProgramAction&>(*x).action_name_);
}

void GologProgramAction::accept(GologProgramNodeVisitor& v) const {v.visit(*this);}

// test
std::size_t GologProgramTest::get_type_id() const {return type_id_;}

std::size_t GologProgramTest::hash() const {
    return type_id_ * arg_->hash();
}

bool GologProgramTest::equals(const golog_ptr& x) const {
    return type_id_ == x->get_type_id() && (arg_->equals(dynamic_cast<const GologProgramTest&>(*x).arg_));
}

void GologProgramTest::accept(GologProgramNodeVisitor& v) const {v.visit(*this);}

// sequence
std::size_t GologProgramSequence::hash() const {
    std::size_t hash = type_id_;
    for (const auto& a : args_) hash *= a->hash();
    return hash;
}

std::size_t GologProgramSequence::get_type_id() const {return type_id_;}

bool GologProgramSequence::equals(const golog_ptr& x) const {
    if (type_id_ == x->get_type_id()) {
        auto d = dynamic_cast<const GologProgramSequence&>(*x);
        if (args_.size() != d.args_.size()) return false;
        auto d_args = d.args_;
        auto a = args_.begin();
        auto b = d_args.begin();
        for (; a != args_.end(); ++a, ++b) {
            if (!(*a)->equals(*b)) return false;
        }
        return true;
    } else {
        return false;
    }
}

void GologProgramSequence::accept(GologProgramNodeVisitor& v) const {v.visit(*this);}

// choice
std::size_t GologProgramChoice::get_type_id() const {return type_id_;}

std::size_t GologProgramChoice::hash() const {
    std::size_t hash = type_id_;
    for (const auto& a : args_)
        hash *= a->hash();
    return hash;
}

bool GologProgramChoice::equals(const golog_ptr& x) const {
    if (type_id_ != x->get_type_id()) return false;
    const auto& d = dynamic_cast<const GologProgramChoice&>(*x);
    if (args_.size() != d.args_.size()) return false;
    for (const auto& a : args_) {
        bool found = false;
        for (const auto& b : d.args_) {
            if (a->equals(b)) { found = true; break; }
        }
        if (!found) return false;
    }
    return true;
}

void GologProgramChoice::accept(GologProgramNodeVisitor& v) const {v.visit(*this);}

// iteration
std::size_t GologProgramIteration::get_type_id() const {
    return type_id_;
}

std::size_t GologProgramIteration::hash() const {
    return type_id_ * arg_->hash();
}

bool GologProgramIteration::equals(const golog_ptr& x) const {
    return type_id_ == x->get_type_id() && 
        (arg_->equals(dynamic_cast<const GologProgramIteration&>(*x).arg_));
}

void GologProgramIteration::accept(GologProgramNodeVisitor& v) const {v.visit(*this);}

// visitors
// to string visitor
void GologProgramNodeToString::visit(const GologProgramNil& x) {
    result_ = "nil";
}

void GologProgramNodeToString::visit(const GologProgramUnd& x) {
    result_ = "und";
}

void GologProgramNodeToString::visit(const GologProgramAction& x) {
    result_ = x.action_name_;
}

void GologProgramNodeToString::visit(const GologProgramTest& x) {
    std::ostringstream s;
    auto arg = x.arg_;
    PropositionalLogicNodeToStringVisitor v;
    s << "[" << v.apply(*arg) << "]?";
    result_ = s.str();
} 

void GologProgramNodeToString::visit(const GologProgramSequence& x) {
    std::ostringstream s;
    auto args = x.args_;
    s << "(";
    s << apply(**args.begin());
    for (auto it = ++(args.begin()); it != args.end(); ++it)
        s << ";" << apply(**it);
    s << ")";
    result_ = s.str();
}

void GologProgramNodeToString::visit(const GologProgramChoice& x) {
    std::ostringstream s;
    auto args = x.args_;
    s << "(";
    s << apply(**args.begin());
    for (auto it = ++(args.begin()); it != args.end(); ++it)
        s << "|" << apply(**it);
    s << ")";
    result_ = s.str();
}

void GologProgramNodeToString::visit(const GologProgramIteration& x) {
    std::ostringstream s;
    s << "(" << apply(*x.arg_) << ")*";
    result_ = s.str();
}

std::string GologProgramNodeToString::apply(const GologProgramNode& x) {
    x.accept(*this);
    return result_;
}

std::string to_string(const golog_ptr& x) {
    GologProgramNodeToString v;
    return v.apply(*x);
}

std::size_t ProgramActionPair::hash() const {
    return program_->hash() * std::hash<std::string>{}(action_);
}

bool ProgramActionPair::equals(const std::shared_ptr<const ProgramActionPair>& x) const {
    return (program_->equals(x->program_)) && (action_ == x->action_);
}

std::size_t ProgramTransition::hash() const {
    return (BDDHash{}(guard_)) * successor_program_->hash();
}

bool ProgramTransition::equals(const std::shared_ptr<const ProgramTransition>& x) const {
    return (guard_ == x->guard_) && (successor_program_->equals(x->successor_program_));
}

void TFCVisitor::visit(const GologProgramNil& x) {
    // recall nil=[true]?
    golog_ptr x_ptr = std::make_shared<GologProgramNil>();

    // debug
    std::cout << "Currently visiting program: nil" << std::endl;

    // function T
    // do nothing -- as for [true]?

    // function F
    result_.final_functions_.emplace(x_ptr, var_mgr_->cudd_mgr()->bddOne());

    // function C
    if (result_.continuation_functions_.find(x_ptr) != result_.continuation_functions_.end()) {
        result_.continuation_functions_[x_ptr].insert(var_mgr_->cudd_mgr()->bddOne());
    } else {
        result_.continuation_functions_[x_ptr] = {var_mgr_->cudd_mgr()->bddOne()};
    }
}

void TFCVisitor::visit(const GologProgramUnd& x) {
    // do nothing
    // i.e., we implement TFCResult as a partial function 
    // determinization is done during program graph construction
}

void TFCVisitor::visit(const GologProgramAction& x) {
    golog_ptr x_ptr = std::make_shared<GologProgramAction>(x.action_name_);
    golog_ptr nil = std::make_shared<GologProgramNil>();

    // debug
    std::cout << "Currently visiting program action: " << to_string(x_ptr) << std::endl; 

    // function T
    if (action_name_to_bdd_.find(x.action_name_) != action_name_to_bdd_.end()) {
        // i.e., action_name is in action_name_to_bdd_
        // create T(x_ptr, action_name)
        program_action_ptr pa = std::make_shared<ProgramActionPair>(x_ptr, x.action_name_);
        // generate program transition using rule T(a, a) = (poss(a), nil)
        transition_ptr t = std::make_shared<ProgramTransition>(action_name_to_pre_bdd_[x.action_name_], nil);
        // append transition to result

        // debug
        std::cout << "Generated transition. T(" << to_string(pa->program_) << ", " << x.action_name_ << ") = (" << t->guard_ << ", " << to_string(nil) << ")" << std::endl;   
        if (result_.transitions_.find(pa) != result_.transitions_.end()) 
            {result_.transitions_[pa].insert(t);}
        else {
            transition_set t_set = {t};
            result_.transitions_[pa] = t_set;
        }
    } else {
        throw std::runtime_error(x.action_name_+" is not a valid action name");
    }

    // function F
    // generate final states using rule F(a) = False
    result_.final_functions_.emplace(x_ptr, var_mgr_->cudd_mgr()->bddZero());

    // function C
    // generate continuation functions using rule C(a) = {True}
    if (result_.continuation_functions_.find(x_ptr) != result_.continuation_functions_.end()) {
        result_.continuation_functions_[x_ptr].insert(var_mgr_->cudd_mgr()->bddOne());
    } else {
        result_.continuation_functions_[x_ptr] = {var_mgr_->cudd_mgr()->bddOne()};
    }
}

void TFCVisitor::visit(const GologProgramTest& x) {
    golog_ptr x_ptr = std::make_shared<GologProgramTest>(x.arg_);
    auto bdd = to_bdd(x.arg_, var_mgr_);

    // debug
    std::cout << "Currently visiting program test: " << to_string(x_ptr) << std::endl;

    // function T
    // do nothing, as as prescribed by the rules

    // function F
    // generate final states using the rule F(\varphi?) = \varphi 
    result_.final_functions_.emplace(x_ptr, bdd);

    // function C
    // generate continuation function using the rule F([\varphi]?) = {\varphi}
    if (result_.continuation_functions_.find(x_ptr) != result_.continuation_functions_.end()) {
        result_.continuation_functions_[x_ptr].insert(bdd);
    } else {
        result_.continuation_functions_[x_ptr] = {bdd};
    }
}

void TFCVisitor::visit(const GologProgramSequence& x) {
    auto x_ptr = std::make_shared<GologProgramSequence>(x.args_);
    auto x_args = x.args_;

    // debug
    std::cout << "Currently visiting program sequence: " << to_string(x_ptr) << std::endl;

    // data structures for TFC results related to main program
    CUDD::BDD bdd_f = var_mgr_->cudd_mgr()->bddOne(); // function F
    bdd_set bdd_s = {}; // function C

    for (int i = 0; i < x_args.size(); ++i) {
        
        std::cout << "Current argument (" << i << "-th index) is: " << to_string(x_args[i]) << std::endl;
 
        // TFCVisitor av(var_mgr_, action_name_to_bdd_, action_name_to_pre_bdd_);
        // TFCResult av_result = av.apply(*x_args[i]);
        TFCResult av_result = this->apply(*x_args[i]);

        // function F
        // add function F for each subprogram
        for (const auto& f : av_result.final_functions_)
            result_.final_functions_.insert(f);
        // construct function F as the conjunction of the final function of the subprograms
        bdd_f *= result_.final_functions_[x_args[i]];

        // function T 
        for (const auto& p: av_result.transitions_) {
            // append results of visiting each arg to result
            result_.transitions_.insert(p);
            if (x_args[i]->equals(p.first->program_)) {
                program_action_ptr pa = std::make_shared<ProgramActionPair>(x_ptr, p.first->action_);
                for (const auto& t : p.second) {
                    // generate guard
                    CUDD::BDD guard = var_mgr_->cudd_mgr()->bddOne();
                    for (int j = 0; j<i; ++j) guard *= result_.final_functions_[x_args[j]];
                    guard *= t->guard_;
                    if (guard != var_mgr_->cudd_mgr()->bddZero()) {
                        // generate successor program
                        golog_ptr succ = t->successor_program_;
                        golog_vec vv = {succ};
                        for (int j = i+1; j<x_args.size(); ++j)
                            vv.push_back(x_args[j]);
                        // add program transition to result
                        golog_ptr succ_program = std::make_shared<const GologProgramSequence>(vv);
                        transition_ptr t_ptr = std::make_shared<ProgramTransition>(guard, succ_program);
                        // debug
                        std::cout << "Generated transition. T(" << to_string(pa->program_) << ", " << p.first->action_ << ") = (" << guard << ", " << to_string(succ_program) << ")" << std::endl;   
                        if (result_.transitions_.find(pa) != result_.transitions_.end()) 
                          {result_.transitions_[pa].insert(t_ptr);}
                        else {
                            transition_set t_set = {t_ptr};
                            result_.transitions_[pa] = t_set;
                        }
                        if (visited_programs_.find(succ_program) == visited_programs_.end())
                            programs_queue_.push(succ_program);
                    }
                }
            }
        }

        // function C
        // add function C for each subprogram
        for (const auto& c_bdd : av_result.continuation_functions_)
            result_.continuation_functions_.insert(c_bdd);
        CUDD::BDD c = var_mgr_->cudd_mgr()->bddOne();
        for (int j = 0; j<i; ++j) 
            c *= result_.final_functions_[x_args[j]];
        c *= !result_.final_functions_[x_args[i]];
        for (const auto &c_bdd : av_result.continuation_functions_[x_args[i]])
            bdd_s.insert(c*c_bdd);
    }

    // function F for the main program
    result_.final_functions_[x_ptr] = bdd_f;

    // function C for the main program
    result_.continuation_functions_[x_ptr] = bdd_s;
}

void TFCVisitor::visit(const GologProgramChoice& x) {
    auto x_ptr = std::make_shared<GologProgramChoice>(x.args_);
    auto x_args = x.args_;

    // debug
    std::cout << "Currently visiting program choice: " << to_string(x_ptr) << std::endl;

    // data structures for TFC results related to main program
    CUDD::BDD bdd_f = var_mgr_->cudd_mgr()->bddZero(); // function F
    bdd_set bdd_s = {}; // function C

    for (const auto& arg : x_args) {
        // debug
        std::cout << "Current argument is: " << to_string(arg) << std::endl;

        // create a TFCVisitor and apply to each arg
        // TFCVisitor av(var_mgr_, action_name_to_bdd_, action_name_to_pre_bdd_);
        // TFCResult av_result = av.apply(*arg);
        TFCResult av_result = this->apply(*arg);

        // function T
        for (const auto& p: av_result.transitions_){
            // append results of visiting each arg to result
            result_.transitions_.insert(p);
            if (arg -> equals(p.first->program_)) {
                program_action_ptr pa = std::make_shared<ProgramActionPair>(x_ptr, p.first->action_); 
                for (const auto& t : p.second) {
                    // debug
                    std::cout << "Generated transition. T(" << to_string(pa->program_) << ", " << p.first->action_ << ") = (" << t->guard_ << ", " << to_string(t->successor_program_) << ")" << std::endl;   
                    if (result_.transitions_.find(pa) != result_.transitions_.end()) 
                        result_.transitions_[pa].insert(t);
                    else {
                        transition_set t_set = {t};
                        result_.transitions_[pa] = t_set;
                    }
                }
            }   
        }

        // add function F for each subprogram
        for (const auto& f: av_result.final_functions_)
            result_.final_functions_.insert(f);
        // construct function F for the main program as the disjunction of the final function of the subprograms
        bdd_f += av_result.final_functions_[arg];

        // add function C for each subprogram
        for (const auto& c_bdd : av_result.continuation_functions_)
            result_.continuation_functions_.insert(c_bdd);
        // construct function C for the main program as the union of function C for each subprogram
        bdd_s.insert(av_result.continuation_functions_[arg].begin(), av_result.continuation_functions_[arg].end());
    }

    // add function F for the main program
    result_.final_functions_[x_ptr] = bdd_f;
    // add function C for the main program
    result_.continuation_functions_[x_ptr] = bdd_s;
}

void TFCVisitor::visit(const GologProgramIteration& x) {
    auto x_ptr = std::make_shared<GologProgramIteration>(x.arg_);
    auto x_arg = x.arg_;

    // debug
    std::cout << "Currently visiting program iteration: " << to_string(x_ptr) << std::endl;
    std::cout << "Argument of iteration is: " << to_string(x_arg) << std::endl;

    // data structure for results related to main program
    CUDD::BDD bdd_f = var_mgr_->cudd_mgr()->bddOne(); // function F
    bdd_set bdd_s = {var_mgr_->cudd_mgr()->bddOne()}; // function C
    golog_set tbv = {}; // programs to be visited in future

    // create a TFCVisitor and apply to x_arg: obtain TFC(arg)
    // TFCVisitor av(var_mgr_, action_name_to_bdd_, action_name_to_pre_bdd_);
    // TFCResult av_result = av.apply(*x_arg);
    TFCResult av_result = this->apply(*x_arg);

    // function F (needs to be computed before function T for iteration)
    // add function F for each subprogram
    for (const auto& f : av_result.final_functions_)
        result_.final_functions_.insert(f);
    // set function F for main program to True
    result_.final_functions_[x_ptr] = bdd_f;

    // function T
    // add function T for each subprgoram
    for (const auto& p: av_result.transitions_) {
        // append transitions resulting from arg to result 
        result_.transitions_.insert(p);
        // generate pair T(d, a)
        if (x_arg->equals(p.first->program_)) {
            program_action_ptr pa = std::make_shared<ProgramActionPair>(x_ptr, p.first->action_);
            for (const auto& t : p.second) {
                // generate guard as !F(d) && t->guard_
                CUDD::BDD trans_guard = (!result_.final_functions_[x_arg]) * t->guard_;
                if (trans_guard != var_mgr_->cudd_mgr()->bddZero())
                    {// take successor program d'
                    golog_ptr succ = t->successor_program_;
                    golog_vec vv = {succ, x_ptr};
                    // construct successor program as d';d*
                    golog_ptr succ_program = std::make_shared<const GologProgramSequence>(vv);
                    transition_ptr t_ptr = std::make_shared<ProgramTransition>(trans_guard, succ_program);
                    // debug
                    std::cout << "Generated transition. T(" << to_string(pa->program_) << ", " << p.first->action_ << ") = (" << trans_guard << ", " << to_string(succ_program) << ")" << std::endl;   
                    if (result_.transitions_.find(pa) != result_.transitions_.end()) 
                       {result_.transitions_[pa].insert(t_ptr);}
                    else {
                        transition_set t_set = {t_ptr};
                        result_.transitions_[pa] = t_set;
                    }
                    if (visited_programs_.find(succ_program) == visited_programs_.end())
                        programs_queue_.push(succ_program);
                        // add new successor program to queue it has not been visited yet
                }
            }
        }
    }   
    // function C
    // add function C for each subprogram
    for (const auto& c_bdd : av_result.continuation_functions_)
        result_.continuation_functions_.insert(c_bdd);
    // set function C for main program to {True}
    result_.continuation_functions_[x_ptr] = bdd_s;
}

TFCResult TFCVisitor::apply(const GologProgramNode& x) {
    // x.accept(*this);
    // return result_;
    golog_ptr x_ptr = x.shared_from_this();
    programs_queue_.push(x_ptr);
    while (!programs_queue_.empty()) {
        golog_ptr p = programs_queue_.front();
        programs_queue_.pop();
        visited_programs_.insert(p);
        p->accept(*this);
    }
    return result_;
}

TFCResult get_tfc(const golog_ptr& x, 
    const std::shared_ptr<Syft::VarMgr>& var_mgr, 
    const std::unordered_map<std::string, CUDD::BDD>& action_name_to_bdd,
    const std::unordered_map<std::string, CUDD::BDD>& action_name_to_pre_bdd) {
    TFCVisitor v(var_mgr, action_name_to_bdd, action_name_to_pre_bdd);
    return v.apply(*x);
}