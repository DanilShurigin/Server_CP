#include <UnitTest++/UnitTest++.h>

#include <iostream>
#include <string>
#include <map>

#include "../src/client_base.hpp"
#include "../src/server_exceptions.hpp"

struct ClientBaseTest {
  ClientBase cl_base{"data/vcalc.conf"};
};



SUITE( KeyTest ) {
    //
}

int main(int argc, char **argv) {
    std::cout << "------- Client Base Tests -------" << std::endl;
    UnitTest::RunAllTests(); // Запуск тестов
    return 0;
}