#include"base_golog_synthesizer.h"

BaseGologSynthesizer::BaseGologSynthesizer(
    const std::string& domain_file,
    const std::string& init_file,
    const std::string& golog_file
) {
    std::cout << "[syft4golog] Parsing domain...";
    var_mgr_ = std::make_shared<Syft::VarMgr>();

    domain_ = std::make_shared<Domain>(var_mgr_, domain_file, init_file);
    std::cout << "DONE" << std::endl;

    std::cout << "[syft4golog] Parsing Golog program..." << std::flush;

    std::ifstream in(golog_file);
    std::string program_str = "";
    std::getline(in, program_str);
 
    auto driver = std::make_shared<GologDriver>(); 

    std::stringstream program_stream(program_str);
    driver->parse(program_stream);
    golog_program_ = driver -> get_result();

    std::cout << "DONE" << std::endl;
}