#pragma once

#include <boost/program_options.hpp>
#include "log_writer.hpp"

namespace po = boost::program_options;

class UserInterface {
  public:
    UserInterface();

    bool CheckParams(int argc, char** argv);
    void StartServer(LogPriority verbosity);
    
#ifdef UNIT_TEST_SRV
  public:
#else
  private:
#endif
    po::options_description description_;
    po::variables_map var_map_;

    const int defPort = 33333;
    const std::string defLogFilePath = "/var/log/vcalc.log";

    std::string client_base_file_path_;
    std::string log_file_path_;
    int port_;
};