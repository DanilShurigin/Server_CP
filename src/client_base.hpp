#pragma once

#include <string>
#include <map>

class ClientBase {
public:
    ClientBase() = delete;
    ClientBase(const std::string &file_path);
    ClientBase(const ClientBase &other):clients_(other.clients_){};

    // Поиск id в списке. Возвращает пароль.
    std::pair< bool, std::string > operator()(const std::string &login);

#ifdef UNIT_TEST_SRV
  public:
#else
  private:
#endif
    // Список < id : password >
    std::map< std::string, std::string > clients_;
};