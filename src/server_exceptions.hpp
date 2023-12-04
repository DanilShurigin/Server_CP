#pragma once

#include <exception>
#include <system_error>
#include <boost/program_options/errors.hpp>

#include <string>

// Общий класс исключений сервера
class ServerException : public std::runtime_error {

public:
    ServerException(const char* msg): std::runtime_error(msg){};
    ServerException(const std::string& msg): std::runtime_error(msg){};

};

// Исключения при обработке командной строки
class ComLineException : public boost::program_options::error  {
public:
    ComLineException(const char* msg): boost::program_options::error(msg){};
};

// Исключения при работе с файлами
class SysException : public ServerException {

public:
    SysException(const char* msg): ServerException(msg){};
    SysException(const std::string& msg): ServerException(msg){};
};

// Исключения при аутентификации клиента
class AuthException : public ServerException {

public:
    AuthException(const char* msg): ServerException(msg){};
    AuthException(const std::string& msg): ServerException(msg){};
};

// Исключения при работе с данными от клиента
class CalcException : public ServerException {

public:
    CalcException(const char* msg): ServerException(msg){};
    CalcException(const std::string& msg): ServerException(msg){};
};