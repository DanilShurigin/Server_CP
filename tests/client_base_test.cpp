#include <UnitTest++/UnitTest++.h>

#include <iostream>
#include <string>
#include <map>

#include "../src/client_base.hpp"
#include "../src/server_exceptions.hpp"

SUITE( ClientBaseTests ) {
    TEST(testEmptyFile) {
      CHECK_THROW(ClientBase("data/vcalc_test_1_1.conf"), SysException);
    }
    TEST(testEmptyLine) {
      CHECK_THROW(ClientBase("data/vcalc_test_1_2.conf"), SysException);
    }
    TEST(testBadLine) {
      CHECK_THROW(ClientBase("data/vcalc_test_1_3.conf"), SysException);
    }
    TEST(testDuplicateID) {
      CHECK_THROW(ClientBase("data/vcalc_test_1_4.conf"), SysException);
    }
    TEST(testGoodFile) {
      std::map<std::string,std::string> test_map{{"user", "P@ssW0rd"}, {"test", "pass"}};
      CHECK(test_map == ClientBase("data/vcalc_test_2_1.conf").clients_);
    }
    TEST(testEmptyID) {
      std::map<std::string,std::string> test_map{{"user", "P@ssW0rd"}, {"", "pass"}};
      CHECK(test_map == ClientBase("data/vcalc_test_2_2.conf").clients_);
    }
    TEST(testEmptyPassword) {
      std::map<std::string,std::string> test_map{{"user", "P@ssW0rd"}, {"test", ""}};
      CHECK(test_map == ClientBase("data/vcalc_test_2_3.conf").clients_);
    }
    TEST(testEmptyIDPassword) {
      std::map<std::string,std::string> test_map{{"user", "P@ssW0rd"}, {"", ""}};
      CHECK(test_map == ClientBase("data/vcalc_test_2_4.conf").clients_);
    }
}

int main(int argc, char **argv) {
    std::cout << "------- Client Base Tests -------" << std::endl;
    UnitTest::RunAllTests(); // Запуск тестов
    return 0;
}