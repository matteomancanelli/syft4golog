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
    std::cout << "Currently visiting program: nil" << std::endl;

    // recall nil=[true]?
    golog_ptr x_ptr = std::make_shared<GologProgramNil>();
    CUDD::BDD bdd_true = var_mgr_->cudd_mgr()->bddOne();

    // function T
    // do nothing

    // function F
    result_.final_functions_.emplace(x_ptr, bdd_true);

    // function C
    result_.continuation_functions_[x_ptr].insert(bdd_true);
}

void TFCVisitor::visit(const GologProgramUnd& x) {
    // do nothing
    // i.e., we implement TFCResult as a partial function 
    // determinization is done during program graph construction
}

void TFCVisitor::visit(const GologProgramAction& x) {
    golog_ptr x_ptr = std::make_shared<GologProgramAction>(x.action_name_);
    golog_ptr nil = std::make_shared<GologProgramNil>();

    // std::cout << "Currently visiting program action: " << to_string(x_ptr) << std::endl; 

    auto it = action_name_to_bdd_.find(x.action_name_);
    if (it == action_name_to_bdd_.end()) {
        throw std::runtime_error(x.action_name_ + " is not a valid action name");
    }    

    // function T
    program_action_ptr pa = std::make_shared<ProgramActionPair>(x_ptr, x.action_name_);
    transition_ptr t = std::make_shared<ProgramTransition>(action_name_to_pre_bdd_[x.action_name_], nil);

    // std::cout << "Generated transition. T(" << to_string(pa->program_)
    //           << ", " << x.action_name_
    //           << ") = (" << t->guard_ << ", " << to_string(nil) << ")" << std::endl;

    result_.transitions_[pa].insert(t);

    // function F
    // generate final states using rule F(a) = False
    CUDD::BDD bdd_false = var_mgr_->cudd_mgr()->bddZero();
    CUDD::BDD bdd_true = var_mgr_->cudd_mgr()->bddOne();

    result_.final_functions_.emplace(x_ptr, bdd_false);

    // function C
    // generate continuation functions using rule C(a) = {True}
    result_.continuation_functions_[x_ptr].insert(bdd_true);
}

void TFCVisitor::visit(const GologProgramTest& x) {
    golog_ptr x_ptr = std::make_shared<GologProgramTest>(x.arg_);
    auto bdd = to_bdd(x.arg_, var_mgr_);

    // debug
    // std::cout << "Currently visiting program test: " << to_string(x_ptr) << std::endl;

    // function T
    // do nothing

    // function F
    // generate final states using the rule F(\varphi?) = \varphi 
    result_.final_functions_.emplace(x_ptr, bdd);

    // function C
    // generate continuation function using the rule F([\varphi]?) = {\varphi}
    result_.continuation_functions_[x_ptr].insert(bdd);
}

void TFCVisitor::visit(const GologProgramSequence& x) {
    auto x_ptr = std::make_shared<const GologProgramSequence>(x.args_);
    const auto& x_args = x.args_;

    // debug
    // std::cout << "Currently visiting program sequence: " << to_string(x_ptr) << std::endl;

    // data structures for functions F and C
    CUDD::BDD f = var_mgr_->cudd_mgr()->bddOne();
    bdd_set c;

    for (int i = 0; i < x_args.size(); ++i) {
        const auto& arg = x_args[i];
        // debug
        // std::cout << "Current argument (" << i << "-th index) is: " << to_string(arg) << std::endl;

        // check whether argument has been visited
        // if not, visit it
        auto it = visited_programs_.find(arg);
        if (it == visited_programs_.end()) {
            visited_programs_.insert(arg);
            arg->accept(*this);
        }

        // function F
        f *= result_.final_functions_[arg];

        // function T
        for (const auto& p : result_.transitions_) {
            if (arg -> equals(p.first->program_)) {
                program_action_ptr pa = std::make_shared<ProgramActionPair>(x_ptr, p.first->action_);
                for (const auto& t : p.second) {
                    // generate transition guard
                    CUDD::BDD guard = var_mgr_->cudd_mgr()->bddOne();
                    for (int j = 0; j<i; ++j) guard *= result_.final_functions_[x_args[j]];
                    guard *= t->guard_;
                    if (guard != var_mgr_->cudd_mgr()->bddZero()) {
                        golog_ptr succ = t->successor_program_;
                        golog_vec vv = {succ};
                        for (int j = i+1; j<x_args.size(); ++j)
                            vv.push_back(x_args[j]);
                        golog_ptr succ_program; 
                        if (vv.size() == 1) succ_program = vv[0];
                        else succ_program = std::make_shared<const GologProgramSequence>(vv);
                        transition_ptr t_ptr = std::make_shared<ProgramTransition>(guard, succ_program);
                        // std::cout << "Generated transition. T(" << to_string(pa->program_) << ", " << p.first->action_ << ") = (" << guard << ", " << to_string(succ_program) << ")" << std::endl;   
                        result_.transitions_[pa].insert(t_ptr);
                        if (visited_programs_.find(succ_program) == visited_programs_.end())
                            programs_queue_.push(succ_program);
                    } 
                }
            }
        }
        // function C
        CUDD::BDD cc = var_mgr_->cudd_mgr()->bddOne();
        for (int j = 0; j<i; ++j) 
            cc *= result_.final_functions_[x_args[j]];
        cc *= !result_.final_functions_[arg];
        for (const auto& c_bdd : result_.continuation_functions_[arg])
            c.insert(cc * c_bdd);
    }
    result_.final_functions_[x_ptr] = f;
    result_.continuation_functions_[x_ptr] = c;
}

void TFCVisitor::visit(const GologProgramChoice& x) {
    auto x_ptr = std::make_shared<GologProgramChoice>(x.args_);
    const auto& x_args = x.args_;

    // debug
    // std::cout << "Currently visiting program choice: " << to_string(x_ptr) << std::endl;

    // data structures for results of functions F and C
    CUDD::BDD f = var_mgr_->cudd_mgr()->bddZero();
    bdd_set c;

    for (const auto& arg : x_args) {
        // debug
        // std::cout << "Current argument is: " << to_string(arg) << std::endl;

        // check if arg program has been visited
        // if not, visit it
        auto it = visited_programs_.find(arg);
        if (it == visited_programs_.end()) { 
            visited_programs_.insert(arg);
            // this->apply(*arg);
            arg->accept(*this);
        }

        // construct transitions T for choice program
        for (const auto& p : result_.transitions_) {
            if (arg -> equals(p.first->program_)) {
                auto pa_main = std::make_shared<ProgramActionPair>(x_ptr, p.first->action_);
                for (const auto& t : p.second) {
                    // debug
                    // std::cout << "Generated transition. T(" << to_string(pa_main->program_)
                    //           << ", " << pa_main->action_
                    //           << ") = (" << t->guard_ << ", "
                    //           << to_string(t->successor_program_) << ")" << std::endl;
                    result_.transitions_[pa_main].insert(t);
                }
            }
        }

        // functions F and C for choice program
        f += result_.final_functions_[arg];
        c.insert(result_.continuation_functions_[arg].begin(), result_.continuation_functions_[arg].end());
    }
    result_.final_functions_[x_ptr] = f;
    result_.continuation_functions_[x_ptr] = c;
}

void TFCVisitor::visit(const GologProgramIteration& x) {
    auto x_ptr = std::make_shared<GologProgramIteration>(x.arg_);
    const auto& x_arg = x.arg_;

    // debug
    // std::cout << "Currently visiting program iteration: " << to_string(x_ptr) << std::endl;
    // std::cout << "Argument of iteration is: " << to_string(x_arg) << std::endl;

    // check whether arg has been visited
    // if not, visit it
    auto it = visited_programs_.find(x_arg);
    if (it == visited_programs_.end()) {
        visited_programs_.insert(x_arg);
        // this->apply(*x_arg);
        x_arg->accept(*this);
    }

    for (const auto& p : result_.transitions_) {
        if (x_arg -> equals(p.first->program_)) {
            program_action_ptr pa = std::make_shared<ProgramActionPair>(x_ptr, p.first->action_);
            for (const auto& t : p.second) {
                CUDD::BDD guard = (!result_.final_functions_[x_arg]) * t->guard_;
                if (guard != var_mgr_->cudd_mgr() -> bddZero()) {
                    // take successor program d' of arg
                    golog_ptr succ = t->successor_program_;
                    // construct program d';d*
                    golog_vec vv = {succ, x_ptr};
                    golog_ptr succ_program = std::make_shared<const GologProgramSequence>(vv);
                    transition_ptr t_ptr = std::make_shared<ProgramTransition>(guard, succ_program);

                    // std::cout << "Generated transition. T(" << to_string(pa->program_)
                    //           << ", " << pa->action_ << ") = ("
                    //           << guard << ", " << to_string(succ_program) << ")" << std::endl;

                    result_.transitions_[pa].insert(t_ptr);
                    // if new successor program has not been visited, push it into the queue
                    if (visited_programs_.find(succ_program) == visited_programs_.end())
                        programs_queue_.push(succ_program);
                }
            }
        }
    }
    // data structures for functions F and C of iteration program
    CUDD::BDD bdd_true = var_mgr_->cudd_mgr()->bddOne();
    result_.final_functions_[x_ptr] = bdd_true;
    result_.continuation_functions_[x_ptr] = {bdd_true};
}

TFCResult TFCVisitor::apply(const GologProgramNode& x) {
    golog_ptr nil_ptr = std::make_shared<GologProgramNil>();
    programs_queue_.push(nil_ptr);

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
    // v.apply(GologProgramNil()); // visit nil before everything
    return v.apply(*x);
}