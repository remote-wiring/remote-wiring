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
    void * attachInterrupt_context_arg;
    bool attachInterrupt_invoked;
    signal_t attachInterrupt_isr_arg;
    size_t attachInterrupt_mode_arg;
    size_t attachInterrupt_pin_arg;
    bool detachInterrupt_invoked;
    size_t detachInterrupt_pin_arg;
    bool digitalRead_invoked;
    size_t digitalRead_pin_arg;
    bool digitalRead_result;
    bool digitalWrite_invoked;
    size_t digitalWrite_pin_arg;
    bool digitalWrite_value_arg;
    bool pinMode_invoked;
    size_t pinMode_mode_arg;
    size_t pinMode_pin_arg;

    ConcreteWiring (
        void
    ) :
        analogRead_invoked(false),
        analogRead_pin_arg(UINT_MAX),
        analogRead_result(0),
        analogWrite_invoked(false),
        analogWrite_pin_arg(UINT_MAX),
        analogWrite_value_arg(UINT_MAX),
        attachInterrupt_context_arg(nullptr),
        attachInterrupt_invoked(false),
        attachInterrupt_isr_arg(nullptr),
        attachInterrupt_mode_arg(UINT_MAX),
        attachInterrupt_pin_arg(UINT_MAX),
        detachInterrupt_invoked(false),
        detachInterrupt_pin_arg(UINT_MAX),
        digitalRead_invoked(false),
        digitalRead_pin_arg(UINT_MAX),
        digitalRead_result(false),
        digitalWrite_invoked(false),
        digitalWrite_pin_arg(UINT_MAX),
        digitalWrite_value_arg(false),
        pinMode_invoked(false),
        pinMode_mode_arg(UINT_MAX),
        pinMode_pin_arg(UINT_MAX)
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

    void
    _analogWrite (
        size_t pin_,
        size_t value_
    ) override {
        analogWrite_invoked = true;
        analogWrite_pin_arg = pin_;
        analogWrite_value_arg = value_;
        return;
    }

    void
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
        return;
    }

    void
    _detachInterrupt (
        size_t pin_
    ) override {
        detachInterrupt_invoked = true;
        detachInterrupt_pin_arg = pin_;
        return;
    }

    bool
    _digitalRead (
        size_t pin_
    ) override {
        digitalRead_invoked = true;
        digitalRead_pin_arg = pin_;
        return digitalRead_result;
    }

    void
    _digitalWrite (
        size_t pin_,
        bool value_
    ) override {
        digitalWrite_invoked = true;
        digitalWrite_pin_arg = pin_;
        digitalWrite_value_arg = value_;
        return;
    }

    void
    _pinMode (
        size_t pin_,
        size_t mode_
    ) override {
        pinMode_invoked = true;
        pinMode_mode_arg = mode_;
        pinMode_pin_arg = pin_;
        return;
    }
};

TEST_CASE("Wiring::analogRead - Out of range pin argument generates error", "[analogRead]") {
    errno = 0;
    ConcreteWiring wiring;
    wiring.analogRead(128);
    REQUIRE( EINVAL == errno );
}

TEST_CASE("Wiring::analogRead - Implementation not invoked when invalid argument supplied", "[analogRead]") {
    ConcreteWiring wiring;
    wiring.analogRead(128);
    REQUIRE( false == wiring.analogRead_invoked );
}

TEST_CASE("Wiring::analogRead - Return `UINT_MAX` on pin argument error", "[analogRead]") {
    ConcreteWiring wiring;
    size_t actual_result = wiring.analogRead(128);
    REQUIRE( UINT_MAX == actual_result );
}

TEST_CASE("Wiring::analogRead - Supplies pin argument to the underlying implementation", "[analogRead]") {
    ConcreteWiring wiring;
    const size_t expected_pin_arg = 7;
    wiring.analogRead(expected_pin_arg);
    REQUIRE( expected_pin_arg == wiring.analogRead_pin_arg );
}

TEST_CASE("Wiring::analogRead - Returns value from underlying implementation", "[analogRead]") {
    ConcreteWiring wiring;
    const size_t expected_result = 889;
    wiring.analogRead_result = expected_result;
    size_t actual_result = wiring.analogRead(0);
    REQUIRE( expected_result == actual_result );
}

TEST_CASE("Wiring::analogRead - Out of range return value from underlying implementation generates error", "[analogRead]") {
    errno = 0;
    ConcreteWiring wiring;
    wiring.analogRead_result = 1979; // out of range result to trigger error
    wiring.analogRead(0);
    REQUIRE( ERANGE == errno );
}

TEST_CASE("Wiring::analogRead - Return 1024 on out of range return value from underlying implementation", "[analogRead]") {
    ConcreteWiring wiring;
    const size_t expected_result = 1024;
    wiring.analogRead_result = 1979; // out of range result to trigger error
    size_t actual_result = wiring.analogRead(0);
    REQUIRE( expected_result == actual_result );
}

TEST_CASE("Wiring::analogWrite - Out of range pin argument generates error", "[analogWrite]") {
    errno = 0;
    ConcreteWiring wiring;
    wiring.analogWrite(128, 0);
    REQUIRE( EINVAL == errno );
}

TEST_CASE("Wiring::analogWrite - Out of range value argument generates error", "[analogWrite]") {
    errno = 0;
    ConcreteWiring wiring;
    wiring.analogWrite(0, 256);
    REQUIRE( EINVAL == errno );
}

TEST_CASE("Wiring::analogWrite - Supplies pin argument to the underlying implementation", "[analogWrite]") {
    ConcreteWiring wiring;
    const size_t expected_pin_arg = 7;
    wiring.analogWrite(expected_pin_arg, 0);
    REQUIRE( expected_pin_arg == wiring.analogWrite_pin_arg );
}

TEST_CASE("Wiring::analogWrite - Supplies value argument to the underlying implementation", "[analogWrite]") {
    ConcreteWiring wiring;
    const size_t expected_value_arg = 197;
    wiring.analogWrite(0, expected_value_arg);
    REQUIRE( expected_value_arg == wiring.analogWrite_value_arg );
}

TEST_CASE("Wiring::analogWrite - Implementation not invoked when invalid argument supplied", "[analogWrite]") {
    ConcreteWiring wiring;
    wiring.analogWrite(128, 0);
    REQUIRE( false == wiring.analogWrite_invoked );
}

TEST_CASE("Wiring::attachInterrupt - Out of range pin argument generates error", "[attachInterrupt]") {
    errno = 0;
    ConcreteWiring wiring;
    wiring.attachInterrupt(128, ::callback, wiring::LOW, nullptr);
    REQUIRE( EINVAL == errno );
}

TEST_CASE("Wiring::attachInterrupt - Supplies pin argument to the underlying implementation", "[attachInterrupt]") {
    ConcreteWiring wiring;
    const size_t expected_pin_arg = 7;
    wiring.attachInterrupt(expected_pin_arg, ::callback, wiring::LOW, nullptr);
    REQUIRE( expected_pin_arg == wiring.attachInterrupt_pin_arg );
}

TEST_CASE("Wiring::attachInterrupt - Null isr argument generates error", "[attachInterrupt]") {
    errno = 0;
    ConcreteWiring wiring;
    wiring.attachInterrupt(0, nullptr, wiring::LOW, nullptr);
    REQUIRE( EINVAL == errno );
}

TEST_CASE("Wiring::attachInterrupt - Supplies isr argument to the underlying implementation", "[attachInterrupt]") {
    const signal_t expected_isr_arg = ::callback;
    ConcreteWiring wiring;
    wiring.attachInterrupt(0, callback, wiring::LOW, nullptr);
    REQUIRE( expected_isr_arg == wiring.attachInterrupt_isr_arg );
}

TEST_CASE("Wiring::attachInterrupt - Out of range mode argument generates error", "[attachInterrupt]") {
    errno = 0;
    ConcreteWiring wiring;
    wiring.attachInterrupt(0, ::callback, UINT_MAX, nullptr);
    REQUIRE( EINVAL == errno );
}

TEST_CASE("Wiring::attachInterrupt - Supplies mode argument to the underlying implementation", "[attachInterrupt]") {
    const size_t expected_mode_arg = wiring::RISING;
    ConcreteWiring wiring;
    wiring.attachInterrupt(0, callback, expected_mode_arg, nullptr);
    REQUIRE( expected_mode_arg == wiring.attachInterrupt_mode_arg );
}

TEST_CASE("Wiring::attachInterrupt - Supplies context argument to the underlying implementation", "[attachInterrupt]") {
    size_t context = 19790917;
    void * const expected_context_arg = &context;
    ConcreteWiring wiring;
    wiring.attachInterrupt(0, callback, wiring::LOW, expected_context_arg);
    REQUIRE( expected_context_arg == wiring.attachInterrupt_context_arg );
}

TEST_CASE("Wiring::attachInterrupt - Implementation not invoked when invalid argument supplied", "[attachInterrupt]") {
    ConcreteWiring wiring;
    wiring.attachInterrupt(128, ::callback, wiring::LOW, nullptr);
    REQUIRE( false == wiring.attachInterrupt_invoked );
}

TEST_CASE("Wiring::detachInterrupt - Out of range pin argument generates error", "[detachInterrupt]") {
    errno = 0;
    ConcreteWiring wiring;
    wiring.detachInterrupt(128);
    REQUIRE( EINVAL == errno );
}

TEST_CASE("Wiring::detachInterrupt - Supplies pin argument to the underlying implementation", "[detachInterrupt]") {
    ConcreteWiring wiring;
    const size_t expected_pin_arg = 7;
    wiring.detachInterrupt(expected_pin_arg);
    REQUIRE( expected_pin_arg == wiring.detachInterrupt_pin_arg );
}

TEST_CASE("Wiring::detachInterrupt - Implementation not invoked when invalid argument supplied", "[detachInterrupt]") {
    ConcreteWiring wiring;
    wiring.detachInterrupt(128);
    REQUIRE( false == wiring.detachInterrupt_invoked );
}

TEST_CASE("Wiring::digitalRead - Out of range pin argument generates error", "[digitalRead]") {
    errno = 0;
    ConcreteWiring wiring;
    wiring.digitalRead(128);
    REQUIRE( EINVAL == errno );
}

TEST_CASE("Wiring::digitalRead - Implementation not invoked when invalid argument supplied", "[digitalRead]") {
    ConcreteWiring wiring;
    wiring.digitalRead(128);
    REQUIRE( false == wiring.digitalRead_invoked );
}

TEST_CASE("Wiring::digitalRead - Return false when invalid argument supplied", "[digitalRead]") {
    ConcreteWiring wiring;
    wiring.digitalRead_result = true;  // Set "normal" return value to `true`
    bool actual_result = wiring.digitalRead(128);
    REQUIRE( false == actual_result );
}

TEST_CASE("Wiring::digitalRead - Supplies value argument to the underlying implementation", "[digitalRead]") {
    ConcreteWiring wiring;
    const size_t expected_pin_arg = 7;
    wiring.digitalRead(expected_pin_arg);
    REQUIRE( expected_pin_arg == wiring.digitalRead_pin_arg );
}

TEST_CASE("Wiring::digitalRead - Returns value from underlying implementation", "[digitalRead]") {
    ConcreteWiring wiring;
    const bool expected_result = true;
    wiring.digitalRead_result = expected_result;
    bool actual_result = wiring.digitalRead(0);
    REQUIRE( expected_result == actual_result );
}

TEST_CASE("Wiring::digitalWrite - Out of range pin argument generates error", "[digitalWrite]") {
    errno = 0;
    ConcreteWiring wiring;
    wiring.digitalWrite(128, false);
    REQUIRE( EINVAL == errno );
}

TEST_CASE("Wiring::digitalWrite - Supplies pin argument to the underlying implementation", "[digitalWrite]") {
    ConcreteWiring wiring;
    const size_t expected_pin_arg = 7;
    wiring.digitalWrite(expected_pin_arg, false);
    REQUIRE( expected_pin_arg == wiring.digitalWrite_pin_arg );
}

TEST_CASE("Wiring::digitalWrite - Supplies value argument to the underlying implementation", "[digitalWrite]") {
    ConcreteWiring wiring;
    const bool expected_value_arg = true;
    wiring.digitalWrite(0, expected_value_arg);
    REQUIRE( expected_value_arg == wiring.digitalWrite_value_arg );
}

TEST_CASE("Wiring::digitalWrite - Implementation not invoked when invalid argument supplied", "[digitalWrite]") {
    ConcreteWiring wiring;
    wiring.digitalWrite(128, wiring::OUTPUT);
    REQUIRE( false == wiring.digitalWrite_invoked );
}

TEST_CASE("Wiring::pinMode - Out of range pin argument generates error", "[pinMode]") {
    errno = 0;
    ConcreteWiring wiring;
    wiring.pinMode(128, wiring::OUTPUT);
    REQUIRE( EINVAL == errno );
}

TEST_CASE("Wiring::pinMode - Out of range mode argument generates error", "[pinMode]") {
    errno = 0;
    ConcreteWiring wiring;
    wiring.pinMode(0, UINT_MAX);
    REQUIRE( EINVAL == errno );
}

TEST_CASE("Wiring::pinMode - Supplies pin argument to the underlying implementation", "[pinMode]") {
    ConcreteWiring wiring;
    const size_t expected_pin_arg = 7;
    wiring.pinMode(expected_pin_arg, wiring::OUTPUT);
    REQUIRE( expected_pin_arg == wiring.pinMode_pin_arg );
}

TEST_CASE("Wiring::pinMode - Supplies mode argument to the underlying implementation", "[pinMode]") {
    ConcreteWiring wiring;
    const size_t expected_mode_arg = wiring::INPUT_PULLUP;
    wiring.pinMode(0, expected_mode_arg);
    REQUIRE( expected_mode_arg == wiring.pinMode_mode_arg );
}

TEST_CASE("Wiring::pinMode - Implementation not invoked when invalid argument supplied", "[pinMode]") {
    ConcreteWiring wiring;
    wiring.pinMode(128, wiring::OUTPUT);
    REQUIRE( false == wiring.pinMode_invoked );
}

/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */
