/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */

#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include <cerrno>

#include "RwUtility.h"

using namespace remote_wiring;

void * callback_context = nullptr;

void
callback (
    void * context_
) {
    (void)context_;
    return;
}

class TestClass {
  public:
    bool testMethod_invoked;
    signal_t testMethod_uponComplete_arg;
    void * testMethod_context_arg;
    bool testMethod_should_hang;
    int testMethod_result;

    TestClass (
        void
    ) :
        testMethod_invoked(false),
        testMethod_uponComplete_arg(nullptr),
        testMethod_context_arg(nullptr),
        testMethod_should_hang(false),
        testMethod_result(0)
    {}

    int
    testMethod (
        signal_t uponComplete_,
        void * context_
    ) {
        testMethod_invoked = true;
        testMethod_uponComplete_arg = uponComplete_;
        testMethod_context_arg = context_;
        if ( uponComplete_ && !testMethod_should_hang ) { uponComplete_(context_); }
        return testMethod_result;
    }
};

TEST_CASE("utility::invoke_blockable_async_method - `nullptr` in `object_` argument results in error", "[utility::invoke_blockable_async_method]") {
    // This requirement is confirmed by the compiler do to the templated reference type.
}

TEST_CASE("utility::invoke_blockable_async_method - An `object_` argument not of templated type results in error", "[utility::invoke_blockable_async_method]") {
    // This requirement is confirmed by the compiler do to the templated reference type.
}

TEST_CASE("utility::invoke_blockable_async_method - `nullptr` in `method_` argument results in error", "[utility::invoke_blockable_async_method]") {
    // This requirement is confirmed by the compiler do to the function signature.
}

TEST_CASE("utility::invoke_blockable_async_method - A `method_` argument not a member function of templated type results in error", "[utility::invoke_blockable_async_method]") {
    // This requirement is confirmed by the compiler do to the function signature.
}

TEST_CASE("utility::invoke_blockable_async_method - Invokes the provided method", "[utility::invoke_blockable_async_method]") {
    TestClass test;
    utility::invoke_blockable_async_method(test, &TestClass::testMethod, nullptr, nullptr);
    REQUIRE(test.testMethod_invoked);
}

TEST_CASE("utility::invoke_blockable_async_method - Supplies `uponComplete` to the provided method", "[utility::invoke_blockable_async_method]") {
    TestClass test;
    const signal_t expected_result = callback;
    utility::invoke_blockable_async_method(test, &TestClass::testMethod, expected_result, nullptr);
    REQUIRE(expected_result == test.testMethod_uponComplete_arg);
}

TEST_CASE("utility::invoke_blockable_async_method - Supplies `context` to the provided method", "[utility::invoke_blockable_async_method]") {
    TestClass test;
    void * const expected_result = &test;
    utility::invoke_blockable_async_method(test, &TestClass::testMethod, callback, expected_result);
    REQUIRE(expected_result == test.testMethod_context_arg);
}

TEST_CASE("utility::invoke_blockable_async_method - If `nullptr` is supplied to `uponComplete`, the `context` argument shall be discarded", "[utility::invoke_blockable_async_method]") {
    TestClass test;
    void * const context_provided = &test;
    utility::invoke_blockable_async_method(test, &TestClass::testMethod, nullptr, context_provided);
    REQUIRE(context_provided != test.testMethod_context_arg);
}

TEST_CASE("utility::invoke_blockable_async_method - Any error code returned by the provided method shall result in `errno` being set to that value", "[utility::invoke_blockable_async_method]") {
    errno = 0;
    TestClass test;
    test.testMethod_result = ENOTTY;
    utility::invoke_blockable_async_method(test, &TestClass::testMethod, callback, &test);
    REQUIRE(ENOTTY == errno);
}

TEST_CASE("utility::invoke_blockable_async_method - If no error code returned by the provided method, then pre-existing value of `errno` shall be preserved", "[utility::invoke_blockable_async_method]") {
    errno = ENOTTY;
    TestClass test;
    test.testMethod_result = 0;
    utility::invoke_blockable_async_method(test, &TestClass::testMethod, callback, &test);
    REQUIRE(ENOTTY == errno);
}

TEST_CASE("utility::invoke_blockable_async_method - `nullptr` supplied to `uponComplete` argument is replaced by internal callback", "[utility::invoke_blockable_async_method]") {
    TestClass test;
    utility::invoke_blockable_async_method(test, &TestClass::testMethod, nullptr, nullptr);
    REQUIRE(static_cast<signal_t>(utility::synchronizing_callback) == test.testMethod_uponComplete_arg);
}

TEST_CASE("utility::invoke_blockable_async_method - A timeout occurring while waiting for the provided method to return generates error", "[utility::invoke_blockable_async_method]") {
    errno = 0;
    TestClass test;
    test.testMethod_should_hang = true;
    utility::invoke_blockable_async_method(test, &TestClass::testMethod, nullptr, nullptr);
    REQUIRE(ETIME == errno);
}

TEST_CASE("utility::invoke_blockable_async_method - If no error code returned from the blocking operation, then pre-existing value of `errno` shall be preserved", "[utility::invoke_blockable_async_method]") {
    errno = ENOTTY;
    TestClass test;
    test.testMethod_result = 0;
    utility::invoke_blockable_async_method(test, &TestClass::testMethod, nullptr, nullptr);
    REQUIRE(ENOTTY == errno);
}

TEST_CASE("utility::invoke_blockable_async_method - The function shall return the result of the underlying function", "[utility::invoke_blockable_async_method]") {
    TestClass test;
    test.testMethod_result = ENOTTY;
    REQUIRE(ENOTTY == utility::invoke_blockable_async_method(test, &TestClass::testMethod, nullptr, nullptr));
}

TEST_CASE("utility::invoke_blockable_async_method - The function shall return an error if the underlying function fails to complete", "[utility::invoke_blockable_async_method]") {
    errno = 0;
    TestClass test;
    test.testMethod_should_hang = true;
    REQUIRE(ETIME == utility::invoke_blockable_async_method(test, &TestClass::testMethod, nullptr, nullptr));
}

/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */
