/**
 * @file client_base.hpp
 * @author Шурыгин Д.Д.
 * @brief Заголовочный файл определения класса ClientBase
 * @date 2023-12-16
 * @warning Создано только в учебных целях.
 */
#pragma once

#include <string>
#include <map>

/**
 * @brief Класс для чтения и обработки аутентификационных данных пользователей.
 */
class ClientBase {
public:
    ClientBase() = delete;
    /**
     * @brief Конструктор для обработки данных пользователей.
     * 
     * @param [in] file_path Путь к файлу с данными пользователей.
     * @throw SysException Ошибки при чтении файла с данными.
     * Параметры исключения:
     * @code {.cpp}
     * what = "Failed to open client base file."
     *        "Client base file is empty."
     *        "Client base reading error. Incorrect string format."
     *        "Client base reading error. Duplicate ID."
     * @endcode
     */
    ClientBase(const std::string &file_path);
    /**
     * @brief Конструктор копирования
     * 
     * @param [in] other Ссылка на другой объект типа ClientBase.
     */
    ClientBase(const ClientBase &other):clients_(other.clients_){};

    /**
     * @brief Найти id среди указанных в данных пользователей.
     * @details Если запрошенный id существует - возвращается пароль.
     * 
     * @param [in] login Запрашиваемый id пользователя. 
     * @return std::pair< bool, std::string > Наличие/отсутствие запрашиваемого id.
     * Пароль для запрашиваемого id, если такой существует.
     */
    std::pair< bool, std::string > operator()(const std::string &login);

#ifdef UNIT_TEST_SRV
  public:
#else
  private:
#endif
    /**
     * @brief Ассоциативный массив, связывающий id пользователей с из паролями.
     */
    std::map< std::string, std::string > clients_;
};