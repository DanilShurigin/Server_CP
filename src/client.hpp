/**
 * @file client.hpp
 * @author Шурыгин Д.Д.
 * @brief Заголовочный файл определения класса Client.
 * @date 2023-12-16
 * @warning Создано только в учебных целях.
 */
#pragma once

#include <string>
#include <vector>

#include "client_base.hpp"
#include "log_writer.hpp"

/**
 * @brief Класс для обработки клиента.
 * @details Предоставляет набор функций, необходимый для обработки запроса клиента
 * (аутентификация, приём данных, калькулятор для вычислений).
 */
class Client {
  public:
    Client() = delete;
    /**
     * @brief Конструктор для функций обработки клиента.
     * 
     * @param [in] sock Сокет, через который сервер взаимодействует с клиентом.
     * @param [in] port Порт клиента.
     * @param [in] viewer Ссылка на объект ClientBase.
     * @param [in] logger Ссылка на объект LogWriter.
     */
    Client(int sock, uint16_t port, ClientBase& viewer, LogWriter& logger):
            work_socket_(sock), port_(port), cb_viewer_(viewer), logger_(logger){};

    /**
     * @brief Аутентифицировать клиента.
     * @throw AuthException Ошибка при аутентификации.
     * Параметры исключения:
     * @code {.cpp}
     * what = "Failed to recieve ID."
     *        "ID doesn`t exist in the client base."
     *        "Failed to send SALT."
     *        "Failed to recieve HASH."
     *        "Authentification error."
     *        "Failed to send OK."
     * @endcode
     */
    void Authentificate();
    /**
     * @brief Обработать присылаемые векторы чисел.
     * @details Принимает количество обрабатываемых векторов и
     * запускает их последовательную обработку - получить, вычислить, отправить результат.
     * 
     * @throw CalcException Ошибки при взаимодействии с клиентом.
     * Параметры исключения:
     * @code {.cpp}
     * what = "Failed to receive vectors count."
     *        "F"ailed to send result."
     * @endcode
     * 
     */
    void Calculate();
#ifdef UNIT_TEST_SRV
  public:
#else
  private:
#endif
    int work_socket_;         ///< окет, через который сервер взаимодействует с клиентом.
    uint16_t port_;           ///< Порт клиента.
    ClientBase& cb_viewer_;   ///< Ссылка на объект с аутентификационными данными пользователей.
    LogWriter& logger_;       ///< Ссылка на объект, записывающий в журнал работы.

    // Обработка присылаемых векторов
    /**
     * @brief Обработать вектор.
     * @details Производит вычисления на основе данных верктора (сложение всех чисел).
     * 
     * @param [in] vec Вектор с числами типа int32_t 
     * @return int32_t Сумма всех чисел в векторе или максимальное|минимальное
     * значение для случаев переполнения.
     */
    int32_t CalculateVector(const std::vector<int32_t>& vec);
    /**
     * @brief Принять строку от клиента.
     * @details Используется метод удвоения в цикле.
     * 
     * @return std::pair<bool, std::string> Успех/неудача при получении строки.
     * Полученная строка в случае успеха.
     */
    std::pair<bool, std::string> RecvMessage();
    /**
     * @brief Принять данные вектора от клиента.
     * 
     * @return std::vector<int32_t> Вектор чисел, полученный от клиента.
     * @throw CalcException Ошибка при получении данных.
     * Праметры исключения:
     * @code {.cpp}
     * what = "Failed to receive size of vector."
     *        "Failed to receive vector."
     *        "Failed to receive vector. Size error."
     * @endcode
     */
    std::vector<int32_t> RecvVector();
};
