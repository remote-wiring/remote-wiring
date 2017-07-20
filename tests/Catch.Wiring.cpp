/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */

#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include <cerrno>
#include <climits>

#include "RwConstants.h"
#include "Wiring.h"

using namespace remote_wiring;

void
callback (
    void * context_
) {
    (void)context_;
    return;
}

class ConcreteWiring : public Wiring {
  public:
    bool analogRead_invoked;
    size_t analogRead_pin_arg;
    size_t analogRead_result;
    bool analogWrite_invoked;
    size_t analogWrite_pin_arg;
    size_t analogWrite_value_arg;
    int analogWrite_result;
    void * attachInterrupt_context_arg;
    bool attachInterrupt_invoked;
    signal_t attachInterrupt_isr_arg;
    size_t attachInterrupt_mode_arg;
    size_t attachInterrupt_pin_arg;
    int attachInterrupt_result;
    bool detachInterrupt_invoked;
    size_t detachInterrupt_pin_arg;
    int detachInterrupt_result;
    bool digitalRead_invoked;
    size_t digitalRead_pin_arg;
    bool digitalRead_result;
    bool digitalWrite_invoked;
    size_t digitalWrite_pin_arg;
    bool digitalWrite_value_arg;
    int digitalWrite_result;
    bool pinMode_invoked;
    size_t pinMode_mode_arg;
    size_t pinMode_pin_arg;
    int pinMode_result;

    ConcreteWiring (
        void
    ) :
        Wiring(*reinterpret_cast<TwoWire *>(0xBAD12C)),
        analogRead_invoked(false),
        analogRead_pin_arg(UINT_MAX),
        analogRead_result(0),
        analogWrite_invoked(false),
        analogWrite_pin_arg(UINT_MAX),
        analogWrite_value_arg(UINT_MAX),
        analogWrite_result(0),
        attachInterrupt_context_arg(nullptr),
        attachInterrupt_invoked(false),
        attachInterrupt_isr_arg(nullptr),
        attachInterrupt_mode_arg(UINT_MAX),
        attachInterrupt_pin_arg(UINT_MAX),
        attachInterrupt_result(0),
        detachInterrupt_invoked(false),
        detachInterrupt_pin_arg(UINT_MAX),
        detachInterrupt_result(0),
        digitalRead_invoked(false),
        digitalRead_pin_arg(UINT_MAX),
        digitalRead_result(false),
        digitalWrite_invoked(false),
        digitalWrite_pin_arg(UINT_MAX),
        digitalWrite_value_arg(false),
        digitalWrite_result(0),
        pinMode_invoked(false),
        pinMode_mode_arg(UINT_MAX),
        pinMode_pin_arg(UINT_MAX),
        pinMode_result(0)
    {}

  private:
    size_t
    _analogRead (
        size_t pin_
    ) override {
        analogRead_invoked = true;
        analogRead_pin_arg = pin_;
        return analogRead_result;
    }

    int
    _analogWrite (
        size_t pin_,
        size_t value_
    ) override {
        analogWrite_invoked = true;
        analogWrite_pin_arg = pin_;
        analogWrite_value_arg = value_;
        return analogWrite_result;
    }

    int
    _attachInterrupt (
        size_t pin_,
        signal_t isr_,
        size_t mode_,
        void * context_
    ) override {
        attachInterrupt_context_arg = context_;
        attachInterrupt_invoked = true;
        attachInterrupt_isr_arg = isr_;
        attachInterrupt_mode_arg = mode_;
        attachInterrupt_pin_arg = pin_;
        return attachInterrupt_result;
    }

    int
    _detachInterrupt (
        size_t pin_
    ) override {
        detachInterrupt_invoked = true;
        detachInterrupt_pin_arg = pin_;
        return detachInterrupt_result;
    }

    bool
    _digitalRead (
        size_t pin_
    ) override {
        digitalRead_invoked = true;
        digitalRead_pin_arg = pin_;
        return digitalRead_result;
    }

    int
    _digitalWrite (
        size_t pin_,
        bool value_
    ) override {
        digitalWrite_invoked = true;
        digitalWrite_pin_arg = pin_;
        digitalWrite_value_arg = value_;
        return digitalWrite_result;
    }

    int
    _pinMode (
        size_t pin_,
        size_t mode_
    ) override {
        pinMode_invoked = true;
        pinMode_mode_arg = mode_;
        pinMode_pin_arg = pin_;
        return pinMode_result;
    }
};

TEST_CASE("Wiring::analogRead - Invokes underlying implementation", "[Wiring::analogRead]") {
    ConcreteWiring wiring;
    wiring.analogRead(0);
    REQUIRE( true == wiring.analogRead_invoked );
}

TEST_CASE("Wiring::analogRead - Out of range pin argument generates error", "[Wiring::analogRead]") {
    errno = 0;
    ConcreteWiring wiring;
    wiring.analogRead(128);
    REQUIRE( EINVAL == errno );
}

TEST_CASE("Wiring::analogRead - Implementation not invoked when invalid argument supplied", "[Wiring::analogRead]") {
    ConcreteWiring wiring;
    wiring.analogRead(128);
    REQUIRE( false == wiring.analogRead_invoked );
}

TEST_CASE("Wiring::analogRead - Return `UINT_MAX` on pin argument error", "[Wiring::analogRead]") {
    ConcreteWiring wiring;
    const size_t actual_result = wiring.analogRead(128);
    REQUIRE( UINT_MAX == actual_result );
}

TEST_CASE("Wiring::analogRead - Supplies pin argument to the underlying implementation", "[Wiring::analogRead]") {
    ConcreteWiring wiring;
    const size_t expected_pin_arg = 7;
    wiring.analogRead(expected_pin_arg);
    REQUIRE( expected_pin_arg == wiring.analogRead_pin_arg );
}

TEST_CASE("Wiring::analogRead - Returns value from underlying implementation", "[Wiring::analogRead]") {
    ConcreteWiring wiring;
    const size_t expected_result = 889;
    wiring.analogRead_result = expected_result;
    const size_t actual_result = wiring.analogRead(0);
    REQUIRE( expected_result == actual_result );
}

TEST_CASE("Wiring::analogRead - Out of range return value from underlying implementation generates error", "[Wiring::analogRead]") {
    errno = 0;
    ConcreteWiring wiring;
    wiring.analogRead_result = 1979; // out of range result to trigger error
    wiring.analogRead(0);
    REQUIRE( ERANGE == errno );
}

TEST_CASE("Wiring::analogRead - Return 1024 on out of range return value from underlying implementation", "[Wiring::analogRead]") {
    ConcreteWiring wiring;
    const size_t expected_result = 1024;
    wiring.analogRead_result = 1979; // out of range result to trigger error
    const size_t actual_result = wiring.analogRead(0);
    REQUIRE( expected_result == actual_result );
}

TEST_CASE("Wiring::analogWrite - Invokes underlying implementation", "[Wiring::analogWrite]") {
    ConcreteWiring wiring;
    wiring.analogWrite(0, 0);
    REQUIRE( true == wiring.analogWrite_invoked );
}

TEST_CASE("Wiring::analogWrite - Out of range pin argument generates error", "[Wiring::analogWrite]") {
    errno = 0;
    ConcreteWiring wiring;
    wiring.analogWrite(128, 0);
    REQUIRE( EINVAL == errno );
}

TEST_CASE("Wiring::analogWrite - Out of range value argument generates error", "[Wiring::analogWrite]") {
    errno = 0;
    ConcreteWiring wiring;
    wiring.analogWrite(0, 256);
    REQUIRE( EINVAL == errno );
}

TEST_CASE("Wiring::analogWrite - Supplies pin argument to the underlying implementation", "[Wiring::analogWrite]") {
    ConcreteWiring wiring;
    const size_t expected_pin_arg = 7;
    wiring.analogWrite(expected_pin_arg, 0);
    REQUIRE( expected_pin_arg == wiring.analogWrite_pin_arg );
}

TEST_CASE("Wiring::analogWrite - Supplies value argument to the underlying implementation", "[Wiring::analogWrite]") {
    ConcreteWiring wiring;
    const size_t expected_value_arg = 197;
    wiring.analogWrite(0, expected_value_arg);
    REQUIRE( expected_value_arg == wiring.analogWrite_value_arg );
}

TEST_CASE("Wiring::analogWrite - Implementation not invoked when invalid argument supplied", "[Wiring::analogWrite]") {
    ConcreteWiring wiring;
    wiring.analogWrite(128, 0);
    REQUIRE( false == wiring.analogWrite_invoked );
}

TEST_CASE("Wiring::analogWrite - Any error code returned by the underlying implementation shall result in `errno` being set to that value", "[Wiring::analogWrite]") {
    errno = 0;
    ConcreteWiring wiring;
    wiring.analogWrite_result = ENOTTY;
    wiring.analogWrite(0, 0);
    REQUIRE( ENOTTY == errno );
}

TEST_CASE("Wiring::analogWrite - If no errors occur during execution, any previous `errno` shall be preserved", "[Wiring::analogWrite]") {
    errno = ENOTTY;
    ConcreteWiring wiring;
    wiring.analogWrite(0, 0);
    REQUIRE( ENOTTY == errno );
}

TEST_CASE("Wiring::attachInterrupt - Invokes underlying implementation", "[Wiring::attachInterrupt]") {
    ConcreteWiring wiring;
    wiring.attachInterrupt(0, ::callback, wiring::LOW, nullptr);
    REQUIRE( true == wiring.attachInterrupt_invoked );
}

TEST_CASE("Wiring::attachInterrupt - Out of range pin argument generates error", "[Wiring::attachInterrupt]") {
    errno = 0;
    ConcreteWiring wiring;
    wiring.attachInterrupt(128, ::callback, wiring::LOW, nullptr);
    REQUIRE( EINVAL == errno );
}

TEST_CASE("Wiring::attachInterrupt - Supplies pin argument to the underlying implementation", "[Wiring::attachInterrupt]") {
    ConcreteWiring wiring;
    const size_t expected_pin_arg = 7;
    wiring.attachInterrupt(expected_pin_arg, ::callback, wiring::LOW, nullptr);
    REQUIRE( expected_pin_arg == wiring.attachInterrupt_pin_arg );
}

TEST_CASE("Wiring::attachInterrupt - Null isr argument generates error", "[Wiring::attachInterrupt]") {
    errno = 0;
    ConcreteWiring wiring;
    wiring.attachInterrupt(0, nullptr, wiring::LOW, nullptr);
    REQUIRE( EINVAL == errno );
}

TEST_CASE("Wiring::attachInterrupt - Supplies isr argument to the underlying implementation", "[Wiring::attachInterrupt]") {
    const signal_t expected_isr_arg = ::callback;
    ConcreteWiring wiring;
    wiring.attachInterrupt(0, expected_isr_arg, wiring::LOW, nullptr);
    REQUIRE( expected_isr_arg == wiring.attachInterrupt_isr_arg );
}

TEST_CASE("Wiring::attachInterrupt - Out of range mode argument generates error", "[Wiring::attachInterrupt]") {
    errno = 0;
    ConcreteWiring wiring;
    wiring.attachInterrupt(0, ::callback, UINT_MAX, nullptr);
    REQUIRE( EINVAL == errno );
}

TEST_CASE("Wiring::attachInterrupt - Supplies mode argument to the underlying implementation", "[Wiring::attachInterrupt]") {
    const size_t expected_mode_arg = wiring::RISING;
    ConcreteWiring wiring;
    wiring.attachInterrupt(0, ::callback, expected_mode_arg, nullptr);
    REQUIRE( expected_mode_arg == wiring.attachInterrupt_mode_arg );
}

TEST_CASE("Wiring::attachInterrupt - Supplies context argument to the underlying implementation", "[Wiring::attachInterrupt]") {
    ConcreteWiring wiring;
    void * const expected_context_arg = &wiring;
    wiring.attachInterrupt(0, ::callback, wiring::LOW, expected_context_arg);
    REQUIRE( expected_context_arg == wiring.attachInterrupt_context_arg );
}

TEST_CASE("Wiring::attachInterrupt - Unspecified `context` argument, results in `nullptr` provided by default to the underlying implementation", "[Wiring::attachInterrupt]") {
    ConcreteWiring wiring;
    wiring.attachInterrupt(0, ::callback, wiring::LOW);
    REQUIRE(nullptr == wiring.attachInterrupt_context_arg);
}

TEST_CASE("Wiring::attachInterrupt - Implementation not invoked when invalid argument supplied", "[Wiring::attachInterrupt]") {
    ConcreteWiring wiring;
    wiring.attachInterrupt(128, ::callback, wiring::LOW, nullptr);
    REQUIRE( false == wiring.attachInterrupt_invoked );
}

TEST_CASE("Wiring::attachInterrupt - Any error code returned by the underlying implementation shall result in `errno` being set to that value", "[Wiring::attachInterrupt]") {
    errno = 0;
    ConcreteWiring wiring;
    wiring.attachInterrupt_result = ENOTTY;
    wiring.attachInterrupt(0, ::callback, wiring::LOW, nullptr);
    REQUIRE( ENOTTY == errno );
}

TEST_CASE("Wiring::attachInterrupt - If no errors occur during execution, any previous `errno` shall be preserved", "[Wiring::attachInterrupt]") {
    errno = ENOTTY;
    ConcreteWiring wiring;
    wiring.attachInterrupt(0, ::callback, wiring::LOW, nullptr);
    REQUIRE( ENOTTY == errno );
}

TEST_CASE("Wiring::detachInterrupt - Invokes underlying implementation", "[Wiring::detachInterrupt]") {
    ConcreteWiring wiring;
    wiring.detachInterrupt(0);
    REQUIRE( true == wiring.detachInterrupt_invoked );
}

TEST_CASE("Wiring::detachInterrupt - Out of range pin argument generates error", "[Wiring::detachInterrupt]") {
    errno = 0;
    ConcreteWiring wiring;
    wiring.detachInterrupt(128);
    REQUIRE( EINVAL == errno );
}

TEST_CASE("Wiring::detachInterrupt - Supplies pin argument to the underlying implementation", "[Wiring::detachInterrupt]") {
    ConcreteWiring wiring;
    const size_t expected_pin_arg = 7;
    wiring.detachInterrupt(expected_pin_arg);
    REQUIRE( expected_pin_arg == wiring.detachInterrupt_pin_arg );
}

TEST_CASE("Wiring::detachInterrupt - Implementation not invoked when invalid argument supplied", "[Wiring::detachInterrupt]") {
    ConcreteWiring wiring;
    wiring.detachInterrupt(128);
    REQUIRE( false == wiring.detachInterrupt_invoked );
}

TEST_CASE("Wiring::detachInterrupt - Any error code returned by the underlying implementation shall result in `errno` being set to that value", "[Wiring::detachInterrupt]") {
    errno = 0;
    ConcreteWiring wiring;
    wiring.detachInterrupt_result = ENOTTY;
    wiring.detachInterrupt(0);
    REQUIRE( ENOTTY == errno );
}

TEST_CASE("Wiring::detachInterrupt - If no errors occur during execution, any previous `errno` shall be preserved", "[Wiring::detachInterrupt]") {
    errno = ENOTTY;
    ConcreteWiring wiring;
    wiring.detachInterrupt(0);
    REQUIRE( ENOTTY == errno );
}

TEST_CASE("Wiring::digitalRead - Invokes underlying implementation", "[Wiring::digitalRead]") {
    ConcreteWiring wiring;
    wiring.digitalRead(0);
    REQUIRE( true == wiring.digitalRead_invoked );
}

TEST_CASE("Wiring::digitalRead - Out of range pin argument generates error", "[Wiring::digitalRead]") {
    errno = 0;
    ConcreteWiring wiring;
    wiring.digitalRead(128);
    REQUIRE( EINVAL == errno );
}

TEST_CASE("Wiring::digitalRead - Implementation not invoked when invalid argument supplied", "[Wiring::digitalRead]") {
    ConcreteWiring wiring;
    wiring.digitalRead(128);
    REQUIRE( false == wiring.digitalRead_invoked );
}

TEST_CASE("Wiring::digitalRead - Return false when invalid argument supplied", "[Wiring::digitalRead]") {
    ConcreteWiring wiring;
    wiring.digitalRead_result = true;  // Set "normal" return value to `true`
    const bool actual_result = wiring.digitalRead(128);
    REQUIRE( false == actual_result );
}

TEST_CASE("Wiring::digitalRead - Supplies value argument to the underlying implementation", "[Wiring::digitalRead]") {
    ConcreteWiring wiring;
    const size_t expected_pin_arg = 7;
    wiring.digitalRead(expected_pin_arg);
    REQUIRE( expected_pin_arg == wiring.digitalRead_pin_arg );
}

TEST_CASE("Wiring::digitalRead - Returns value from underlying implementation", "[Wiring::digitalRead]") {
    ConcreteWiring wiring;
    const bool expected_result = true;
    wiring.digitalRead_result = expected_result;
    const bool actual_result = wiring.digitalRead(0);
    REQUIRE( expected_result == actual_result );
}

TEST_CASE("Wiring::digitalWrite - Invokes underlying implementation", "[Wiring::digitalWrite]") {
    ConcreteWiring wiring;
    wiring.digitalWrite(0, wiring::LOW);
    REQUIRE( true == wiring.digitalWrite_invoked );
}

TEST_CASE("Wiring::digitalWrite - Out of range pin argument generates error", "[Wiring::digitalWrite]") {
    errno = 0;
    ConcreteWiring wiring;
    wiring.digitalWrite(128, wiring::LOW);
    REQUIRE( EINVAL == errno );
}

TEST_CASE("Wiring::digitalWrite - Supplies pin argument to the underlying implementation", "[Wiring::digitalWrite]") {
    ConcreteWiring wiring;
    const size_t expected_pin_arg = 7;
    wiring.digitalWrite(expected_pin_arg, wiring::LOW);
    REQUIRE( expected_pin_arg == wiring.digitalWrite_pin_arg );
}

TEST_CASE("Wiring::digitalWrite - Supplies value argument to the underlying implementation", "[Wiring::digitalWrite]") {
    ConcreteWiring wiring;
    const bool expected_value_arg = wiring::HIGH;
    wiring.digitalWrite(0, expected_value_arg);
    REQUIRE( expected_value_arg == wiring.digitalWrite_value_arg );
}

TEST_CASE("Wiring::digitalWrite - Implementation not invoked when invalid argument supplied", "[Wiring::digitalWrite]") {
    ConcreteWiring wiring;
    wiring.digitalWrite(128, wiring::LOW);
    REQUIRE( false == wiring.digitalWrite_invoked );
}

TEST_CASE("Wiring::digitalWrite - Any error code returned by the underlying implementation shall result in `errno` being set to that value", "[Wiring::digitalWrite]") {
    errno = 0;
    ConcreteWiring wiring;
    wiring.digitalWrite_result = ENOTTY;
    wiring.digitalWrite(0, wiring::LOW);
    REQUIRE( ENOTTY == errno );
}

TEST_CASE("Wiring::digitalWrite - If no errors occur during execution, any previous `errno` shall be preserved", "[Wiring::digitalWrite]") {
    errno = ENOTTY;
    ConcreteWiring wiring;
    wiring.digitalWrite(0, wiring::LOW);
    REQUIRE( ENOTTY == errno );
}

TEST_CASE("Wiring::pinMode - Invokes underlying implementation", "[Wiring::pinMode]") {
    ConcreteWiring wiring;
    wiring.pinMode(0, wiring::OUTPUT);
    REQUIRE( true == wiring.pinMode_invoked );
}

TEST_CASE("Wiring::pinMode - Out of range pin argument generates error", "[Wiring::pinMode]") {
    errno = 0;
    ConcreteWiring wiring;
    wiring.pinMode(128, wiring::OUTPUT);
    REQUIRE( EINVAL == errno );
}

TEST_CASE("Wiring::pinMode - Out of range mode argument generates error", "[Wiring::pinMode]") {
    errno = 0;
    ConcreteWiring wiring;
    wiring.pinMode(0, UINT_MAX);
    REQUIRE( EINVAL == errno );
}

TEST_CASE("Wiring::pinMode - Supplies pin argument to the underlying implementation", "[Wiring::pinMode]") {
    ConcreteWiring wiring;
    const size_t expected_pin_arg = 7;
    wiring.pinMode(expected_pin_arg, wiring::OUTPUT);
    REQUIRE( expected_pin_arg == wiring.pinMode_pin_arg );
}

TEST_CASE("Wiring::pinMode - Supplies mode argument to the underlying implementation", "[Wiring::pinMode]") {
    ConcreteWiring wiring;
    const size_t expected_mode_arg = wiring::INPUT_PULLUP;
    wiring.pinMode(0, expected_mode_arg);
    REQUIRE( expected_mode_arg == wiring.pinMode_mode_arg );
}

TEST_CASE("Wiring::pinMode - Implementation not invoked when invalid argument supplied", "[Wiring::pinMode]") {
    ConcreteWiring wiring;
    wiring.pinMode(128, wiring::OUTPUT);
    REQUIRE( false == wiring.pinMode_invoked );
}

TEST_CASE("Wiring::pinMode - Any error code returned by the underlying implementation shall result in `errno` being set to that value", "[Wiring::pinMode]") {
    errno = 0;
    ConcreteWiring wiring;
    wiring.pinMode_result = ENOTTY;
    wiring.pinMode(0, wiring::OUTPUT);
    REQUIRE( ENOTTY == errno );
}

TEST_CASE("Wiring::pinMode - If no errors occur during execution, any previous `errno` shall be preserved", "[Wiring::pinMode]") {
    errno = ENOTTY;
    ConcreteWiring wiring;
    wiring.pinMode(0, wiring::OUTPUT);
    REQUIRE( ENOTTY == errno );
}

/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */
