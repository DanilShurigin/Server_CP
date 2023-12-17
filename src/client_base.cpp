/**
 * @file client_base.cpp
 * @author Шурыгин Д.Д.
 * @brief Файл реализации класса ClientBase.
 * @date 2023-12-16
 * @warning Создано только в учебных целях.
 */
#include <fstream>
#include <utility>

#include "server_exceptions.hpp"

#include "client_base.hpp"

ClientBase::ClientBase(const std::string &file_path) {
    std::ifstream in_file(file_path);

    if( in_file.fail() ) {
        throw SysException("Failed to open client base file.");
    }
    if (in_file.peek() == std::fstream::traits_type::eof()) {
        throw SysException("Client base file is empty.");
    }

    std::string line;
    int line_index = 0;
    while( std::getline(in_file, line) ) {
        line_index++;

        size_t sep_index = line.find_first_of(':');

        if (sep_index == std::string::npos) {
            throw SysException("Client base reading error. Incorrect string format. Line " + std::to_string(line_index));
        }

        std::string login = line.substr(0, sep_index);
        std::string password = line.substr(sep_index+1, sizeof(line));

        if( auto status = clients_.insert({login, password}); status.second == false ) {
            throw SysException("Client base reading error. Duplicate ID. Line " + std::to_string(line_index));
        }
    }
    in_file.close();
}

std::pair< bool, std::string > ClientBase::operator()(const std::string &login) {
    auto it = clients_.find(login);

    if( it != clients_.end() ) {
        return std::make_pair(true, it->second);
    }

    return std::make_pair(false, "");
}