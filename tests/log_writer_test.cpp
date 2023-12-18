#include <UnitTest++/UnitTest++.h>

#include <iostream>
#include <string>

#include "../src/log_writer.hpp"
#include "../src/server_exceptions.hpp"

struct LogWriterTest {
  LogWriter logger{"data/test_vcalc.log", Info};
};

SUITE( LoggerTest ) {
    TEST_FIXTURE(LogWriterTest, testCharMsg) {
      CHECK(logger("Message", Info));
    }
    TEST_FIXTURE(LogWriterTest, testStringMsg) {
      std::string message = "Message";
      CHECK(logger(message, Info));
    }
    TEST_FIXTURE(LogWriterTest, testLowPriority) {
      CHECK(logger("Message", Debug));
    }
    TEST_FIXTURE(LogWriterTest, testHighPriority) {
      CHECK(logger("Message", Error));
    }
}

int main(int argc, char **argv) {
    std::cout << "------- Log Writer Tests -------" << std::endl;
    UnitTest::RunAllTests(); // Запуск тестов
    return 0;
}