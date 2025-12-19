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

extern "C" int yylex() { return 0; } ;

int main(int argc, char ** argv) {
    try {
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
        
        NodeFactory factory;
        factory.initSingletons();
        SemCache cache;

        std::cerr << "Computing the syntactic closure..." << std::endl;
        // Compute the syntactic closure
        const gologpp::Instruction& definition = mainproc->definition();
        InstructionSet syntactic_closure;
        computeSyntacticClosure(&definition, syntactic_closure, factory);
        std::cerr << "Syntactic closure computed!" << std::endl;

        //std::vector<const gologpp::Instruction*> nodes(syntactic_closure.begin(), syntactic_closure.end());
        std::vector<const gologpp::Instruction*> nodes;
        nodes.reserve(syntactic_closure.size());
        for (const auto* instruction : syntactic_closure) {
            nodes.push_back(instruction);
        }

        std::unordered_map<const gologpp::Instruction*, int> id;
        id.reserve(nodes.size());
        for (int i = 0; i < (int) nodes.size(); ++i) 
            id[nodes[i]] = i;

        // Choose a target action for demo: "A"
        //auto actions = gpp.actions(); // if your API differs, adjust
        //gologpp::Action* A = findActionByName(actions, "A");
        //if (!A) {
        //    std::cout << "Action A not found; transitions demo will be empty.\n";
        //}

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
        std::cout << "Iterating through syntactic closure with " << syntactic_closure.size() << " instructions." << std::endl;



        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}