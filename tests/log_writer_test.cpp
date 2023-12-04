#include <UnitTest++/UnitTest++.h>

#include <iostream>
#include <string>

#include "../src/log_writer.hpp"
#include "../src/server_exceptions.hpp"

LogWriter logger("data/test_vcalc.log", Info);

SUITE( KeyTest ) {
    TEST(testGoodCopy) {
      std::string path = "data/vcalc.log";
      LogPriority limit = Info;

      LogWriter l(path, limit);
      CHECK_EQUAL(&path, &l.log_path_);
    }
    TEST(testCharMsg) {
      CHECK(logger("Message", Info) == true);
    }
    TEST(testStringMsg) {
      std::string message = "Message";
      CHECK(logger(message, Info));
    }
    TEST(testLowPriority) {
      CHECK(logger("Message", Debug));
    }
    TEST(testHighPriority) {
      CHECK(logger("Message", Error));
    }
}

int main(int argc, char **argv) {
    std::cout << "------- Log Writer Tests -------" << std::endl;
    UnitTest::RunAllTests(); // Запуск тестов
    return 0;
}