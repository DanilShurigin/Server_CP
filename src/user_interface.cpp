/**
 * @file user_interface.cpp
 * @author Шурыгин Д.Д.
 * @brief Файл реализации класса UserInterface.
 * @date 2023-12-16
 * @warning Создано только в учебных целях.
 */
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
     po::value< unsigned int >(&port_)->default_value(defPort),
     "Port number")
    ("threads,t",
     po::value< unsigned int >(&threads_)->default_value(defThreadsCount),
     "Threads count")
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
    }

    {
        std::ofstream test(log_file_path_, std::ios_base::app);
        if (test.fail()) {
            std::cerr << "File '" << log_file_path_
                      << "' is unavailable." << std::endl;
            return false;
        }
    }

    if (port_ < 0 ||
        port_ > std::numeric_limits<uint16_t>::max() ||
        port_ <= SYS_RESERVED_PORTS) {
        std::cerr << "Port No " << port_
                  << " is unavailable." << std::endl;
        return false;
    }

    if (threads_ < 1 || threads_ > std::thread::hardware_concurrency()) {
        std::cout << "Number of threads '"
                  << threads_
                  << "' is unavailable." << std::endl;
        return false;
    }
    return true;
}

void UserInterface::StartServer(LogPriority limit) {
    LogWriter logger(log_file_path_, limit);
    logger("Start logger", Debug);
    
    try {
        ClientBase cl_base(client_base_file_path_);
        logger("Client base is successfully read.", Debug);

        Communicator comm(port_, QUEUE_LENGTH, cl_base, logger, threads_);
        logger("Server is running", Info);

        comm();
    } catch(const ServerException& e) {
        logger(e.what(), Fatal);
        throw e;
    }
}
