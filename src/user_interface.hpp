/**
 * @file user_interface.hpp
 * @author Шурыгин Д.Д.
 * @brief Заголовочный файл определения класса UserInterface
 * @date 2023-12-16
 * @warning Создано только в учебных целях.
 */
#pragma once

#include <boost/program_options.hpp>
#include "log_writer.hpp"

namespace po = boost::program_options;

/**
 * @brief Класс для обработки командной строки.
 */
class UserInterface {
  public:
    /**
     * @brief Конструктор без параметров.
     * Создаёт список параметров командной строки.
     * @details
     * --help,-h        -   Получение справки
     * --client_base,-c -   Путь к файлу с данными пользователей
     * --log,-l         -   Путь к фалу журнала работы сервера (по умолчанию: /var/log/vcalc.log)
     * --port,-p        -   Порт, на котором будет запущен сервер (по умолчанию: 33333)
     * --threads,-t     -   Количество потоков для обработки клиентов (по умолчанию: 1)
     */
    UserInterface();
    /**
     * @brief 
     * 
     * @param [in] argc Количество указанных в командной строке параметров.
     * @param [in] argv Список указателей на строки параметров.
     * @return true Все параметры валидны.
     * @return false В процессе обработки была выявлена ошибка, или выведена справка. 
     */
    bool CheckParams(int argc, char** argv);
    /**
     * @brief Запускает процесс создания сервера.
     * 
     * @param [in] verbosity Определяет многословность журнала (какие именно записи будут сохраняться). 
     */
    void StartServer(LogPriority verbosity);
    
#ifdef UNIT_TEST_SRV
  public:
#else
  private:
#endif
    
    po::options_description description_; ///< Описание параметров командой строки.
    /**
     * @brief Ассоциативный массив, связывающий параметры командной строки с их значениями.
     */
    po::variables_map var_map_;

    const unsigned int defPort = 33333;                       ///< Значение по умолчанию для порта.
    const std::string defLogFilePath = "/var/log/vcalc.log";  ///< Значение по умолчанию для пути к фалу журнала.
    const unsigned int defThreadsCount = 1;                   ///< Значение по умолчанию для количества потоков.

    std::string client_base_file_path_; ///< Путь к файлу с данными пользователей
    std::string log_file_path_;         ///< Путь к файлу журнала работы сервера
    unsigned int port_;                 ///< Порт, на котором будет запущен сервер
    unsigned int threads_;              ///< Количество потоков для обработки клиентов
};