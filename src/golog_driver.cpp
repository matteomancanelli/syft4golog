#include"golog_driver.hpp"

GologDriver::~GologDriver() {
    delete (scanner);
    scanner = nullptr;
    delete (parser);
    parser = nullptr;
    delete (result);
    result = nullptr;
}

void GologDriver::parse_helper(std::istream& stream) {
    delete (scanner);
    try {
        scanner = new GologScanner(&stream);
    } catch (std::bad_alloc& ba) {
        std::cerr << "Failed to allocate scanner: (" << ba.what() << "). Termination!\n";
        exit(EXIT_FAILURE);
    }
    delete (parser);
    try {
        parser = new yy::GologParser((*scanner) /* scanner */, (*this) /* driver */);        
    } catch (std::bad_alloc& ba) {
        std::cerr << "Failed to allocate parser: (" << ba.what() << "). Termination!\n";
        exit(EXIT_FAILURE);
    }
    const int accept(0);
    if (parser->parse() != accept) {
        std::cerr << "Parse failed!\n";
        exit(EXIT_FAILURE);
    }
}

void GologDriver::parse(const char* const filename) {
    assert(filename != nullptr);
    std::ifstream in_file(filename);
    if (!in_file.good()) {
        exit(EXIT_FAILURE);
    }
    parse_helper(in_file);
}

void GologDriver::parse(std::istream& stream) {
    if (!stream.good() && stream.eof()) {
        return;
    }
    parse_helper(stream);
}

PropositionalLogicNode* GologDriver::add_PropositionalLogicTrue() const {
    return new PropositionalLogicTrue();
}

PropositionalLogicNode* GologDriver::add_PropositionalLogicFalse() const {
    return new PropositionalLogicFalse();
}

PropositionalLogicNode* GologDriver::add_PropositionalLogicAtom(std::string s) const {
    return new PropositionalLogicAtom(s);
}

PropositionalLogicNode* GologDriver::add_PropositionalLogicNegation(PropositionalLogicNode& arg) const {
    return new PropositionalLogicNegation(&arg);
}

PropositionalLogicNode* GologDriver::add_PropositionalLogicConjunction(
    PropositionalLogicNode& lhs,
    PropositionalLogicNode& rhs   
) const {
    std::unordered_set<PropositionalLogicNode*, PropositionalLogicHash, PropositionalLogicEquals> c = { &lhs, &rhs };
    return new PropositionalLogicConjunction(&c);
}

PropositionalLogicNode* GologDriver::add_PropositionalLogicDisjunction(
    PropositionalLogicNode& lhs,
    PropositionalLogicNode& rhs   
) const {
    std::unordered_set<PropositionalLogicNode*, PropositionalLogicHash, PropositionalLogicEquals> c = { &lhs, &rhs };
    return new PropositionalLogicDisjunction(&c);
}

PropositionalLogicNode* GologDriver::add_PropositionalLogicImplication(
    PropositionalLogicNode& lhs,
    PropositionalLogicNode& rhs
) const {
    // a -> b = ~a || b
    auto* neg = new PropositionalLogicNegation(&lhs);
    auto* c = new std::unordered_set<PropositionalLogicNode*, PropositionalLogicHash, PropositionalLogicEquals>{
        neg, &rhs
    };
    return new PropositionalLogicDisjunction(c);
}

PropositionalLogicNode* GologDriver::add_PropositionalLogicEquivalence(
    PropositionalLogicNode& lhs,
    PropositionalLogicNode& rhs
) const {
    auto* neg_lhs = new PropositionalLogicNegation(&lhs);
    auto* arg_1   = new PropositionalLogicDisjunction(
        new std::unordered_set<PropositionalLogicNode*, PropositionalLogicHash, PropositionalLogicEquals>{ neg_lhs, &rhs }
    );

    auto* neg_rhs = new PropositionalLogicNegation(&rhs);
    auto* arg_2   = new PropositionalLogicDisjunction(
        new std::unordered_set<PropositionalLogicNode*, PropositionalLogicHash, PropositionalLogicEquals>{ &lhs, neg_rhs }
    );

    return new PropositionalLogicConjunction(
        new std::unordered_set<PropositionalLogicNode*, PropositionalLogicHash, PropositionalLogicEquals>{ arg_1, arg_2 }
    );
}

GologProgramNode* GologDriver::add_GologProgramNil() const {
    return new GologProgramNil();
}

GologProgramNode* GologDriver::add_GologProgramUnd() const {
    return new GologProgramUnd();
}

GologProgramNode* GologDriver::add_GologProgramAction(std::string s) const {
    return new GologProgramAction(s);
}

GologProgramNode* GologDriver::add_GologProgramTest(PropositionalLogicNode& f) const {
    return new GologProgramTest(&f);
}

GologProgramNode* GologDriver::add_GologProgramSequence(
    GologProgramNode& fst,
    GologProgramNode& snd
) const {
    std::vector<GologProgramNode*> vv = {&fst, &snd};
    return new GologProgramSequence(&vv);
}

GologProgramNode* GologDriver::add_GologProgramChoice(
    GologProgramNode& lhs,
    GologProgramNode& rhs
) const {
    std::unordered_set<GologProgramNode*, GologProgramHash, GologProgramEquals> ss = {&lhs, &rhs};
    return new GologProgramChoice(&ss);
}

GologProgramNode* GologDriver::add_GologProgramIteration(
    GologProgramNode& arg 
) const {
    return new GologProgramIteration(&arg);
}

