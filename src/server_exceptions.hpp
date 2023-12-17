/**
 * @file server_exceptions.hpp
 * @author Шурыгин Д.Д.
 * @brief Заголовочный файл определения классов исключений.
 * @date 2023-12-16
 * @warning Создано только в учебных целях.
 */
#pragma once

#include <exception>
#include <system_error>
#include <boost/program_options/errors.hpp>

#include <string>

/**
 * @brief Общий класс исключений сервера.
 */
class ServerException : public std::runtime_error {

public:
    ServerException(const char* msg): std::runtime_error(msg){};
    ServerException(const std::string& msg): std::runtime_error(msg){};
};

// 
/**
 * @brief Класс исключений при работе с файлами.
 */
class SysException : public ServerException {

public:
    SysException(const char* msg): ServerException(msg){};
    SysException(const std::string& msg): ServerException(msg){};
};

// 
/**
 * @brief Класс исключений при аутентификации клиента.
 */
class AuthException : public ServerException {

public:
    AuthException(const char* msg): ServerException(msg){};
    AuthException(const std::string& msg): ServerException(msg){};
};

// 
/**
 * @brief Класс исключений при работе с данными от клиента.
 */
class ClientException : public ServerException {

public:
    ClientException(const char* msg): ServerException(msg){};
    ClientException(const std::string& msg): ServerException(msg){};
};