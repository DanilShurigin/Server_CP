#pragma once

#include <string>
#include <vector>

class ClientBase;
class LogWriter;

class Client {
#ifdef UNIT_TEST_SRV
  public:
#else
  private:
#endif
    int work_socket_;               // Рабочий сокет
    uint16_t port_;                 // Номер порта
    ClientBase& cb_viewer_;   // Аутентификационные данные пользователей
    LogWriter& logger_;             // Записывать логи

    // Обработка присылаемых векторов
    int32_t CalculateVector(const std::vector<int32_t>& vec);
    // Принять сообщение-строку
    std::pair<bool, std::string> RecvMessage();
    std::vector<int32_t> RecvVector();
    void Authentificate();
    void Calculate();
  public:
    Client() = delete;
    Client(int sock, uint16_t port, ClientBase& viewer, LogWriter& logger):
            work_socket_(sock), port_(port), cb_viewer_(viewer), logger_(logger){};

    // Обработать клиента
    void Serve();
};
