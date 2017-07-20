/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */

#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include <cerrno>
#include <cstddef>
#include <cstdint>

#include "Wire.h"

using namespace remote_wiring;

void
onReceiveCallback (
    void * context_,
    size_t bytes_read_
) {
    (void)context_;
    (void)bytes_read_;
    return;
}

void
onRequestCallback (
    void * context_
) {
    (void)context_;
    return;
}

class ConcreteTwoWire : public TwoWire {
  public:
    static bool available_invoked;
    size_t available_result;
    bool begin_invoked;
    uint8_t begin_address_arg;
    int begin_result;
    bool beginTransmission_invoked;
    uint8_t beginTransmission_address_arg;
    int beginTransmission_result;
    bool end_invoked;
    int end_result;
    bool endTransmission_invoked;
    bool endTransmission_stop_arg;
    size_t endTransmission_result;
    bool flush_invoked;
    int flush_result;
    bool onReceive_invoked;
    wire_event_t onReceive_handler_arg;
    void * onReceive_context_arg;
    int onReceive_result;
    bool onRequest_invoked;
    signal_t onRequest_handler_arg;
    void * onRequest_context_arg;
    int onRequest_result;
    bool read_invoked;
    int read_result;
    bool requestFrom_invoked;
    uint8_t requestFrom_address_arg;
    size_t requestFrom_quantity_arg;
    bool requestFrom_stop_arg;
    size_t requestFrom_result;
    bool setClock_invoked;
    size_t setClock_freq_Hz_arg;
    int setClock_result;
    bool write_invoked;
    uint8_t write_byte_arg;
    int write_result;

    ConcreteTwoWire (
        void
    ) :
        available_result(0),
        begin_invoked(false),
        begin_address_arg(0x00),
        begin_result(0),
        beginTransmission_invoked(false),
        beginTransmission_address_arg(0x00),
        beginTransmission_result(0),
        end_invoked(false),
        end_result(0),
        endTransmission_invoked(false),
        endTransmission_stop_arg(false),
        endTransmission_result(0x00),
        flush_invoked(false),
        flush_result(0),
        onReceive_invoked(false),
        onReceive_handler_arg(nullptr),
        onReceive_context_arg(nullptr),
        onReceive_result(0),
        onRequest_invoked(false),
        onRequest_handler_arg(nullptr),
        onRequest_context_arg(nullptr),
        onRequest_result(0),
        read_invoked(false),
        read_result(0),
        requestFrom_invoked(false),
        requestFrom_address_arg(0x00),
        requestFrom_quantity_arg(0),
        requestFrom_stop_arg(false),
        requestFrom_result(0),
        setClock_invoked(false),
        setClock_freq_Hz_arg(0),
        setClock_result(0),
        write_invoked(false),
        write_byte_arg(255),
        write_result(0)
    {}

  private:
    inline
    size_t
    _available (
        void
    ) const override {
        available_invoked = true;
        return available_result;
    }

    inline
    int
    _begin (
        uint8_t address_
    ) override {
        begin_invoked = true;
        begin_address_arg = address_;
        return begin_result;
    }

    inline
    int
    _beginTransmission (
        uint8_t address_
    ) override {
        beginTransmission_invoked = true;
        beginTransmission_address_arg = address_;
        return beginTransmission_result;
    }

    inline
    int
    _end (
        void
    ) override {
        end_invoked = true;
        return end_result;
    }

    inline
    int
    _endTransmission (
        bool stop_
    ) override {
        endTransmission_invoked = true;
        endTransmission_stop_arg = stop_;
        return endTransmission_result;
    }

    inline
    int
    _flush (
        void
    ) override {
        flush_invoked = true;
        return flush_result;
    }

    inline
    int
    _onReceive (
        wire_event_t handler_,
        void * context_
    ) override {
        onReceive_invoked = true;
        onReceive_handler_arg = handler_;
        onReceive_context_arg = context_;
        return onReceive_result;
    }

    inline
    int
    _onRequest (
        signal_t handler_,
        void * context_
    ) override {
        onRequest_invoked = true;
        onRequest_handler_arg = handler_;
        onRequest_context_arg = context_;
        return onRequest_result;
    }

    inline
    int
    _read (
        void
    ) override {
        read_invoked = true;
        return read_result;
    }

    inline
    size_t
    _requestFrom (
        uint8_t address_,
        size_t quantity_,
        bool stop_
    ) override {
        requestFrom_invoked = true;
        requestFrom_address_arg = address_;
        requestFrom_quantity_arg = quantity_;
        requestFrom_stop_arg = stop_;
        return requestFrom_result;
    }

    inline
    int
    _setClock (
        size_t freq_Hz_
    ) override {
        setClock_invoked = true;
        setClock_freq_Hz_arg = freq_Hz_;
        return setClock_result;
    }

    inline
    size_t
    _write (
        uint8_t byte_
    ) override {
        write_invoked = true;
        write_byte_arg = byte_;
        return write_result;
    }
};

bool ConcreteTwoWire::available_invoked = false;

TEST_CASE("TwoWire::available - Invokes the underlying implementation", "[TwoWire::available]") {
    ConcreteTwoWire wire;
    wire.available();
    REQUIRE( wire.available_invoked );
}

TEST_CASE("TwoWire::available - Returns the result of the underlying implementation", "[TwoWire::available]") {
    ConcreteTwoWire wire;
    const size_t expected_result = 79;
    wire.available_result = expected_result;
    REQUIRE( expected_result == wire.available() );
}

TEST_CASE("TwoWire::begin - Invokes the underlying implementation", "[TwoWire::begin]") {
    ConcreteTwoWire wire;
    wire.begin();
    REQUIRE( wire.begin_invoked );
}

TEST_CASE("TwoWire::begin - Supplies `address` argument to the underlying implementation", "[TwoWire::begin]") {
    ConcreteTwoWire wire;
    uint8_t expected_result = 0x17;
    wire.begin(expected_result);
    REQUIRE( expected_result == wire.begin_address_arg );
}

TEST_CASE("TwoWire::begin - Unspecified `address` argument, results in `0x77` provided by default to the underlying implementation", "[TwoWire::begin]") {
    ConcreteTwoWire wire;
    wire.begin();
    REQUIRE(0x77 == wire.begin_address_arg);
}

TEST_CASE("TwoWire::begin - An `address` requiring more than 7-bits shall result in errno being set to `EDOM`", "[TwoWire::begin]") {
    errno = 0;
    ConcreteTwoWire wire;
    wire.begin(0x80);
    REQUIRE(EDOM == errno);
}

TEST_CASE("TwoWire::begin - An `address` of 0x00 shall result in errno being set to `EDOM`", "[TwoWire::begin]") {
    errno = 0;
    ConcreteTwoWire wire;
    wire.begin(0x00);
    REQUIRE(EDOM == errno);
}

TEST_CASE("TwoWire::begin - An `address` of 0x01 shall result in errno being set to `EDOM`", "[TwoWire::begin]") {
    errno = 0;
    ConcreteTwoWire wire;
    wire.begin(0x01);
    REQUIRE(EDOM == errno);
}

TEST_CASE("TwoWire::begin - An `address` of 0x02 shall result in errno being set to `EDOM`", "[TwoWire::begin]") {
    errno = 0;
    ConcreteTwoWire wire;
    wire.begin(0x02);
    REQUIRE(EDOM == errno);
}

TEST_CASE("TwoWire::begin - An `address` of 0x03 shall result in errno being set to `EDOM`", "[TwoWire::begin]") {
    errno = 0;
    ConcreteTwoWire wire;
    wire.begin(0x03);
    REQUIRE(EDOM == errno);
}

TEST_CASE("TwoWire::begin - An `address` in the range [4, 7] shall result in errno being set to `EDOM`", "[TwoWire::begin]") {
    errno = 0;
    ConcreteTwoWire wire;
    for (int i = 4 ; i <= 7 ; ++i) {
        INFO("wire.begin(" << i << ")");
        wire.begin(i);
        CHECK(EDOM == errno);
    }
}

TEST_CASE("TwoWire::begin - An `address` in the range [120, 123] shall result in errno being set to `EDOM`", "[TwoWire::begin]") {
    errno = 0;
    ConcreteTwoWire wire;
    for (int i = 120 ; i <= 123 ; ++i) {
        INFO("wire.begin(" << i << ")");
        wire.begin(i);
        CHECK(EDOM == errno);
    }
}

TEST_CASE("TwoWire::begin - An `address` in the range [124, 127] shall result in errno being set to `EDOM`", "[TwoWire::begin]") {
    errno = 0;
    ConcreteTwoWire wire;
    for (int i = 124 ; i <= 127 ; ++i) {
        INFO("wire.begin(" << i << ")");
        wire.begin(i);
        CHECK(EDOM == errno);
    }
}

TEST_CASE("TwoWire::begin - An argument error shall prevent the underlying implementation from being invoked", "[TwoWire::begin]") {
    errno = 0;
    ConcreteTwoWire wire;
    wire.begin(0x80);
    CHECK(EDOM == errno);
    REQUIRE(!wire.begin_invoked);
}

TEST_CASE("TwoWire::begin - Any error code returned by the underlying implementation shall result in `errno` being set to that value", "[TwoWire::begin]") {
    errno = 0;
    ConcreteTwoWire wire;
    wire.begin_result = ENOTTY;
    wire.begin();
    REQUIRE( ENOTTY == errno );
}

TEST_CASE("TwoWire::begin - If no errors occur during execution, any previous `errno` shall be preserved", "[TwoWire::begin]") {
    errno = ENOTTY;
    ConcreteTwoWire wire;
    wire.begin();
    REQUIRE( ENOTTY == errno );
}

TEST_CASE("TwoWire::beginTransmission - Invokes the underlying implementation", "[TwoWire::beginTransmission]") {
    ConcreteTwoWire wire;
    wire.beginTransmission(0x79);
    REQUIRE( wire.beginTransmission_invoked );
}

TEST_CASE("TwoWire::beginTransmission - Supplies `address` argument to the underlying implementation", "[TwoWire::beginTransmission]") {
    ConcreteTwoWire wire;
    uint8_t expected_result = 0x79;
    wire.beginTransmission(expected_result);
    REQUIRE( expected_result == wire.beginTransmission_address_arg );
}

TEST_CASE("TwoWire::beginTransmission - An `address` requiring more than 7-bits shall result in errno being set to `EDOM`", "[TwoWire::beginTransmission]") {
    errno = 0;
    ConcreteTwoWire wire;
    wire.beginTransmission(0x80);
    REQUIRE(EDOM == errno);
}

TEST_CASE("TwoWire::beginTransmission - An argument error shall prevent the underlying implementation from being invoked", "[TwoWire::beginTransmission]") {
    errno = 0;
    ConcreteTwoWire wire;
    wire.beginTransmission(0x80);
    CHECK(EDOM == errno);
    REQUIRE(!wire.beginTransmission_invoked);
}

TEST_CASE("TwoWire::beginTransmission - Any error code returned by the underlying implementation shall result in `errno` being set to that value", "[TwoWire::beginTransmission]") {
    errno = 0;
    ConcreteTwoWire wire;
    wire.beginTransmission_result = ENOTTY;
    wire.beginTransmission(0x79);
    REQUIRE( ENOTTY == errno );
}

TEST_CASE("TwoWire::beginTransmission - If no errors occur during execution, any previous `errno` shall be preserved", "[TwoWire::beginTransmission]") {
    errno = ENOTTY;
    ConcreteTwoWire wire;
    wire.beginTransmission(0x79);
    REQUIRE( ENOTTY == errno );
}

TEST_CASE("TwoWire::end - Invokes the underlying implementation", "[TwoWire::end]") {
    ConcreteTwoWire wire;
    wire.end();
    REQUIRE( wire.end_invoked );
}

TEST_CASE("TwoWire::end - Any error code returned by the underlying implementation shall result in `errno` being set to that value", "[TwoWire::end]") {
    errno = 0;
    ConcreteTwoWire wire;
    wire.end_result = ENOTTY;
    wire.end();
    REQUIRE( ENOTTY == errno );
}

TEST_CASE("TwoWire::end - If no errors occur during execution, any previous `errno` shall be preserved", "[TwoWire::end]") {
    errno = ENOTTY;
    ConcreteTwoWire wire;
    wire.end();
    REQUIRE( ENOTTY == errno );
}

TEST_CASE("TwoWire::endTransmission - Invokes the underlying implementation", "[TwoWire::endTransmission]") {
    ConcreteTwoWire wire;
    wire.endTransmission(0x79);
    REQUIRE( wire.endTransmission_invoked );
}

TEST_CASE("TwoWire::endTransmission - Supplies `stop` argument to the underlying implementation", "[TwoWire::endTransmission]") {
    ConcreteTwoWire wire;
    bool expected_result = false;
    wire.endTransmission(expected_result);
    REQUIRE( expected_result == wire.endTransmission_stop_arg );
}

TEST_CASE("TwoWire::endTransmission - Unspecified `stop` argument, results in `true` provided by default to the underlying implementation", "[TwoWire::endTransmission]") {
    ConcreteTwoWire wire;
    wire.endTransmission();
    REQUIRE(true == wire.endTransmission_stop_arg);
}

TEST_CASE("TwoWire::endTransmission - Returns the result of the underlying implementation", "[TwoWire::endTransmission]") {
    ConcreteTwoWire wire;
    const int expected_result = 4;
    wire.endTransmission_result = expected_result;
    REQUIRE( expected_result == wire.endTransmission() );
}

TEST_CASE("TwoWire::endTransmission - A result returned from the underlying implementation not in the set {x: 0 <= x <= 4}, shall result in `ERANGE` error", "[TwoWire::endTransmission]") {
    errno = 0;
    ConcreteTwoWire wire;
    wire.endTransmission_result = 0x1979;
    wire.endTransmission();
    REQUIRE( ERANGE == errno );
}

TEST_CASE("TwoWire::flush - Invokes the underlying implementation", "[TwoWire::flush]") {
    ConcreteTwoWire wire;
    wire.flush();
    REQUIRE( wire.flush_invoked );
}

TEST_CASE("TwoWire::flush - Any error code returned by the underlying implementation shall result in `errno` being set to that value", "[TwoWire::flush]") {
    errno = 0;
    ConcreteTwoWire wire;
    wire.flush_result = ENOTTY;
    wire.flush();
    REQUIRE( ENOTTY == errno );
}

TEST_CASE("TwoWire::flush - If no errors occur during execution, any previous `errno` shall be preserved", "[TwoWire::flush]") {
    errno = ENOTTY;
    ConcreteTwoWire wire;
    wire.flush();
    REQUIRE( ENOTTY == errno );
}

TEST_CASE("TwoWire::onReceive - Invokes the underlying implementation", "[TwoWire::onReceive]") {
    ConcreteTwoWire wire;
    wire.onReceive(nullptr);
    REQUIRE( wire.onReceive_invoked );
}

TEST_CASE("TwoWire::onReceive - A pointer adhering to the `wire_event_t` signature shall be passed to the underlying implementation", "[TwoWire::onReceive]") {
    // This requirement is confirmed by the compiler do to the function signature.
    //TODO: Create a test similar to the code below to lock the method signature, and break if the method signature changes
    /*
     * ConcreteTwoWire wire;
     * typedef void(TwoWire::*onReceive_func_t)(wire_event_t, void *);
     * onReceive_func_t onReceive_func = &wire.onReceive;
     */
}

TEST_CASE("TwoWire::onReceive - Supplies `handler` argument to the underlying implementation", "[TwoWire::onReceive]") {
    ConcreteTwoWire wire;
    wire_event_t expected_result = onReceiveCallback;
    wire.onReceive(expected_result);
    REQUIRE( expected_result == wire.onReceive_handler_arg );
}

TEST_CASE("TwoWire::onReceive - Supplies `context` argument to the underlying implementation", "[TwoWire::onReceive]") {
    ConcreteTwoWire wire;
    void * const context_provided = &wire;
    wire.onReceive(onReceiveCallback, context_provided);
    REQUIRE( context_provided == wire.onReceive_context_arg );
}

TEST_CASE("TwoWire::onReceive - If the `context` argument is unspecified, the default value shall be `nullptr`", "[TwoWire::onReceive]") {
    ConcreteTwoWire wire;
    wire.onReceive(nullptr);
    REQUIRE( nullptr == wire.onReceive_context_arg );
}

TEST_CASE("TwoWire::onReceive - If `nullptr` is supplied to `handler`, the `context` argument shall be discarded", "[TwoWire::onReceive]") {
    ConcreteTwoWire wire;
    void * const context_provided = &wire;
    wire.onReceive(nullptr, context_provided);
    REQUIRE(context_provided != wire.onReceive_context_arg);
}

TEST_CASE("TwoWire::onReceive - Any error code returned by the underlying implementation shall result in `errno` being set to that value", "[TwoWire::onReceive]") {
    errno = 0;
    ConcreteTwoWire wire;
    wire.onReceive_result = ENOTTY;
    wire.onReceive(nullptr);
    REQUIRE( ENOTTY == errno );
}

TEST_CASE("TwoWire::onReceive - If no errors occur during execution, any previous `errno` shall be preserved", "[TwoWire::onReceive]") {
    errno = ENOTTY;
    ConcreteTwoWire wire;
    wire.onReceive(nullptr);
    REQUIRE( ENOTTY == errno );
}

TEST_CASE("TwoWire::onRequest - Invokes the underlying implementation", "[TwoWire::onRequest]") {
    ConcreteTwoWire wire;
    wire.onRequest(nullptr);
    REQUIRE( wire.onRequest_invoked );
}

TEST_CASE("TwoWire::onRequest - A pointer adhering to the `signal_t` signature shall be passed to the underlying implementation", "[TwoWire::onRequest]") {
    // This requirement is confirmed by the compiler do to the function signature.
    //TODO: Create a test similar to the code below to lock the method signature, and break if the method signature changes
    /*
     * ConcreteTwoWire wire;
     * typedef void(TwoWire::*onRequest_func_t)(signal_t, void *);
     * onRequest_func_t onRequest_func = &wire.onRequest;
     */
}

TEST_CASE("TwoWire::onRequest - Supplies `handler` argument to the underlying implementation", "[TwoWire::onRequest]") {
    ConcreteTwoWire wire;
    signal_t expected_result = onRequestCallback;
    wire.onRequest(expected_result);
    REQUIRE( expected_result == wire.onRequest_handler_arg );
}

TEST_CASE("TwoWire::onRequest - Supplies `context` argument to the underlying implementation", "[TwoWire::onRequest]") {
    ConcreteTwoWire wire;
    void * const context_provided = &wire;
    wire.onRequest(onRequestCallback, context_provided);
    REQUIRE( context_provided == wire.onRequest_context_arg );
}

TEST_CASE("TwoWire::onRequest - If the `context` argument is unspecified, the default value shall be `nullptr`", "[TwoWire::onRequest]") {
    ConcreteTwoWire wire;
    wire.onRequest(nullptr);
    REQUIRE( nullptr == wire.onRequest_context_arg );
}

TEST_CASE("TwoWire::onRequest - If `nullptr` is supplied to `handler`, the `context` argument shall be discarded", "[TwoWire::onRequest]") {
    ConcreteTwoWire wire;
    void * const context_provided = &wire;
    wire.onRequest(nullptr, context_provided);
    REQUIRE(context_provided != wire.onRequest_context_arg);
}

TEST_CASE("TwoWire::onRequest - Any error code returned by the underlying implementation shall result in `errno` being set to that value", "[TwoWire::onRequest]") {
    errno = 0;
    ConcreteTwoWire wire;
    wire.onRequest_result = ENOTTY;
    wire.onRequest(nullptr);
    REQUIRE( ENOTTY == errno );
}

TEST_CASE("TwoWire::onRequest - If no errors occur during execution, any previous `errno` shall be preserved", "[TwoWire::onRequest]") {
    errno = ENOTTY;
    ConcreteTwoWire wire;
    wire.onRequest(nullptr);
    REQUIRE( ENOTTY == errno );
}

TEST_CASE("TwoWire::read - Invokes the underlying implementation", "[TwoWire::read]") {
    ConcreteTwoWire wire;
    wire.read();
    REQUIRE( wire.read_invoked );
}

TEST_CASE("TwoWire::read - Returns the result of the underlying implementation", "[TwoWire::read]") {
    ConcreteTwoWire wire;
    const int expected_result = 0x79;
    wire.read_result = expected_result;
    REQUIRE( expected_result == wire.read() );
}

TEST_CASE("TwoWire::read - A result returned from the underlying implementation not in the set {x: -1 <= x <= 255}, shall result in `ERANGE` error", "[TwoWire::read]") {
    errno = 0;
    ConcreteTwoWire wire;
    wire.read_result = 0x1979;
    wire.read();
    REQUIRE( ERANGE == errno );
}

TEST_CASE("TwoWire::requestFrom - Invokes the underlying implementation", "[TwoWire::requestFrom]") {
    ConcreteTwoWire wire;
    wire.requestFrom(0x00, 0);
    REQUIRE( wire.requestFrom_invoked );
}

TEST_CASE("TwoWire::requestFrom - Supplies `address` argument to the underlying implementation", "[TwoWire::requestFrom]") {
    ConcreteTwoWire wire;
    const uint8_t expected_result = 0x79;
    wire.requestFrom(expected_result, 0);
    REQUIRE( expected_result == wire.requestFrom_address_arg );
}

TEST_CASE("TwoWire::requestFrom - An `address` requiring more than 7-bits shall result in errno being set to `EDOM`", "[TwoWire::requestFrom]") {
    errno = 0;
    ConcreteTwoWire wire;
    wire.requestFrom(0x80, 0);
    REQUIRE(EDOM == errno);
}

TEST_CASE("TwoWire::requestFrom - Supplies `quantity` argument to the underlying implementation", "[TwoWire::requestFrom]") {
    ConcreteTwoWire wire;
    const size_t expected_result = 0x19790917;
    wire.requestFrom(0x00, expected_result);
    REQUIRE( expected_result == wire.requestFrom_quantity_arg );
}

TEST_CASE("TwoWire::requestFrom - Supplies `stop` argument to the underlying implementation", "[TwoWire::requestFrom]") {
    ConcreteTwoWire wire;
    const bool expected_result = true;
    wire.requestFrom(0x00, 0, expected_result);
    REQUIRE( expected_result == wire.requestFrom_stop_arg );
}

TEST_CASE("TwoWire::requestFrom - Unspecified `address` argument, results in `true` provided by default to the underlying implementation", "[TwoWire::requestFrom]") {
    ConcreteTwoWire wire;
    wire.requestFrom(0x00, 0);
    REQUIRE(true == wire.requestFrom_stop_arg);
}

TEST_CASE("TwoWire::requestFrom - An argument error shall prevent the underlying implementation from being invoked", "[TwoWire::requestFrom]") {
    errno = 0;
    ConcreteTwoWire wire;
    wire.requestFrom(0x80, 0);
    CHECK(EDOM == errno);
    REQUIRE(!wire.requestFrom_invoked);
}

TEST_CASE("TwoWire::requestFrom - An argument error shall result in zero being returned from the API", "[TwoWire::requestFrom]") {
    errno = 0;
    ConcreteTwoWire wire;
    wire.requestFrom_result = 0x19790917;
    const size_t actual_result = wire.requestFrom(0x80, 0);
    CHECK(EDOM == errno);
    REQUIRE( 0 == actual_result );
}

TEST_CASE("TwoWire::requestFrom - Returns the result of the underlying implementation", "[TwoWire::requestFrom]") {
    ConcreteTwoWire wire;
    const int expected_result = 0x79;
    wire.requestFrom_result = expected_result;
    REQUIRE( expected_result == wire.requestFrom(0x00, 0) );
}

TEST_CASE("TwoWire::setClock - Invokes the underlying implementation", "[TwoWire::setClock]") {
    ConcreteTwoWire wire;
    wire.setClock(1);
    REQUIRE( wire.setClock_invoked );
}

TEST_CASE("TwoWire::setClock - Supplies `freq_Hz` argument to the underlying implementation", "[TwoWire::setClock]") {
    ConcreteTwoWire wire;
    const size_t expected_result = 0x19790917;
    wire.setClock(expected_result);
    REQUIRE( expected_result == wire.setClock_freq_Hz_arg );
}

TEST_CASE("TwoWire::setClock - A `freq_Hz` of zero shall result in errno being set to `EDOM`", "[TwoWire::setClock]") {
    errno = 0;
    ConcreteTwoWire wire;
    wire.setClock(0);
    REQUIRE(EDOM == errno);
}

TEST_CASE("TwoWire::setClock - An argument error shall prevent the underlying implementation from being invoked", "[TwoWire::setClock]") {
    errno = 0;
    ConcreteTwoWire wire;
    wire.setClock(0);
    CHECK(EDOM == errno);
    REQUIRE(!wire.setClock_invoked);
}

TEST_CASE("TwoWire::setClock - Any error code returned by the underlying implementation shall result in `errno` being set to that value", "[TwoWire::setClock]") {
    errno = 0;
    ConcreteTwoWire wire;
    wire.setClock_result = ENOTTY;
    wire.setClock(1);
    REQUIRE( ENOTTY == errno );
}

TEST_CASE("TwoWire::setClock - If no errors occur during execution, any previous `errno` shall be preserved", "[TwoWire::setClock]") {
    errno = ENOTTY;
    ConcreteTwoWire wire;
    wire.setClock(1);
    REQUIRE( ENOTTY == errno );
}

TEST_CASE("TwoWire::write - Invokes the underlying implementation", "[TwoWire::write]") {
    ConcreteTwoWire wire;
    wire.write(0x00);
    REQUIRE( wire.write_invoked );
}

TEST_CASE("TwoWire::write - Supplies `byte` argument to the underlying implementation", "[TwoWire::write]") {
    ConcreteTwoWire wire;
    const uint8_t expected_result = 0x79;
    wire.write(expected_result);
    REQUIRE( expected_result == wire.write_byte_arg );
}

TEST_CASE("TwoWire::write - A `byte` argument not in the set {x: 0 <= x <= 255}, shall result in `EDOM` error", "[TwoWire::write]") {
    // This requirement is confirmed by the compiler do to the function signature.
    //TODO: Create a test similar to the code below to lock the method signature, and break if the method signature changes
    /*
     * ConcreteTwoWire wire;
     * typedef void(TwoWire::*write_func_t)(uint8_t);
     * write_func_t write_func = &wire.write;
     */
}

TEST_CASE("TwoWire::write - Returns the result of the underlying implementation", "[TwoWire::write]") {
    ConcreteTwoWire wire;
    const int expected_result = 0x79;
    wire.write_result = expected_result;
    REQUIRE( expected_result == wire.write(0x00) );
}

/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */
