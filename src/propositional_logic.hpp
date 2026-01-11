// implementation of a simple class for propositional formulas
// used for Golog tests

// implementation based on AST design pattern

# ifndef PROPOSITIONAL_LOGIC_H
# define PROPOSITIONAL_LOGIC_H

#include<string>
#include<sstream>
#include<unordered_set>
#include <functional>

#include"cuddObj.hh"

#include<synthesis/header/VarMgr.h>

// type ID for each concrete object of PropositionalLogicNode subclasses
// used to create hashes
enum PropositionalLogicTypeID {
    t_ProposositionalLogicTrue,
    t_ProposositionalLogicFalse,
    t_ProposositionalLogicAtom,
    t_ProposositionalLogicNegation,
    t_ProposositionalLogicConjunction,
    t_ProposositionalLogicDisjunction
};

class PropositionalLogicNodeVisitor;

// abstract class for AST nodes representing propositional formulas
class PropositionalLogicNode {

    public:
        virtual ~PropositionalLogicNode() = default;
        virtual void accept(PropositionalLogicNodeVisitor& v) = 0;
        virtual std::size_t hash() const = 0;
        virtual bool equals(const PropositionalLogicNode& x) const = 0;
        virtual std::size_t get_type_id() const = 0;

};

struct PropositionalLogicHash {
    std::size_t operator()(const PropositionalLogicNode* f) const {return f -> hash();}
};

struct PropositionalLogicEquals {
    bool operator()(const PropositionalLogicNode* a, const PropositionalLogicNode* b) const {return a->equals(*b);}
};

// Concrete AST nodes representing propositional formulas

// True
class PropositionalLogicTrue : public PropositionalLogicNode {
    public:
        const static PropositionalLogicTypeID type_id_ = PropositionalLogicTypeID::t_ProposositionalLogicTrue;
        PropositionalLogicTrue() {}
        void accept(PropositionalLogicNodeVisitor& v) override;
        std::size_t hash() const override;
        bool equals(const PropositionalLogicNode& x) const override;
        std::size_t get_type_id() const;
};

// False
class PropositionalLogicFalse : public PropositionalLogicNode {
    public:
        const static PropositionalLogicTypeID type_id_ = PropositionalLogicTypeID::t_ProposositionalLogicFalse;
        PropositionalLogicFalse() {}
        void accept(PropositionalLogicNodeVisitor& v) override;
        std::size_t hash() const override;
        bool equals(const PropositionalLogicNode& x) const override;
        std::size_t get_type_id() const;
};

// Atoms
class PropositionalLogicAtom : public PropositionalLogicNode {
    public:
        const static PropositionalLogicTypeID type_id_ = PropositionalLogicTypeID::t_ProposositionalLogicAtom;
        std::string name_ = "";        
        PropositionalLogicAtom(const std::string& x): name_(x) {}
        void accept(PropositionalLogicNodeVisitor& v) override;
        std::size_t hash() const override;
        bool equals(const PropositionalLogicNode& x) const override;
        std::size_t get_type_id() const;
};

// Negation
class PropositionalLogicNegation : public PropositionalLogicNode {
    public:
        const static PropositionalLogicTypeID type_id_ = PropositionalLogicTypeID::t_ProposositionalLogicNegation;
        PropositionalLogicNode* arg_;
        PropositionalLogicNegation(PropositionalLogicNode* x): arg_(x) {}
        void accept(PropositionalLogicNodeVisitor& v) override;
        std::size_t hash() const override;
        bool equals(const PropositionalLogicNode& x) const override;
        std::size_t get_type_id() const;

};

// Conjunction
class PropositionalLogicConjunction : public PropositionalLogicNode {
    public:
        std::unordered_set<PropositionalLogicNode*, PropositionalLogicHash, PropositionalLogicEquals> args_;
        const static PropositionalLogicTypeID type_id_ = PropositionalLogicTypeID::t_ProposositionalLogicConjunction;
        PropositionalLogicConjunction(const std::unordered_set<PropositionalLogicNode*, PropositionalLogicHash, PropositionalLogicEquals>* c): args_(*c) {}
        void accept(PropositionalLogicNodeVisitor& v) override;
        std::size_t hash() const override;
        bool equals(const PropositionalLogicNode& x) const override;
        std::size_t get_type_id() const;
};

// Disjunction
class PropositionalLogicDisjunction: public PropositionalLogicNode {
    public:
        std::unordered_set<PropositionalLogicNode*, PropositionalLogicHash, PropositionalLogicEquals> args_;
        const static PropositionalLogicTypeID type_id_ = PropositionalLogicTypeID::t_ProposositionalLogicDisjunction;
        PropositionalLogicDisjunction(const std::unordered_set<PropositionalLogicNode*, PropositionalLogicHash, PropositionalLogicEquals>* c): args_(*c) {}
        void accept(PropositionalLogicNodeVisitor& v) override;
        std::size_t hash() const override;
        bool equals(const PropositionalLogicNode& x) const override;
        std::size_t get_type_id() const;
};


// abstract class to visit propositional logic formulas
class PropositionalLogicNodeVisitor {

    public: 
        virtual void visit(const PropositionalLogicTrue&) = 0; 
        virtual void visit(const PropositionalLogicFalse&) = 0;
        virtual void visit(const PropositionalLogicAtom&) = 0;
        virtual void visit(const PropositionalLogicNegation&) = 0;
        virtual void visit(const PropositionalLogicConjunction&) = 0;
        virtual void visit(const PropositionalLogicDisjunction&) = 0;

};

// visitor that prints a formula in propositional logic
class PropositionalLogicNodeToStringVisitor : public PropositionalLogicNodeVisitor {
    public:
        std::string result_;

        void visit(const PropositionalLogicTrue&) override;
        void visit(const PropositionalLogicFalse&) override;
        void visit(const PropositionalLogicAtom&) override;
        void visit(const PropositionalLogicNegation&) override;
        void visit(const PropositionalLogicConjunction&) override;
        void visit(const PropositionalLogicDisjunction&) override;

        std::string apply(PropositionalLogicNode& f);
};

// visitor that transforms a formula in propositional logic to BDD
class PropositionalLogicNodeToBDDVisitor : public PropositionalLogicNodeVisitor {
    public:
        std::shared_ptr<Syft::VarMgr> var_mgr_;
        CUDD::BDD result_;

        void visit(const PropositionalLogicTrue&) override;
        void visit(const PropositionalLogicFalse&) override;
        void visit(const PropositionalLogicAtom&) override;
        void visit(const PropositionalLogicNegation&) override;
        void visit(const PropositionalLogicConjunction&) override;
        void visit(const PropositionalLogicDisjunction&) override;

        CUDD::BDD apply(PropositionalLogicNode& f);

        PropositionalLogicNodeToBDDVisitor(std::shared_ptr<Syft::VarMgr> var_mgr) : var_mgr_(var_mgr) {}
};

std::string to_str(const PropositionalLogicNode& f);
CUDD::BDD to_bdd(const PropositionalLogicNode& f);

#endif