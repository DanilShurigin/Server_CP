/**
 * @file log_writer.cpp
 * @author Шурыгин Д.Д.
 * @brief Файл реализации класса LogWriter.
 * @date 2023-12-16
 * @warning Создано только в учебных целях.
 */
#include <fstream>
#include <ctime>
#include <iostream>

#include "server_exceptions.hpp"

#include "log_writer.hpp"

bool LogWriter::operator()(const std::string& message, LogPriority priority) {
    if (priority < verbosity_) {
        return true;
    }

    const std::lock_guard< std::mutex > lock(mt);

    // std::ofstream log_file(log_path_, std::ios_base::app);
    // if( log_file.fail() ) {
    //     std::cerr << "Failed to write on log file." << std::endl;
    //     return false;
    // }

    // log_file << GetCurTime() << '\t';
    std::string log_entry;

    log_entry += GetCurTime() + '\t';

    switch( priority ) {
    case Debug:
        // log_file << "[Debug]\t";
        log_entry += "[Debug]\t";
        break;
    case Info:
        // log_file << "[Info]\t";
        log_entry += "[Info]\t";
        break;
    case Warn:
        // log_file << "[Warn]\t";
        log_entry += "[Warn]\t";
        break;
    case Error:
        // log_file << "[Error]\t";
        log_entry += "[Error]\t";
        break;
    case Fatal:
        // log_file << "[Fatal]\t";
        log_entry += "[Fatal]\t";
        break;
    }

    // log_file << message << std::endl << std::flush;
    log_entry += message;

    std::ofstream log_file(log_path_, std::ios_base::app);
    if( log_file.fail() ) {
        std::cerr << "Failed to write on log file. | "
                  << log_entry << std::endl << std::flush;
        return false;
    }

    log_file << log_entry << std::endl << std::flush;

    log_file.close();
    return true;
}

std::string LogWriter::GetCurTime() {
    std::time_t cur_time = std::time(nullptr);
    size_t buf_size = sizeof("dd/mmm/yyyy:HH:MM:SS");
    char buf[buf_size];
    std::strftime(buf, buf_size, "%d/%b/%Y:%H:%M:%S", std::localtime(&cur_time));
    return std::string(buf);
}
