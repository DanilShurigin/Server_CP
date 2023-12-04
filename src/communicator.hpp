#pragma once

#include <netinet/in.h>
#include <unistd.h>
#include <memory>
#include <thread>
#include <atomic>

class ClientBase;
class LogWriter;

class Communicator {
  public:
    Communicator() = delete;
    Communicator(int port, int queue_length, ClientBase& viewer, LogWriter& logger, int max_threads=1);

    // Приём запросов от клиентов
    void operator()();

#ifdef UNIT_TEST_SRV
  public:
#else
  private:
#endif
    int listen_socket_;                             // Сокет сервера
    int queue_length_;                              // Длина очереди клиентов
    std::unique_ptr< sockaddr_in > self_addr_;      // Данные сервера
    std::unique_ptr< sockaddr_in > remote_addr_;    // Данные клиента

    ClientBase& client_base_; // Аутентификационные данные пользователей
    LogWriter& logger_;             // Записывать логи

    // Максимальное доступное число потоков
    const unsigned int threads_limit_ = std::thread::hardware_concurrency()-1;
    // Установленное при создании число потоков
    unsigned int max_threads_;
    // Текущая заполненность потоков
    std::atomic_uint threads_ = 0;

    // Обработка клиента
    void ServeClient(int work_socket, uint16_t port);

};