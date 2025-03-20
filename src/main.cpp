#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <memory>

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


int main() {
    try {
        //std::string filename = "../examples/robot_program.gpp";
        //std::string filename = "../examples/minimal_example.gpp";
        std::string filename = "../examples/star_example.gpp";

        std::cout << "Parsing program from file..." << std::endl;
        gologpp::parser::parse_file(filename);
        std::cout << "Parsed program successfully!" << std::endl;
        
        // Get the main procedure
        auto mainproc = gologpp::global_scope().lookup_global<gologpp::Procedure>("main");
        if (!mainproc) {
            std::cerr << "No procedure main() found in program!" << std::endl;
            return 1;
        }
        
        const gologpp::Instruction& definition = mainproc->definition();
        auto syntactic_closure = computeSyntacticClosure(&definition);

        std::cout << "Syntactic closure of the main procedure:" << std::endl;
        std::cout << std::endl;


        std::cout << "Creating action A..." << std::endl;

        const gologpp::Reference<gologpp::Action>* action_ref = nullptr;
        
        // Look for any existing actions in the global scope first
        auto existing_action = gologpp::global_scope().lookup_global<gologpp::Action>("A");
        if (existing_action) {
            std::cout << "Found existing action A in global scope" << std::endl;
            action_ref = existing_action->make_ref(std::vector<gologpp::Expression*>{});
        } 
        else {
            std::cout << "No existing action found, creating a new one" << std::endl;
            
            // Create a minimal test with the action to isolate the issue
            auto action = std::make_shared<gologpp::Action>(
                &gologpp::global_scope(),
                gologpp::type<gologpp::VoidType>(),
                "A",
                std::vector<std::shared_ptr<gologpp::Variable>>{}
            );
            
            // Keep the action scope limited to avoid any potential issues
            std::cout << "Action created successfully: " << action->to_string("") << std::endl;
            action_ref = action->make_ref(std::vector<gologpp::Expression*>{});
        }
        

        int i = 0;
        for (const auto* instruction : syntactic_closure) {
            std::cout << "Subprogram " << i << std::endl;
            std::cout << instruction->to_string("") << std::endl;

            std::cout << "Final " << i << std::endl;
            std::cout << F(instruction)->to_string("") << std::endl;
            std::cout << std::endl;

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