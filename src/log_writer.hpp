/**
 * @file log_writer.hpp
 * @author Шурыгин Д.Д.
 * @brief Заголовочный файл определения класса LogWriter
 * @date 2023-12-16
 * @warning Создано только в учебных целях.
 */
#pragma once

#include <string>
#include <mutex>

static std::mutex mt;
/**
 * @brief Набор уровней записей журнала.
 * @details
 * [Debug]  -   Записи, содержащие подробную информацию о действиях.
 * [Info]   -   Записи, содержащие основную информацию о работе сервера.
 * [Warn]   -   Записи, содержищие предупреждения о некорректной работе сервера.
 * [Error]  -   Записи, содержащие информацию о некритических ошибках, возникших при работе с клиентом.
 * [Fatal]  -   Записи, содержащие информацию о критических ошибках, после которых работа сервера прекращается.
 */
enum LogPriority {
    Debug, Info, Warn, Error, Fatal
};

/**
 * @brief Класс для записи информации в журнал работы сервера.
 * @details Пример использования класса для записи критической ошибки
 * @code {.cpp}
 * try {
      // Какой-то код, приводящий к критической ошибке
    } catch(const ServerException& e) {
      logger(e.what(), Fatal);
      exit(EXIT_FAILURE);
    }
 * @endcode
 */
class LogWriter {
  public:
    /**
     * @brief Конструктор для функции записи в жунрал.
     * 
     * @param [in] path Путь к файлу журнала работы сервера.  
     * @param [in] limit Нижняя граница уровней записи (записи, уровень которых ниже, сохраняться не будут).
     */
    LogWriter(const std::string& path, LogPriority limit):log_path_(path), verbosity_(limit){};
    /**
     * @brief Конструктор копирования для функции записи в журнал.
     * 
     * @param [in] other Ссылка на другой объект типа LogWriter.
     */
    LogWriter(const LogWriter& other):log_path_(other.log_path_),verbosity_(other.verbosity_){};

    // Записать в файл строку message, если priority не ниже verbosity_
    /**
     * @brief Запись в журнал работы.
     * @details Записывает только сообщения, уровень которых выше или равен установленному минимуму.
     * 
     * @param [in] message Записываемое сообщение (строка) 
     * @param [in] priority Уровень записи.
     * @return true Сообщение успешно записано, или запись отклонена из-за низкого уровня.
     * @return false Сообщение не записано из-за ошибки открытия файла.
     */
    bool operator()(const std::string& message, LogPriority priority);    
#ifdef UNIT_TEST_SRV
  public:
#else
  private:
#endif
    std::string log_path_;  ///< Путь к файлу с журналом работы
    LogPriority verbosity_; ///< Минимальный уровень выводимых данных

    /**
     * @brief Получить текущее время в строке.
     * 
     * @return std::string Строка с текущим временем в формате "dd/mmm/yyyy:HH:MM:SS".
     */
    std::string GetCurTime();

};
