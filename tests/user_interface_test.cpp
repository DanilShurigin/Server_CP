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
    TEST_FIXTURE(BaseUi, testComLineOnlyHelp) {
        int test_argc = 2;
        const char* test_argv[test_argc] = {"NAME", "-h"};
        ui.CheckParams(test_argc, const_cast<char**>(test_argv));
        CHECK(true);
    }
    TEST_FIXTURE(BaseUi, testComLineHelp) {
        int test_argc = 6;
        const char* test_argv[test_argc] = {"NAME", "-c", "data/vcalc.conf", "-l", "data/vcalc.log", "-h"};
        ui.CheckParams(test_argc, const_cast<char**>(test_argv));
        CHECK(true);
    }
    TEST_FIXTURE(BaseUi, testComLineUnknownOnly) {
        int test_argc = 2;
        const char* test_argv[test_argc] = {"NAME", "-k"};
        CHECK_THROW(ui.CheckParams(test_argc, const_cast<char**>(test_argv)), po::error);
    }
    TEST_FIXTURE(BaseUi, testComLineUnknown) {
        int test_argc = 6;
        const char* test_argv[test_argc] = {"NAME", "-c", "data/vcalc.conf", "-l", "data/vcalc.log", "-k"};
        CHECK_THROW(ui.CheckParams(test_argc, const_cast<char**>(test_argv)), po::error);
    }
    TEST_FIXTURE(BaseUi, testComLineBadClientFilePath) {
        int test_argc = 5;
        const char* test_argv[test_argc] = {"NAME", "-c", "data/vcalc.txt", "-l", "data/vcalc.log"};
        CHECK(ui.CheckParams(test_argc, const_cast<char**>(test_argv)) == false);
    }
    TEST_FIXTURE(BaseUi, testComLineUnableClientFilePath) {
        int test_argc = 5;
        const char* test_argv[test_argc] = {"NAME", "-c", "/vcalc.conf", "-l", "data/vcalc.log"};
        CHECK(ui.CheckParams(test_argc, const_cast<char**>(test_argv)) == false);
    }
    TEST_FIXTURE(BaseUi, testComLineUnableLogFilePath) {
        int test_argc = 3;
        const char* test_argv[test_argc] = {"NAME", "-c", "data/vcalc.conf"};
        CHECK(ui.CheckParams(test_argc, const_cast<char**>(test_argv)) == false);
    }
    TEST_FIXTURE(BaseUi, testComLineUnknownLogFilePath) {
        int test_argc = 5;
        const char* test_argv[test_argc] = {"NAME", "-c", "data/vcalc.conf", "-l", "data/log/vcalc.log"};
        CHECK(ui.CheckParams(test_argc, const_cast<char**>(test_argv)) == false);
    }
    TEST_FIXTURE(BaseUi, testComLineSysPort) {
        int test_argc = 7;
        const char* test_argv[test_argc] = {"NAME", "-c", "data/vcalc.conf", "-l", "data/log/vcalc.log", "-p", "102"};
        CHECK(ui.CheckParams(test_argc, const_cast<char**>(test_argv)) == false);
    }
    TEST_FIXTURE(BaseUi, testComLineBadPort) {
        int test_argc = 7;
        const char* test_argv[test_argc] = {"NAME", "-c", "data/vcalc.conf", "-l", "data/log/vcalc.log", "-p", "70000"};
        CHECK(ui.CheckParams(test_argc, const_cast<char**>(test_argv)) == false);
    }
    TEST_FIXTURE(BaseUi, testComLineSudoStart) {
        int test_argc = 3;
        const char* test_argv[test_argc] = {"NAME", "-c", "data/vcalc.conf"};
        CHECK(ui.CheckParams(test_argc, const_cast<char**>(test_argv)) == false);
    }
    TEST_FIXTURE(BaseUi, testComLineAllStart) {
        int test_argc = 7;
        const char* test_argv[test_argc] = {"NAME", "-c", "data/vcalc.conf", "-l", "data/log/vcalc.log", "-p", "33333"};
        CHECK(ui.CheckParams(test_argc, const_cast<char**>(test_argv)) == false);
    }
}

int main(int argc, char **argv) {
    std::cout << "------- User Interface Tests -------" << std::endl;
    UnitTest::RunAllTests(); // Запуск тестов
    return 0;
}