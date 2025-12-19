#pragma once

#include <unordered_map>

#include "transitions_type.h"

struct SemCache {
  std::unordered_map<const gologpp::Instruction*, gologpp::Expression*> Fcache;

  struct Key {
    const gologpp::Instruction* delta;
    const gologpp::Reference<gologpp::Action>* action;
    bool operator==(const Key& o) const {
      return delta == o.delta && action == o.action;
    }
  };

  struct KeyHash {
    size_t operator()(const Key& k) const {
      return std::hash<const void*>()(k.delta) ^
             (std::hash<const void*>()(k.action) << 1);
    }
  };

  // Store POINTERS, not values
  std::unordered_map<Key, Transitions, KeyHash> Tcache;
};