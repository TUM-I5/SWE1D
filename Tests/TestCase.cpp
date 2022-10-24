#include <catch2/catch_test_macros.hpp>

static int factorial(int number) {
  // return number <= 1 ? number : factorial(number - 1) * number; // fail
  return number <= 1 ? 1 : factorial(number - 1) * number; // pass
}

TEST_CASE("Factorial of 0 is 1 (fail)", "[single-file]") { REQUIRE(factorial(0) == 1); }

TEST_CASE("Factorials of 1 and higher are computed (pass)", "[single-file]") {
  REQUIRE(factorial(1) == 1);
  REQUIRE(factorial(2) == 2);
  REQUIRE(factorial(3) == 6);
  REQUIRE(factorial(10) == 3628800);
}

// Expected compact output (all assertions):
//
// prompt> 010-TestCase --reporter compact --success
// 010-TestCase.cpp:14: failed: Factorial(0) == 1 for: 0 == 1
// 010-TestCase.cpp:18: passed: Factorial(1) == 1 for: 1 == 1
// 010-TestCase.cpp:19: passed: Factorial(2) == 2 for: 2 == 2
// 010-TestCase.cpp:20: passed: Factorial(3) == 6 for: 6 == 6
// 010-TestCase.cpp:21: passed: Factorial(10) == 3628800 for: 3628800 (0x375f00) == 3628800 (0x375f00)
// Failed 1 test case, failed 1 assertion.
