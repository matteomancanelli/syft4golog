#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <memory>

#include "synthesis/header/VarMgr.h"
#include "utils/string_utilities.h"

#include "parser/parser.h"

#include "model/action.h"
#include "model/formula.h"
#include "model/reference.h"
#include "model/procedural.h"
#include "model/types.h"
#include "model/value.h"
#include "model/effect_axiom.h"

#include "syntactic_closure.h"
#include "transitions.h"
#include "utilities.h"

#include "domain.h"
extern "C" int yylex() { return 0; } ;

int main(int argc, char ** argv) {
    try {
        // test correct inclusion of LydiaSyft's functionalities
        std::string ts = Syft::to_upper_copy("hello, world!");
        std::cout << "Uppercase string: " << ts << std::endl;

        // test domain
        std::shared_ptr<Syft::VarMgr> var_mgr = std::make_shared<Syft::VarMgr>();
        std::string domain_file = "./../../examples/fond/domain.pddl";
        std::string init_file = "./../../examples/fond/test1.pddl";

        Domain domain(var_mgr, domain_file, init_file);
        Syft::SymbolicStateDfa domain_sdfa = domain.to_symbolic();
        domain.print_domain();

        // Parse the Golog program
        //std::string filename = "../examples/robot_program.gpp";
        //std::string filename = "../examples/minimal_example.gpp";
        std::string filename = "../examples/star_example.gpp";

        std::cout << "Parsing program from file..." << std::endl;
        gologpp::parser::parse_file(filename);
        std::cout << "Parsed program successfully!" << std::endl;
        

        // Get the main procedure in the program
        auto mainproc = gologpp::global_scope().lookup_global<gologpp::Procedure>("main");
        if (!mainproc) {
            std::cerr << "No procedure main() found in program!" << std::endl;
            return 1;
        }
        
        // Compute the syntactic closure
        const gologpp::Instruction& definition = mainproc->definition();
        auto syntactic_closure = computeSyntacticClosure(&definition);

        std::cout << "Syntactic closure of the main procedure:" << std::endl;
        std::cout << std::endl;

        
        // Create or find action 'A' for testing transition relations
        std::cout << "Creating action A..." << std::endl;
        const gologpp::Reference<gologpp::Action>* action_ref = nullptr;
        
        auto existing_action = gologpp::global_scope().lookup_global<gologpp::Action>("A");
        if (existing_action) {
            std::cout << "Found existing action A in global scope" << std::endl;
            action_ref = existing_action->make_ref(std::vector<gologpp::Expression*>{});
        } 
        else {
            std::cout << "No existing action found, creating a new one" << std::endl;
            
            auto action = std::make_shared<gologpp::Action>(
                &gologpp::global_scope(),
                gologpp::type<gologpp::VoidType>(),
                "A",
                std::vector<std::shared_ptr<gologpp::Variable>>{}
            );
            
            std::cout << "Action created successfully: " << action->to_string("") << std::endl;
            action_ref = action->make_ref(std::vector<gologpp::Expression*>{});
        }
        

        // Iterate through each instruction in the syntactic closure
        int i = 0;
        for (const auto* instruction : syntactic_closure) {
            // Print the instruction
            std::cout << "Subprogram " << i << std::endl;
            std::cout << instruction->to_string("") << std::endl;

            // Compute and print its final condition
            std::cout << "Final " << i << std::endl;
            std::cout << F(instruction)->to_string("") << std::endl;
            std::cout << std::endl;

            // Compute and print all possible transitions
            std::cout << "Trans " << i << std::endl;
            auto transitions = T(instruction, action_ref);
            for (const auto& transition : transitions) {
                std::cout << "    Transition: " << transition.first->to_string("") << " , " << transition.second->to_string("") << std::endl;
            }

            std::cout << "----------" << std::endl;
            i++;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}