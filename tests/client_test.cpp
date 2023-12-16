#include <UnitTest++/UnitTest++.h>

#include <iostream>
#include <string>
#include <vector>

#include "../src/server_exceptions.hpp"
#include "../src/log_writer.hpp"
#include "../src/client_base.hpp"

#include "../src/client.hpp"

struct TestData{
  LogWriter logger{"data/test_vcalc.log", Debug};
  ClientBase cl_base{"data/vcalc.conf"};
  Client client{0, 0, cl_base, logger};
};

SUITE(ClientTests) {
  TEST_FIXTURE(TestData, testZeroVec) {
    std::vector<int32_t> test_vec{0, 0, 0, 0, 0};
    CHECK_EQUAL(0, client.CalculateVector(test_vec));
  }
  TEST_FIXTURE(TestData, testNegativeVec) {
    std::vector<int32_t> test_vec{-10, -15, -1, -6, -10232};
    CHECK_EQUAL(-10264, client.CalculateVector(test_vec));
  }
  TEST_FIXTURE(TestData, testPositiveVec) {
    std::vector<int32_t> test_vec{10, 15, 1, 6, 10232};
    CHECK_EQUAL(10264, client.CalculateVector(test_vec));
  }
  TEST_FIXTURE(TestData, testMixedVec) {
    std::vector<int32_t> test_vec{-10, -15, 10, 15, -5, 7};
    CHECK_EQUAL(2, client.CalculateVector(test_vec));
  }
  TEST_FIXTURE(TestData, testEmptyVec) {
    std::vector<int32_t> test_vec{};
    CHECK_EQUAL(0, client.CalculateVector(test_vec));
  }
  TEST_FIXTURE(TestData, testOverflow) {
    std::vector<int32_t> test_vec{2147483645, 10};
    CHECK_EQUAL(2147483647, client.CalculateVector(test_vec));
  }
  TEST_FIXTURE(TestData, testUnderflow) {
    std::vector<int32_t> test_vec{-2147483640, -10};
    CHECK_EQUAL(-2147483648, client.CalculateVector(test_vec));
  }
}

int main() {
  std::cout << "------- Client Tests -------" << std::endl;
  UnitTest::RunAllTests();
  return 0;
}