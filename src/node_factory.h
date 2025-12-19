#pragma once

#include <memory>
#include <utility>
#include <vector>

#include "model/procedural.h"
#include "model/formula.h"
#include "model/reference.h"
#include "model/types.h"
#include "model/value.h"
#include "model/effect_axiom.h"
#include "model/procedural.h"

struct NodeFactory {
    std::vector<std::unique_ptr<gologpp::Instruction>> instr_store;
    std::vector<std::unique_ptr<gologpp::Expression>> expr_store;

    template <class T, class... Args>
    T* makeInstr(Args&&... args) {
        auto p = std::make_unique<T>(std::forward<Args>(args)...);
        T* raw = p.get();
        instr_store.push_back(std::move(p));
        return raw;
    }

    template <class T, class... Args>
    T* makeExpr(Args&&... args) {
        auto p = std::make_unique<T>(std::forward<Args>(args)...);
        T* raw = p.get();
        expr_store.push_back(std::move(p));
        return raw;
    }

    // Singletons
    gologpp::Value* t = nullptr;
    gologpp::Value* f = nullptr;
    gologpp::Test*  nil = nullptr;   // "empty program" = Test(true)

    void initSingletons() {
        if (!t) t = makeExpr<gologpp::Value>(gologpp::type<gologpp::BoolType>(), true);
        if (!f) f = makeExpr<gologpp::Value>(gologpp::type<gologpp::BoolType>(), false);
        if (!nil) nil = makeInstr<gologpp::Test>(t);
    }
};
