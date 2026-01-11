// implementation of a simple class for Golog programs

// implementation based on AST design pattern

#ifndef GOLOG_PROGRAM_H
#define GOLOG_PROGRAM_H

#include<string>
#include<sstream>
#include<unordered_set>
#include<functional>

#include"propositional_logic.hpp"

// type ID for each concrete object of GologProgramNode subclasses

enum GologProgramTypeID {
    t_GologProgramNil, // 
    t_GologProgramUnd,
    t_GologProgramAction,
    t_GologProgramTest,
    t_GologProgramSequence,
    t_GologProgramChoice,
    t_GologProgramIteration
    // TODO. Add t_GologProgramProcedure
};

class GologProgramNodeVisitor;

// abstract class for AST nodes representing Golog programs
class GologProgramNode {
    public:
        virtual ~GologProgramNode() = default;
        virtual void accept(GologProgramNodeVisitor& v) = 0;
        virtual std::size_t hash() const = 0;
        virtual bool equals(const GologProgramNode& x) const = 0;
        virtual std::size_t get_type_id() const = 0;
};

struct GologProgramHash {
    std::size_t operator()(const GologProgramNode* x) const {return x->hash();}
};

struct GologProgramEquals {
    bool operator()(const GologProgramNode* x1, const GologProgramNode* x2) const {return x1->equals(*x2);}
};

// concrete AST nodes representing Golog Programs

// nil
class GologProgramNil: public GologProgramNode {
    public:
        const static GologProgramTypeID type_id_ = GologProgramTypeID::t_GologProgramNil;
        GologProgramNil() {}
        void accept(GologProgramNodeVisitor& x) override;
        std::size_t hash() const override;
        bool equals(const GologProgramNode& x) const override;
        std::size_t get_type_id() const; 
};

// undefined 
class GologProgramUnd : public GologProgramNode {
    public:
        const static GologProgramTypeID type_id_ = GologProgramTypeID::t_GologProgramUnd;
        GologProgramUnd() {}
        void accept(GologProgramNodeVisitor& v) override;
        std::size_t hash() const override;
        bool equals(const GologProgramNode& x) const override; 
        std::size_t get_type_id() const;
};

// action
class GologProgramAction : public GologProgramNode {
    public:
        const static GologProgramTypeID type_id_ = GologProgramTypeID::t_GologProgramAction;
        std::string action_name_ = "";
        GologProgramAction(const std::string& x): action_name_(x) {}
        void accept(GologProgramNodeVisitor& v) override;
        std::size_t hash() const override;
        bool equals(const GologProgramNode& x) const override; 
        std::size_t get_type_id() const override; 
};

// test
class GologProgramTest : public  GologProgramNode {
    public:
        const static GologProgramTypeID type_id_ = GologProgramTypeID::t_GologProgramTest;
        PropositionalLogicNode* arg_; // argument of test is a propositional formula
        GologProgramTest(PropositionalLogicNode* x): arg_(x) {}
        void accept(GologProgramNodeVisitor& v) override;
        std::size_t hash() const override;
        bool equals(const GologProgramNode& v) const override;
        std::size_t get_type_id() const;
};

// sequence
class GologProgramSequence : public GologProgramNode {
    public:
        const static GologProgramTypeID type_id_ = GologProgramTypeID::t_GologProgramSequence;
        std::vector<GologProgramNode*> args_;
        GologProgramSequence(const std::vector<GologProgramNode*>* c): args_(*c) {}
        void accept(GologProgramNodeVisitor& v) override;
        std::size_t hash() const override;
        bool equals(const GologProgramNode& x) const override; 
        std::size_t get_type_id() const;
};

// choice
class GologProgramChoice : public GologProgramNode {
    public:
        const static GologProgramTypeID type_id_ = GologProgramTypeID::t_GologProgramChoice;
        std::unordered_set<GologProgramNode*, GologProgramHash, GologProgramEquals> args_;
        GologProgramChoice(const std::unordered_set<GologProgramNode*, GologProgramHash, GologProgramEquals>* c): args_(*c) {}
        void accept(GologProgramNodeVisitor& v) override;
        std::size_t hash() const override;
        bool equals(const GologProgramNode& x) const override;
        std::size_t get_type_id() const;
};

// iteration
class GologProgramIteration : public GologProgramNode {
    public:
        const static GologProgramTypeID type_id_ = GologProgramTypeID::t_GologProgramIteration;
        GologProgramNode* arg_;
        GologProgramIteration(GologProgramNode* x): arg_(x) {}
        void accept(GologProgramNodeVisitor& v) override;
        std::size_t hash() const override;
        bool equals(const GologProgramNode& x) const override; 
        std::size_t get_type_id() const;
};

// abstract class for visitors of Golog programs
class GologProgramNodeVisitor {
    public:
        virtual void visit(const GologProgramNil&) = 0;
        virtual void visit(const GologProgramUnd&) = 0;
        virtual void visit(const GologProgramAction&) = 0;
        virtual void visit(const GologProgramTest&) = 0;
        virtual void visit(const GologProgramSequence&) = 0;
        virtual void visit(const GologProgramChoice&) = 0;
        virtual void visit(const GologProgramIteration&) = 0;
};

// visitor that transforms a Golog program in string
class GologProgramNodeToString : public GologProgramNodeVisitor {
    public:
        std::string result_;

        void visit(const GologProgramNil&) override;
        void visit(const GologProgramUnd&) override;
        void visit(const GologProgramAction&) override;
        void visit(const GologProgramTest&) override;
        void visit(const GologProgramSequence&) override;
        void visit(const GologProgramChoice&) override;
        void visit(const GologProgramIteration&) override;

        std::string apply(GologProgramNode& x);
};

#endif