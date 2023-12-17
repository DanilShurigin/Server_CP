/**
 * @file communicator.hpp
 * @author Шурыгин Д.Д.
 * @brief Заголовочный файл определения класса Communicator.
 * @date 2023-12-16
 * @warning Создано только в учебных целях.
 */
#pragma once

#include <netinet/in.h>
#include <unistd.h>
#include <memory>
#include <thread>
#include <atomic>

#include "client_base.hpp"
#include "log_writer.hpp"

/**
 * @brief Класс для управления сетевым взаимодействием.
 * @details Создаёт сокет и закрепляет его.
 * Запускает режим прослушивания для приёма запросов от клиентов.
 * Принимает соединение с клиентами.
 */
class Communicator {
  public:
    Communicator() = delete;
    /**
     * @brief Конструктор для создания и запуска сетевой части сервера.
     * 
     * @param [in] port Порт, на котором будет работать сервер.
     * @param [in] queue_length Количество клиентов, которые могут одновременно ожидать подключения.
     * @param [in] viewer Ссылка на объект ClientBase.
     * @param [in] logger Ссылка на объект LogWriter.
     * @param [in] max_threads Максимальное число потоков, в которых будут обрабатываться клиенты.
     * 
     * @throw ServerException Ошибки при запуске сетевых функций.
     * Параметры исключений:
     * @code {.cpp}
     * what = "Failed to create socket."
     *        "Failed to set socket options (SO_REUSEADDR)."
     *        "Failed to bind socket."
     *        "Failed to start listen."
     * @endcode
     */
    Communicator(int port, int queue_length, ClientBase& viewer, LogWriter& logger, int max_threads=1);

    // Приём запросов от клиентов
    /**
     * @brief Приём запросов от клиентов.
     * @details В однопоточном режиме принимает запрос следующего в очереди клиента
     * после обработки предыдущего.
     * В многопоточном режиме принимает новые запросы, если свободен хотя бы один поток.
     * 
     */
    void operator()();

#ifdef UNIT_TEST_SRV
  public:
#else
  private:
#endif
    int listen_socket_;                             ///< Сокет сервера.
    int queue_length_;                              ///< Длина очереди клиентов.
    std::unique_ptr< sockaddr_in > self_addr_;      ///< Данные сервера.
    std::unique_ptr< sockaddr_in > remote_addr_;    ///< Данные клиента.

    ClientBase& client_base_;       ///< Ссылка на объект с аутентификационными данными пользователей.
    LogWriter& logger_;             ///< Ссылка на объект, записывающий в журнал работы.

    // 
    unsigned int max_threads_;      ///< Максимальное количество рабочих потоков.
    std::atomic_uint threads_ = 0;  ///< Текущее количество занятых потоков.

    // Обработка клиента
    /**
     * @brief Обработать клиента.
     * @details Запускает обработку запроса клиента.
     * 
     * @param [in] work_socket Сокет, через который происходит взаимодействие сервера и клиента.
     * @param [in] port Порт, к которому привязан клиент.
     */
    void ServeClient(int work_socket, uint16_t port);

};