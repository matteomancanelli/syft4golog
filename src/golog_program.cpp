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
        auto d_args = d.args_;
        if (args_.size() != d_args.size()) return false; 
        auto a = args_.begin(); 
        auto b = args_.begin();
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
    if (type_id_ == x->get_type_id()) {
        auto d = dynamic_cast<const GologProgramChoice&>(*x);
        auto d_args = d.args_;
        if (args_.size() != d_args.size()) return false; 
        auto a = args_.begin(); 
        auto b = args_.begin();
        for (; a != args_.end(); ++a, ++b) {
            if (!(*a)->equals(*b)) return false;
        }
        return true;
    } else {
        return false;
    }
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