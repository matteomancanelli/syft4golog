#include"propositional_logic.hpp"

// true
std::size_t PropositionalLogicTrue::hash() const {
    return type_id_;
}

std::size_t PropositionalLogicTrue::get_type_id() const {
    return type_id_;
}

bool PropositionalLogicTrue::equals(const PropositionalLogicNode& x) const {
    return type_id_ == x.get_type_id();
}

void PropositionalLogicTrue::accept(PropositionalLogicNodeVisitor& v) {v.visit(*this);}

// false
std::size_t PropositionalLogicFalse::hash() const {
    return type_id_;
}

std::size_t PropositionalLogicFalse::get_type_id() const {
    return type_id_;
}

bool PropositionalLogicFalse::equals(const PropositionalLogicNode& x) const {
    return type_id_ == x.get_type_id();
}

void PropositionalLogicFalse::accept(PropositionalLogicNodeVisitor& v) {v.visit(*this);}

// atoms
std::size_t PropositionalLogicAtom::hash() const {
    return type_id_ * std::hash<std::string>{}(name_);
}

void PropositionalLogicAtom::accept(PropositionalLogicNodeVisitor& v) {v.visit(*this);}

std::size_t PropositionalLogicAtom::get_type_id() const {
    return type_id_;
}

bool PropositionalLogicAtom::equals(const PropositionalLogicNode& x) const {
    return type_id_ == x.get_type_id() && (name_ == dynamic_cast<const PropositionalLogicAtom&>(x).name_);
}

// negation
std::size_t PropositionalLogicNegation::hash() const {
    return type_id_ * arg_->hash();
}

std::size_t PropositionalLogicNegation::get_type_id() const {
    return type_id_;
}

bool PropositionalLogicNegation::equals(const PropositionalLogicNode& x) const {    
    return type_id_ == x.get_type_id() && (arg_->equals(*dynamic_cast<const PropositionalLogicNegation&>(x).arg_));
}

void PropositionalLogicNegation::accept(PropositionalLogicNodeVisitor& v) {v.visit(*this);}

// conjunction
std::size_t PropositionalLogicConjunction::hash() const {
    std::size_t hash = type_id_;
    for (const auto& a : args_)
        hash *= a->hash();
    return hash;
}

std::size_t PropositionalLogicConjunction::get_type_id() const {
    return type_id_;
}

bool PropositionalLogicConjunction::equals(const PropositionalLogicNode& x) const {
    if (type_id_ == x.get_type_id()) {
        auto d = dynamic_cast<const PropositionalLogicConjunction&>(x);
        if (args_.size() != d.args_.size()) return false;
        auto d_args = d.args_;
        auto a = args_.begin();
        auto b = d_args.begin();
        for (; a != args_.end(); ++a, ++b) {
            if (!(*a)->equals(**b)) return false;
        }
        return true;
    } else {
        return false;
    }
}

void PropositionalLogicConjunction::accept(PropositionalLogicNodeVisitor& v) {v.visit(*this);}

// disjunction
std::size_t PropositionalLogicDisjunction::get_type_id() const {
    return type_id_;
}

std::size_t PropositionalLogicDisjunction::hash() const {
    std::size_t hash = type_id_;
    for (const auto& a : args_)
        hash *= a->hash();
    return hash;
}

bool PropositionalLogicDisjunction::equals(const PropositionalLogicNode& x) const {
    if (type_id_ == x.get_type_id()) {
        auto d = dynamic_cast<const PropositionalLogicDisjunction&>(x);
        if (args_.size() != d.args_.size()) return false;
        auto d_args = d.args_;
        auto a = args_.begin();
        auto b = d_args.begin();
        for (; a != args_.end(); ++a, ++b) {
            if (!(*a)->equals(**b)) return false;
        }
        return true;
    } else {
        return false;
    }
}

void PropositionalLogicDisjunction::accept(PropositionalLogicNodeVisitor& v) {v.visit(*this);}

// visitors
// to string visitor
void PropositionalLogicNodeToStringVisitor::visit(const PropositionalLogicTrue& x) {
    result_ = "true";
}

void PropositionalLogicNodeToStringVisitor::visit(const PropositionalLogicFalse& x) {
    result_ = "false";
}

void PropositionalLogicNodeToStringVisitor::visit(const PropositionalLogicAtom& x) {
    result_ = x.name_;
}

void PropositionalLogicNodeToStringVisitor::visit(const PropositionalLogicNegation& x) {
    std::ostringstream s;
    s << "!(" << apply(*x.arg_) << ")";
    result_ = s.str(); 
}

void PropositionalLogicNodeToStringVisitor::visit(const PropositionalLogicConjunction& x) {
    std::ostringstream s;
    auto args = x.args_;
    s << "(";
    s << apply(**args.begin());
    for (auto it = ++(args.begin()); it != args.end(); ++it) {
        s << " && " << apply(**it);
    }
    s << ")";
    result_ = s.str();
}

void PropositionalLogicNodeToStringVisitor::visit(const PropositionalLogicDisjunction& x) {
    std::ostringstream s;
    auto args = x.args_;
    s << "(";
    s << apply(**args.begin());
    for (auto it = ++(args.begin()); it != args.end(); ++it) {
        s << " || " << apply(**it);
    }
    s << ")";
    result_ = s.str();
}

std::string PropositionalLogicNodeToStringVisitor::apply(PropositionalLogicNode& f) {
    f.accept(*this);
    return result_;
}

// to BDD visitor
void PropositionalLogicNodeToBDDVisitor::visit(const PropositionalLogicTrue& x) {
    result_ = var_mgr_->cudd_mgr()->bddOne();
}

void PropositionalLogicNodeToBDDVisitor::visit(const PropositionalLogicFalse& x) {
    result_ = var_mgr_->cudd_mgr()->bddZero();
}

void PropositionalLogicNodeToBDDVisitor::visit(const PropositionalLogicAtom& x) {
    result_ = var_mgr_->name_to_variable(x.name_);
}

void PropositionalLogicNodeToBDDVisitor::visit(const PropositionalLogicNegation& x) {
    result_ = !(apply(*x.arg_));
}

void PropositionalLogicNodeToBDDVisitor::visit(const PropositionalLogicConjunction& x) {
    CUDD::BDD r = var_mgr_->cudd_mgr()->bddOne();
    auto args = x.args_;
    for (auto& a : args) r = r*apply(*a);
    result_ = r;
}

void PropositionalLogicNodeToBDDVisitor::visit(const PropositionalLogicDisjunction& x) {
    CUDD::BDD r = var_mgr_->cudd_mgr()->bddZero();
    auto args = x.args_;
    for (auto& a : args) r = r+apply(*a);
    result_ = r;
}

CUDD::BDD PropositionalLogicNodeToBDDVisitor::apply(PropositionalLogicNode& f) {
    f.accept(*this);
    return result_;
}