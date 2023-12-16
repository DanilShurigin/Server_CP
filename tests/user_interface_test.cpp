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

SUITE( ComLineTests ) {
    TEST_FIXTURE(BaseUi, testEmpty) {
        int test_argc = 1;
        const char* test_argv[test_argc] = {"NAME"};
        CHECK_THROW(ui.CheckParams(test_argc, const_cast<char**>(test_argv)), po::error);
    }
    TEST_FIXTURE(BaseUi, testOnlyHelp) {
        int test_argc = 2;
        const char* test_argv[test_argc] = {"NAME", "-h"};
        CHECK_EQUAL(false,
                    ui.CheckParams(test_argc, const_cast<char**>(test_argv)));
    }
    TEST_FIXTURE(BaseUi, testHelp) {
        int test_argc = 6;
        const char* test_argv[test_argc] = {"NAME", "-c", "data/vcalc.conf", "-l", "data/vcalc.log", "-h"};
        CHECK_EQUAL(true,
                    ui.CheckParams(test_argc, const_cast<char**>(test_argv)));
    }
    TEST_FIXTURE(BaseUi, testOnlyUnknownParam) {
        int test_argc = 2;
        const char* test_argv[test_argc] = {"NAME", "-k"};
        CHECK_THROW(ui.CheckParams(test_argc, const_cast<char**>(test_argv)), po::error);
    }
    TEST_FIXTURE(BaseUi, testUnknownParam) {
        int test_argc = 6;
        const char* test_argv[test_argc] = {"NAME", "-c", "data/vcalc.conf", "-l", "data/vcalc.log", "-k"};
        CHECK_THROW(ui.CheckParams(test_argc, const_cast<char**>(test_argv)), po::error);
    }
    TEST_FIXTURE(BaseUi, testUnknownClientBaseFile) {
        int test_argc = 5;
        const char* test_argv[test_argc] = {"NAME", "-c", "data/vcalc.txt", "-l", "data/vcalc.log"};
        CHECK_EQUAL(false,
                    ui.CheckParams(test_argc, const_cast<char**>(test_argv)));
    }
    TEST_FIXTURE(BaseUi, testUnableClientBaseFile) {
        int test_argc = 5;
        const char* test_argv[test_argc] = {"NAME", "-c", "/vcalc.conf", "-l", "data/vcalc.log"};
        CHECK_EQUAL(false,
                    ui.CheckParams(test_argc, const_cast<char**>(test_argv)));
    }
    TEST_FIXTURE(BaseUi, testUnableLogFile) {
        int test_argc = 5;
        const char* test_argv[test_argc] = {"NAME", "-c", "data/vcalc.conf", "-l", "/vcalc.log"};
        CHECK_EQUAL(false,
                    ui.CheckParams(test_argc, const_cast<char**>(test_argv)));
    }
    TEST_FIXTURE(BaseUi, testUnknownLogFilePath) {
        int test_argc = 5;
        const char* test_argv[test_argc] = {"NAME", "-c", "data/vcalc.conf", "-l", "data/log/vcalc.log"};
        CHECK_EQUAL(false,
                    ui.CheckParams(test_argc, const_cast<char**>(test_argv)));
    }
    TEST_FIXTURE(BaseUi, testSysPort) {
        int test_argc = 7;
        const char* test_argv[test_argc] = {"NAME", "-c", "data/vcalc.conf", "-l", "data/log/vcalc.log", "-p", "102"};
        CHECK_EQUAL(false,
                    ui.CheckParams(test_argc, const_cast<char**>(test_argv)));
    }
    TEST_FIXTURE(BaseUi, testBadPort) {
        int test_argc = 7;
        const char* test_argv[test_argc] = {"NAME", "-c", "data/vcalc.conf", "-l", "data/log/vcalc.log", "-p", "70000"};
        CHECK_EQUAL(false,
                    ui.CheckParams(test_argc, const_cast<char**>(test_argv)));
    }
    TEST_FIXTURE(BaseUi, testSudoStart) {
        int test_argc = 3;
        const char* test_argv[test_argc] = {"NAME", "-c", "data/vcalc.conf"};
        CHECK_EQUAL(false,
                    ui.CheckParams(test_argc, const_cast<char**>(test_argv)));
    }
    TEST_FIXTURE(BaseUi, testAllStart) {
        int test_argc = 7;
        const char* test_argv[test_argc] = {"NAME", "-c", "data/vcalc.conf", "-l", "data/log/vcalc.log", "-p", "33333"};
        CHECK_EQUAL(false,
                    ui.CheckParams(test_argc, const_cast<char**>(test_argv)));
    }
}

int main(int argc, char **argv) {
    std::cout << "------- User Interface Tests -------" << std::endl;
    UnitTest::RunAllTests(); // Запуск тестов
    return 0;
}