#include"base_golog_synthesizer.h"

BaseGologSynthesizer::BaseGologSynthesizer(
    const std::string& domain_file,
    const std::string& init_file,
    const std::string& golog_file
) {
    Stopwatch parse_domain;
    parse_domain.start();    

    std::cout << "[syft4golog] Parsing domain...";
    
    var_mgr_ = std::make_shared<Syft::VarMgr>();
    domain_ = std::make_shared<Domain>(var_mgr_, domain_file, init_file);

    double t_parse_domain = parse_domain.stop().count() / 1000.0;
    running_times_.push_back(t_parse_domain);

    std::cout << "DONE [" << t_parse_domain << " s]" << std::endl;

    Stopwatch parse_golog;
    parse_golog.start(); 

    std::cout << "[syft4golog] Parsing Golog program..." << std::endl;

    std::ifstream in(golog_file);
    std::string program_str = "";
    std::getline(in, program_str);
 
    auto driver = std::make_shared<GologDriver>(); 

    std::stringstream program_stream(program_str);
    driver->parse(program_stream);
    golog_program_ = driver -> get_result();

    std::cout << "[syft4golog] Generated program: " <<  to_string(golog_program_) << std:: flush;

    double t_parse_golog = parse_golog.stop().count() / 1000.0;
    running_times_.push_back(t_parse_golog);

    std::cout << " DONE [" << t_parse_golog << " s]" << std::endl;
}