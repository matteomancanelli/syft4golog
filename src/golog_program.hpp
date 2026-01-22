// implementation of a simple class for Golog programs

// implementation based on AST design pattern

#ifndef GOLOG_PROGRAM_H
#define GOLOG_PROGRAM_H

#include<string>
#include<sstream>
#include<unordered_set>
#include<functional>
#include<queue>

#include"propositional_logic.hpp"
#include"domain.h"

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
class GologProgramNode;

typedef std::shared_ptr<const GologProgramNode> golog_ptr;
typedef std::vector<std::shared_ptr<const GologProgramNode>> golog_vec;

// abstract class for AST nodes representing Golog programs
class GologProgramNode : public std::enable_shared_from_this<GologProgramNode> {
    public:
        virtual ~GologProgramNode() = default;
        virtual void accept(GologProgramNodeVisitor& v) const = 0;
        virtual std::size_t hash() const = 0;
        virtual bool equals(const golog_ptr& x) const = 0;
        virtual std::size_t get_type_id() const = 0;
};

struct GologProgramHash {
    std::size_t operator()(const golog_ptr& x) const {return x->hash();}
};

struct GologProgramEquals {
    bool operator()(const golog_ptr& x1, const golog_ptr& x2) const {return x1->equals(x2);}
};

typedef std::unordered_set<std::shared_ptr<const GologProgramNode>, GologProgramHash, GologProgramEquals> golog_set;

// concrete AST nodes representing Golog Programs

// nil
class GologProgramNil: public GologProgramNode {
    public:
        const static GologProgramTypeID type_id_ = GologProgramTypeID::t_GologProgramNil;
        GologProgramNil() {}
        void accept(GologProgramNodeVisitor& x) const override;
        std::size_t hash() const override;
        bool equals(const golog_ptr& x) const override;
        std::size_t get_type_id() const; 
};

// undefined 
class GologProgramUnd : public GologProgramNode {
    public:
        const static GologProgramTypeID type_id_ = GologProgramTypeID::t_GologProgramUnd;
        GologProgramUnd() {}
        void accept(GologProgramNodeVisitor& v) const override;
        std::size_t hash() const override;
        bool equals(const golog_ptr& x) const override; 
        std::size_t get_type_id() const;
};

// action
class GologProgramAction : public GologProgramNode {
    public:
        const static GologProgramTypeID type_id_ = GologProgramTypeID::t_GologProgramAction;
        std::string action_name_ = "";
        GologProgramAction(const std::string& x): action_name_(x) {}
        void accept(GologProgramNodeVisitor& v) const override;
        std::size_t hash() const override;
        bool equals(const golog_ptr& x) const override; 
        std::size_t get_type_id() const override; 
};

// test
class GologProgramTest : public  GologProgramNode {
    public:
        const static GologProgramTypeID type_id_ = GologProgramTypeID::t_GologProgramTest;
        formula_ptr arg_; // argument of test is a propositional formula
        GologProgramTest(const formula_ptr& x): arg_(x) {}
        void accept(GologProgramNodeVisitor& v) const override;
        std::size_t hash() const override;
        bool equals(const golog_ptr& v) const override;
        std::size_t get_type_id() const;
};

// sequence
class GologProgramSequence : public GologProgramNode {
    public:
        const static GologProgramTypeID type_id_ = GologProgramTypeID::t_GologProgramSequence;
        golog_vec args_;
        GologProgramSequence(const golog_vec& c): args_(c) {}
        void accept(GologProgramNodeVisitor& v) const override;
        std::size_t hash() const override;
        bool equals(const golog_ptr& x) const override; 
        std::size_t get_type_id() const;
};

// choice
class GologProgramChoice : public GologProgramNode {
    public:
        const static GologProgramTypeID type_id_ = GologProgramTypeID::t_GologProgramChoice;
        golog_set args_;
        GologProgramChoice(const golog_set& c): args_(c) {}
        void accept(GologProgramNodeVisitor& v) const override;
        std::size_t hash() const override;
        bool equals(const golog_ptr& x) const override;
        std::size_t get_type_id() const;
};

// iteration
class GologProgramIteration : public GologProgramNode {
    public:
        const static GologProgramTypeID type_id_ = GologProgramTypeID::t_GologProgramIteration;
        golog_ptr arg_;
        GologProgramIteration(const golog_ptr& x): arg_(x) {}
        void accept(GologProgramNodeVisitor& v) const override;
        std::size_t hash() const override;
        bool equals(const golog_ptr& x) const override; 
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

        std::string apply(const GologProgramNode& x);
};

std::string to_string(const golog_ptr& x);

/**
 * \brief data structure to represent (program, action) input for function T
 */
struct ProgramActionPair {
    golog_ptr program_;
    std::string action_;

    ProgramActionPair(const golog_ptr& program, const std::string& action):
        program_(program), action_(action) {}

    std::size_t hash() const;
    bool equals(const std::shared_ptr<const ProgramActionPair>& x) const;
};

/**
 * \brief data structure to represent (guard, successor program) output for function T
 */
struct ProgramTransition {
    CUDD::BDD guard_;
    golog_ptr successor_program_;

    ProgramTransition(const CUDD::BDD& guard, const golog_ptr& successor_program):
        guard_(guard), successor_program_(successor_program) {}

    std::size_t hash() const; 
    bool equals(const std::shared_ptr<const ProgramTransition>& x) const;
};

// type definitions
typedef std::shared_ptr<const ProgramActionPair> program_action_ptr;
typedef std::shared_ptr<const ProgramTransition> transition_ptr;
typedef std::unordered_set<CUDD::BDD, BDDHash> bdd_set;
typedef std::queue<golog_ptr> golog_queue;

struct ProgramActionPairHash {
    std::size_t operator()(const program_action_ptr& x) const { return x->hash();}
};

struct ProgramActionPairEquals {
    bool operator()(const program_action_ptr& x1, const program_action_ptr& x2) const { return x1->equals(x2);}
};

struct ProgramTransitionHash {
    std::size_t operator()(const transition_ptr& x) const { return x->hash();}
};

struct ProgramTransitionEquals {
    bool operator()(const transition_ptr& x1, const transition_ptr& x2) const { return x1->equals(x2);}
};

typedef std::unordered_set<std::shared_ptr<const ProgramTransition>, ProgramTransitionHash, ProgramTransitionEquals> transition_set;

struct TFCResult {
    std::unordered_map<program_action_ptr, transition_set, ProgramActionPairHash, ProgramActionPairEquals> transitions_;
    std::unordered_map<golog_ptr, CUDD::BDD, GologProgramHash, GologProgramEquals> final_functions_; // F
    std::unordered_map<golog_ptr, CUDD::BDD, GologProgramHash, GologProgramEquals> continuation_functions_;

    void print() const; 
};

/**
 * \brief class TFCVisitor computes function TFC for a given program (and all its subprograms)
 */
class TFCVisitor : public GologProgramNodeVisitor {

    private:
        void get_continuation_function(const golog_ptr& program);

    public:
        std::shared_ptr<Syft::VarMgr> var_mgr_;
        std::unordered_map<std::string, CUDD::BDD> action_name_to_bdd_;
        std::unordered_map<std::string, CUDD::BDD> action_name_to_pre_bdd_;
        TFCResult result_;
        golog_set visited_programs_; // store programs that have been visited so far
        golog_queue programs_queue_; // programs that need to be visited

        void visit(const GologProgramNil&) override;
        void visit(const GologProgramUnd&) override;
        void visit(const GologProgramAction&) override;
        void visit(const GologProgramTest&) override;
        void visit(const GologProgramSequence&) override;
        void visit(const GologProgramChoice&) override;
        void visit(const GologProgramIteration&) override;

        TFCVisitor(std::shared_ptr<Syft::VarMgr> var_mgr,
            const std::unordered_map<std::string, CUDD::BDD>& action_name_to_bdd,
            const std::unordered_map<std::string, CUDD::BDD>& action_name_to_pre_bdd_) : 
                var_mgr_(var_mgr),
                action_name_to_bdd_(action_name_to_bdd),
                action_name_to_pre_bdd_(action_name_to_pre_bdd_),
                visited_programs_() {}

        TFCResult apply(const GologProgramNode& x);
};

TFCResult get_tfc(const golog_ptr& x, 
    const std::shared_ptr<Syft::VarMgr>& var_mgr, 
    const std::unordered_map<std::string, CUDD::BDD>& action_name_to_bdd,
    const std::unordered_map<std::string, CUDD::BDD>& action_name_to_pre_bdd_);

#endif