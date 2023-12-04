#include <UnitTest++/UnitTest++.h>

#include <iostream>
#include <string>
#include <vector>

#include "../src/user_interface.hpp"
#include "../src/log_writer.hpp"
#include "../src/server_exceptions.hpp"

struct BaseUi {
    UserInterface ui;
};

SUITE( KeyTest ) {
    TEST_FIXTURE(BaseUi, testComLineEmpty) {
        int test_argc = 1;
        const char* test_argv[test_argc] = {"NAME"};
        CHECK_THROW(ui.CheckParams(test_argc, const_cast<char**>(test_argv)), po::error);
    }
    TEST_FIXTURE(BaseUi, testComLineStrangeComLineParam) {
        int test_argc = 3;
        const char* test_argv[test_argc] = {"NAME", "-k", "123"};
        CHECK_THROW(ui.CheckParams(test_argc, const_cast<char**>(test_argv)), po::error);
    }
    TEST_FIXTURE(BaseUi, testComLineHelpOnly) {
        int test_argc = 2;
        const char* test_argv[test_argc] = {"NAME", "--help"};
        CHECK(ui.CheckParams(test_argc, const_cast<char**>(test_argv)) == false);
    }
    TEST_FIXTURE(BaseUi, testComLineHelpNotOnly) {
        int test_argc = 4;
        const char* test_argv[test_argc] = {"NAME", "-c", "way_to_hell", "-h"};
        CHECK(ui.CheckParams(test_argc, const_cast<char**>(test_argv)) == false);
    }
    TEST_FIXTURE(BaseUi, testComLineBrokenLogFilePath) {
        int test_argc = 5;
        const char* test_argv[test_argc] = {"NAME", "-c", "data/vcalc.conf", "-l", "/vcalc.log"};
        CHECK(ui.CheckParams(test_argc, const_cast<char**>(test_argv)) == false);
    }
    TEST_FIXTURE(BaseUi, testComLineBrokenClientBaseFilePath) {
        int test_argc = 3;
        const char* test_argv[test_argc] = {"NAME", "-c", "data/vcalc.txt"};
        CHECK(ui.CheckParams(test_argc, const_cast<char**>(test_argv)) == false);
    }
    TEST_FIXTURE(BaseUi, testComLineBrokenPortNumberMinus) {
        int test_argc = 5;
        const char* test_argv[test_argc] = {"NAME", "-c", "data/vcalc.conf", "-l", "data/vcalc.log", "-p", "0"};
        CHECK(ui.CheckParams(test_argc, const_cast<char**>(test_argv)) == false);
    }
    TEST_FIXTURE(BaseUi, testComLineBrokentPortOverMax) {
        int test_argc = 5;
        const char* test_argv[test_argc] = {"NAME", "-c", "data/vcalc.conf", "-l", "data/vcalc.log", "-p", "70000"};
        CHECK(ui.CheckParams(test_argc, const_cast<char**>(test_argv)) == false);
    }
    
}

int main(int argc, char **argv) {
    std::cout << "------- User Interface Tests -------" << std::endl;
    UnitTest::RunAllTests(); // Запуск тестов
    return 0;
}