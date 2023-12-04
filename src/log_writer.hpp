#pragma once

#include <string>
#include <mutex>

static std::mutex mt;

enum LogPriority {
    Debug, Info, Warn, Error, Fatal
};

class LogWriter {
  public:
    LogWriter(const std::string& path, LogPriority limit):log_path_(path), verbosity_(limit){};
    LogWriter(const LogWriter& other):log_path_(other.log_path_),verbosity_(other.verbosity_){};

    // Записать в файл строку message, если priority не ниже verbosity_
    bool operator()(const std::string& message, LogPriority priority);    
#ifdef UNIT_TEST_SRV
  public:
#else
  private:
#endif
    std::string log_path_;  // Путь к файлу с журналом работы
    LogPriority verbosity_; // Граница уровня выводимых данных

    // Получить текущее время в форме строки
    std::string GetCurTime();

};
