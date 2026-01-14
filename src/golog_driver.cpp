#include"golog_driver.hpp"

GologDriver::~GologDriver() {
    delete (scanner);
    scanner = nullptr;
    delete (parser);
    parser = nullptr;
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

formula_ptr GologDriver::add_PropositionalLogicTrue() const {
    return std::make_shared<const PropositionalLogicTrue>();
}

formula_ptr GologDriver::add_PropositionalLogicFalse() const {
    return std::make_shared<const PropositionalLogicFalse>();
}

formula_ptr GologDriver::add_PropositionalLogicAtom(std::string s) const {
    return std::make_shared<const PropositionalLogicAtom>(s);
}

formula_ptr GologDriver::add_PropositionalLogicNegation(formula_ptr& arg) const {
    return std::make_shared<const PropositionalLogicNegation>(arg);
}

formula_ptr GologDriver::add_PropositionalLogicConjunction(
    formula_ptr& lhs,
    formula_ptr& rhs   
) const {
    formula_set c = { lhs, rhs };
    return std::make_shared<const PropositionalLogicConjunction>(c);
}

formula_ptr GologDriver::add_PropositionalLogicDisjunction(
    formula_ptr& lhs,
    formula_ptr& rhs   
) const {
    formula_set c = { lhs, rhs };
    return std::make_shared<const PropositionalLogicDisjunction>(c);
}

formula_ptr GologDriver::add_PropositionalLogicImplication(
    formula_ptr& lhs,
    formula_ptr& rhs
) const {
    // a -> b = ~a || b
    formula_ptr not_lhs = std::make_shared<const PropositionalLogicNegation>(lhs);
    formula_set d = {not_lhs, rhs}; 
    formula_ptr impl = std::make_shared<const PropositionalLogicDisjunction>(d);
    return impl;
}

formula_ptr GologDriver::add_PropositionalLogicEquivalence(
    formula_ptr& lhs,
    formula_ptr& rhs
) const {
    formula_ptr not_lhs = std::make_shared<const PropositionalLogicNegation>(lhs);
    formula_set d1 = {not_lhs, rhs}; 
    formula_ptr c1 = std::make_shared<const PropositionalLogicDisjunction>(d1);

    formula_ptr not_rhs = std::make_shared<const PropositionalLogicNegation>(rhs);
    formula_set d2 = {not_rhs, lhs};
    formula_ptr c2 = std::make_shared<const PropositionalLogicDisjunction>(d2);

    formula_set c = {c1, c2};
    return std::make_shared<const PropositionalLogicConjunction>(c);
}

golog_ptr GologDriver::add_GologProgramNil() const {
    return std::make_shared<const GologProgramNil>();
}

golog_ptr GologDriver::add_GologProgramUnd() const {
    return std::make_shared<const GologProgramUnd>();
}

golog_ptr GologDriver::add_GologProgramAction(std::string s) const {
    return std::make_shared<const GologProgramAction>(s);
}

golog_ptr GologDriver::add_GologProgramTest(formula_ptr& f) const {
    return std::make_shared<const GologProgramTest>(f);
}

golog_ptr GologDriver::add_GologProgramSequence(
    golog_ptr& fst,
    golog_ptr& snd
) const {
    std::vector<golog_ptr> vv = {fst, snd};
    return std::make_shared<const GologProgramSequence>(vv);
}

golog_ptr GologDriver::add_GologProgramChoice(
    golog_ptr& lhs,
    golog_ptr& rhs
) const {
    golog_set s = {lhs, rhs};
    return std::make_shared<const GologProgramChoice>(s);
}

golog_ptr GologDriver::add_GologProgramIteration(
    golog_ptr& arg 
) const {
    return std::make_shared<const GologProgramIteration>(arg);
}

