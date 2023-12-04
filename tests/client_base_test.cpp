#include <UnitTest++/UnitTest++.h>

#include <iostream>
#include <string>
#include <map>

#include "../src/client_base.hpp"
#include "../src/server_exceptions.hpp"


SUITE( KeyTest ) {
    TEST(testGoodCopy) {
      std::string path = "data/vcalc.conf";

      ClientBase c(path);
      ClientBase c1(c);
      CHECK(&c == &c1);
    }
    
}

int main(int argc, char **argv) {
    std::cout << "------- Client Base Tests -------" << std::endl;
    UnitTest::RunAllTests(); // Запуск тестов
    return 0;
}