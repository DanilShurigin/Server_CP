#include <sstream>
#include <iostream>
#include <fstream>
#include <limits>

#include "server_exceptions.hpp"
#include "log_writer.hpp"
#include "client_base.hpp"
#include "communicator.hpp"

#include "user_interface.hpp"

#define SYS_RESERVED_PORTS 1024

constexpr int QUEUE_LENGTH = 10;
constexpr int THREADS_COUNT = 1;

UserInterface::UserInterface() {
    description_.add_options()
    ("help,h", "Read help message")
    ("client_base,c",
     po::value< std::string >(&client_base_file_path_)->required(),
     "Client base file name")
    ("log,l",
     po::value< std::string >(&log_file_path_)->default_value(defLogFilePath),
     "Log file name")
    ("port,p",
     po::value< int >(&port_)->default_value(defPort),
     "Port number")
    ;
}

bool UserInterface::CheckParams(int argc, char** argv) {
    po::store(po::command_line_parser(argc, argv).
              options(description_).run(), var_map_);

    if (var_map_.count("help") == 1) {
        description_.print(std::cout);
        return false;
    }

    po::notify(var_map_);

    {
        std::ifstream test(client_base_file_path_);
        if (test.fail()) {
            std::cerr << "File '" << client_base_file_path_
                      << "' is unavailable." << std::endl;
            return false;
        }
        std::cout << "Ready to read client base file: "
                  << client_base_file_path_ << std::endl;
    }

    {
        std::ofstream test(log_file_path_, std::ios_base::app);
        if (test.fail()) {
            std::cerr << "File '" << log_file_path_
                      << "' is unavailable." << std::endl;
            return false;
        }
        std::cout << "Ready to write log in: "
                  << log_file_path_ << std::endl;
    }

    if (port_ < 0 ||
            port_ > std::numeric_limits<uint16_t>::max() ||
            port_ <= SYS_RESERVED_PORTS) {
            std::cerr << "Port No " << port_
                      << " is unavailable." << std::endl;
        return false;
    }
    return true;
}

void UserInterface::StartServer(LogPriority limit) {
    LogWriter logger(log_file_path_, limit);
    logger("Start logger", Debug);

    ClientBase cl_base(client_base_file_path_);
    logger("Client base is successfully read.", Debug);

    Communicator comm(port_, QUEUE_LENGTH, cl_base, logger, THREADS_COUNT);
    comm();
}
