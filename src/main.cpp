#include <iostream>

#include "server_exceptions.hpp"
#include "log_writer.hpp"
#include "user_interface.hpp"

int main(int argc, char **argv) {
    try {
        UserInterface ui;
        if (!ui.CheckParams(argc, argv)) {
            exit(EXIT_FAILURE);
        }
        ui.StartServer(Debug);
    } catch(const po::error& e) {
        std::cerr << e.what() << std::endl;
        std::cerr << "Use -h for help." << std::endl;
        exit(EXIT_FAILURE);
    } catch(const ServerException& e) {
        std::cerr << e.what() << std::endl;
        exit(EXIT_FAILURE);
    } catch(...) {
        std::cerr << "Unknown error" << std::endl;
        exit(EXIT_FAILURE);
    }
    return 0;
}