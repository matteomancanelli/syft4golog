#include <iostream>
#include <memory>
#include <unordered_map>
#include <fstream>
#include <cudd.h>
#include <istream>
#include <filesystem>


#include "lydia/logic/to_ldlf.hpp"
#include "lydia/dfa/mona_dfa.hpp"
#include "lydia/parser/ldlf/driver.hpp"
#include "lydia/to_dfa/core.hpp"
#include "lydia/to_dfa/strategies/compositional/base.hpp"
#include "lydia/utils/print.hpp"
#include <synthesis/syn.h>


int main(int argc, char **argv) {
    auto dfa_strategy = whitemech::lydia::CompositionalStrategy();
    auto translator = whitemech::lydia::Translator(dfa_strategy);

    std::cout << "HERE" << std::endl;

    auto driver = std::make_shared<whitemech::lydia::parsers::ldlf::Driver>();
    std::cout << "HERE" << std::endl;
    
    //auto filename = "test.ldlf";
    //std::filesystem::path formula_path(filename);
    //driver->parse(formula_path.string().c_str());
    
    driver->parse("[true]end");
    /*std::shared_ptr<whitemech::lydia::AbstractDriver> driver;
    std::cout << "HERE" << std::endl;

    driver = std::make_shared<whitemech::lydia::parsers::ldlf::Driver>();
    std::cout << "HERE" << std::endl;
    */

    std::cout << "HERE" << std::endl;

    auto parsed_formula = driver->get_result();
    auto ldlf_parsed_formula = std::static_pointer_cast<const whitemech::lydia::LDLfFormula>(parsed_formula);

    std::cout << "HERE" << std::endl;

    auto my_dfa = translator.to_dfa(*ldlf_parsed_formula);
    auto my_mona_dfa = std::dynamic_pointer_cast<whitemech::lydia::mona_dfa>(my_dfa);

    std::cout << "HERE" << std::endl;

    whitemech::lydia::dfaPrint(my_mona_dfa->get_dfa(),
                               my_mona_dfa->get_nb_variables(),
                               my_mona_dfa->names,
                               my_mona_dfa->indices.data());

    std::cout << "HERE" << std::endl;

    std::string graphviz_path = "output.dot";
    whitemech::lydia::print_mona_dfa(my_mona_dfa->get_dfa(), graphviz_path,
                                     my_mona_dfa->get_nb_variables());

    auto flag = false;

    if (flag) {
        
        Cudd* mgr = new Cudd();
        std::unordered_map<unsigned, BDD> strategy;

        char* temp_filename = tmpnam(nullptr);
        std::string temp_filename_string = std::string(temp_filename);
        my_mona_dfa->export_dfa(temp_filename_string);

        std::string part_file = "test.part";
        Syft::syn test(mgr, temp_filename_string, part_file);

        bool res = test.realizablity_sys(strategy);
        
        if (res)
            std::cout << "Realizable" << std::endl;
        else
            std::cout << "Unrealizable" << std::endl;
    }

    return 0;
}
