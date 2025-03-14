#include <iostream>
#include <fstream>
#include "parser.h"

int main() {
    try {
        std::string filename = "../examples/robot_program.gpp";

        std::cout << "Parsing program from file..." << std::endl;
        gologpp::parser::parse_file(filename);
        std::cout << "Parsed program successfully!" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}