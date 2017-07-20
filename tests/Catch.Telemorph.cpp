/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */

#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include <cerrno>
#include <climits>

#include "RwConstants.h"
#include "Telemorph.h"

using namespace remote_wiring;

void * callback_context = nullptr;

void
callback (
    void * context_
) {
    (void)context_;
    return;
}

class ConcreteTelemorph : public Telemorph {
  public:
    bool attach_invoked;
    signal_t attach_uponComplete_arg;
    void * attach_context_arg;
    bool attach_should_hang;
    int attach_result;
    bool detach_invoked;
    int detach_result;
    bool firmware_invoked;
    const char * firmware_result;
    bool refresh_invoked;
    signal_t refresh_uponComplete_arg;
    void * refresh_context_arg;
    bool refresh_should_hang;
    int refresh_result;
    bool reset_invoked;
    signal_t reset_uponComplete_arg;
    void * reset_context_arg;
    bool reset_should_hang;
    int reset_result;
    bool samplingInterval_invoked;
    size_t samplingInterval_interval_ms_arg;
    int samplingInterval_result;
    bool survey_invoked;
    signal_t survey_uponComplete_arg;
    void * survey_context_arg;
    bool survey_should_hang;
    int survey_result;
    bool version_invoked;
    const SemVer * version_result;

    ConcreteTelemorph (
        void
    ) :
        attach_invoked(false),
        attach_uponComplete_arg(nullptr),
        attach_context_arg(nullptr),
        attach_should_hang(false),
        attach_result(0),
        detach_invoked(false),
        detach_result(0),
        firmware_invoked(false),
        firmware_result("Mock Telemorph v1.0.0"),
        refresh_invoked(false),
        refresh_uponComplete_arg(nullptr),
        refresh_context_arg(nullptr),
        refresh_should_hang(false),
        refresh_result(0),
        reset_invoked(false),
        reset_uponComplete_arg(nullptr),
        reset_context_arg(nullptr),
        reset_should_hang(false),
        reset_result(0),
        samplingInterval_invoked(false),
        samplingInterval_interval_ms_arg(UINT_MAX),
        samplingInterval_result(false),
        survey_invoked(false),
        survey_uponComplete_arg(nullptr),
        survey_context_arg(nullptr),
        survey_should_hang(false),
        survey_result(0),
        version_invoked(false),
        version_result(nullptr)
    {}

  private:
    int
    _attach (
        signal_t uponComplete_,
        void * context_
    ) override {
        attach_invoked = true;
        attach_uponComplete_arg = uponComplete_;
        attach_context_arg = context_;
        if ( !attach_should_hang && uponComplete_ ) { uponComplete_(context_); }
        return attach_result;
    }

    int
    _detach (
        void
    ) override {
        detach_invoked = true;
        return detach_result;
    }

    const char *
    _firmware (
        void
    ) override {
        firmware_invoked = true;
        return firmware_result;
    }

    int
    _refresh (
        signal_t uponComplete_,
        void * context_
    ) override {
        refresh_invoked = true;
        refresh_uponComplete_arg = uponComplete_;
        refresh_context_arg = context_;
        if ( !refresh_should_hang && uponComplete_ ) { uponComplete_(context_); }
        return refresh_result;
    }

    int
    _reset (
        signal_t uponComplete_,
        void * context_
    ) override {
        reset_invoked = true;
        reset_uponComplete_arg = uponComplete_;
        reset_context_arg = context_;
        if ( !reset_should_hang && uponComplete_ ) { uponComplete_(context_); }
        return reset_result;
    }

    int
    _samplingInterval (
        size_t interval_ms_
    ) override {
        samplingInterval_invoked = true;
        samplingInterval_interval_ms_arg = interval_ms_;
        return samplingInterval_result;
    }

    int
    _survey (
        signal_t uponComplete_,
        void * context_
    ) override {
        survey_invoked = true;
        survey_uponComplete_arg = uponComplete_;
        survey_context_arg = context_;
        if ( !survey_should_hang && uponComplete_ ) { uponComplete_(context_); }
        return survey_result;
    }

    const SemVer *
    _version (
        void
    ) override {
        version_invoked = true;
        return version_result;
    }
};

TEST_CASE("Telemorph::attach - Invokes the underlying implementation", "[Telemorph::attach]") {
    ConcreteTelemorph telemorph;
    telemorph.attach(nullptr, nullptr);
    REQUIRE(telemorph.attach_invoked);
}

TEST_CASE("Telemorph::attach - Supplies `uponComplete` to the underlying implementation", "[Telemorph::attach]") {
    ConcreteTelemorph telemorph;
    const signal_t expected_result = callback;
    telemorph.attach(expected_result, nullptr);
    REQUIRE(expected_result == telemorph.attach_uponComplete_arg);
}

TEST_CASE("Telemorph::attach - Supplies `context` to the underlying implementation", "[Telemorph::attach]") {
    ConcreteTelemorph telemorph;
    void * const expected_result = &telemorph;
    telemorph.attach(callback, expected_result);
    REQUIRE(expected_result == telemorph.attach_context_arg);
}

TEST_CASE("Telemorph::attach - Unspecified `context` argument, results in `nullptr` provided by default to the underlying implementation", "[Telemorph::attach]") {
    ConcreteTelemorph telemorph;
    telemorph.attach(callback);
    REQUIRE(nullptr == telemorph.attach_context_arg);
}

TEST_CASE("Telemorph::attach - Any error code returned by the underlying implementation shall result in `errno` being set to that value", "[Telemorph::attach]") {
    errno = 0;
    ConcreteTelemorph telemorph;
    telemorph.attach_result = ENOTTY;
    telemorph.attach(nullptr, nullptr);
    REQUIRE(ENOTTY == errno);
}

TEST_CASE("Telemorph::attach - If no errors occur during execution, any previous `errno` shall be preserved", "[Telemorph::attach]") {
    errno = ENOTTY;
    ConcreteTelemorph telemorph;
    telemorph.attach(nullptr, nullptr);
    REQUIRE(ENOTTY == errno);
}

TEST_CASE("Telemorph::attach - If `nullptr` is supplied to `uponComplete`, the `context` argument shall be discarded", "[Telemorph::attach]") {
    ConcreteTelemorph telemorph;
    void * const context_provided = &telemorph;
    telemorph.attach(nullptr, context_provided);
    REQUIRE(context_provided != telemorph.attach_context_arg);
}

TEST_CASE("Telemorph::attach - `nullptr` or unspecified `uponComplete` argument is replaced by internal callback", "[Telemorph::attach]") {
    ConcreteTelemorph telemorph;
    telemorph.attach(nullptr);
    REQUIRE(nullptr != telemorph.attach_uponComplete_arg);  // We do not know the address of private, static method, but we know it is not `nullptr`

    ConcreteTelemorph telemorph2;
    telemorph2.attach();
    REQUIRE(telemorph.attach_uponComplete_arg == telemorph2.attach_uponComplete_arg);  // Both variants of this test should result in the use of the same private, static method
}

TEST_CASE("Telemorph::detach - Invokes the underlying implementation", "[Telemorph::detach]") {
    ConcreteTelemorph telemorph;
    telemorph.detach();
    REQUIRE(telemorph.detach_invoked);
}

TEST_CASE("Telemorph::detach - Any error code returned by the underlying implementation shall result in `errno` being set to that value", "[Telemorph::detach]") {
    errno = 0;
    ConcreteTelemorph telemorph;
    telemorph.detach_result = ENOTTY;
    telemorph.detach();
    REQUIRE(ENOTTY == errno);
}

TEST_CASE("Telemorph::detach - If no errors occur during execution, any previous `errno` shall be preserved", "[Telemorph::detach]") {
    errno = ENOTTY;
    ConcreteTelemorph telemorph;
    telemorph.detach();
    REQUIRE(ENOTTY == errno);
}

TEST_CASE("Telemorph::firmware - Invokes the underlying implementation", "[Telemorph::firmware]") {
    ConcreteTelemorph telemorph;
    telemorph.firmware();
    REQUIRE(telemorph.firmware_invoked);
}

TEST_CASE("Telemorph::firmware - Returns the result of the underlying implementation", "[Telemorph::firmware]") {
    ConcreteTelemorph telemorph;
    REQUIRE(!strcmp("Mock Telemorph v1.0.0", telemorph.firmware()));
}

TEST_CASE("Telemorph::refresh - Invokes the underlying implementation", "[Telemorph::refresh]") {
    ConcreteTelemorph telemorph;
    telemorph.refresh(nullptr, nullptr);
    REQUIRE(telemorph.refresh_invoked);
}

TEST_CASE("Telemorph::refresh - Supplies `uponComplete` to the underlying implementation", "[Telemorph::refresh]") {
    ConcreteTelemorph telemorph;
    const signal_t expected_result = callback;
    telemorph.refresh(expected_result, nullptr);
    REQUIRE(expected_result == telemorph.refresh_uponComplete_arg);
}

TEST_CASE("Telemorph::refresh - Supplies `context` to the underlying implementation", "[Telemorph::refresh]") {
    ConcreteTelemorph telemorph;
    void * const expected_result = &telemorph;
    telemorph.refresh(callback, expected_result);
    REQUIRE(expected_result == telemorph.refresh_context_arg);
}

TEST_CASE("Telemorph::refresh - Unspecified `context` argument, results in `nullptr` provided by default to the underlying implementation", "[Telemorph::refresh]") {
    ConcreteTelemorph telemorph;
    telemorph.refresh(callback);
    REQUIRE(nullptr == telemorph.refresh_context_arg);
}

TEST_CASE("Telemorph::refresh - Any error code returned by the underlying implementation shall result in `errno` being set to that value", "[Telemorph::refresh]") {
    errno = 0;
    ConcreteTelemorph telemorph;
    telemorph.refresh_result = ENOTTY;
    telemorph.refresh(nullptr, nullptr);
    REQUIRE(ENOTTY == errno);
}

TEST_CASE("Telemorph::refresh - If no errors occur during execution, any previous `errno` shall be preserved", "[Telemorph::refresh]") {
    errno = ENOTTY;
    ConcreteTelemorph telemorph;
    telemorph.refresh(nullptr, nullptr);
    REQUIRE(ENOTTY == errno);
}

TEST_CASE("Telemorph::refresh - If `nullptr` is supplied to `uponComplete`, the `context` argument shall be discarded", "[Telemorph::refresh]") {
    ConcreteTelemorph telemorph;
    void * const context_provided = &telemorph;
    telemorph.refresh(nullptr, context_provided);
    REQUIRE(context_provided != telemorph.refresh_context_arg);
}

TEST_CASE("Telemorph::refresh - `nullptr` or unspecified `uponComplete` argument is replaced by internal callback", "[Telemorph::refresh]") {
    ConcreteTelemorph telemorph;
    telemorph.refresh(nullptr);
    REQUIRE(nullptr != telemorph.refresh_uponComplete_arg);  // We do not know the address of private, static method, but we know it is not `nullptr`

    ConcreteTelemorph telemorph2;
    telemorph2.refresh();
    REQUIRE(telemorph.refresh_uponComplete_arg == telemorph2.refresh_uponComplete_arg);  // Both variants of this test should result in the use of the same private, static method
}

TEST_CASE("Telemorph::reset - Invokes the underlying implementation", "[Telemorph::reset]") {
    ConcreteTelemorph telemorph;
    telemorph.reset(nullptr, nullptr);
    REQUIRE(telemorph.reset_invoked);
}

TEST_CASE("Telemorph::reset - Supplies `uponComplete` to the underlying implementation", "[Telemorph::reset]") {
    ConcreteTelemorph telemorph;
    const signal_t expected_result = callback;
    telemorph.reset(expected_result, nullptr);
    REQUIRE(expected_result == telemorph.reset_uponComplete_arg);
}

TEST_CASE("Telemorph::reset - Supplies `context` to the underlying implementation", "[Telemorph::reset]") {
    ConcreteTelemorph telemorph;
    void * const expected_result = &telemorph;
    telemorph.reset(callback, expected_result);
    REQUIRE(expected_result == telemorph.reset_context_arg);
}

TEST_CASE("Telemorph::reset - Unspecified `context` argument, results in `nullptr` provided by default to the underlying implementation", "[Telemorph::reset]") {
    ConcreteTelemorph telemorph;
    telemorph.reset(callback);
    REQUIRE(nullptr == telemorph.reset_context_arg);
}

TEST_CASE("Telemorph::reset - Any error code returned by the underlying implementation shall result in `errno` being set to that value", "[Telemorph::reset]") {
    errno = 0;
    ConcreteTelemorph telemorph;
    telemorph.reset_result = ENOTTY;
    telemorph.reset(nullptr, nullptr);
    REQUIRE(ENOTTY == errno);
}

TEST_CASE("Telemorph::reset - If no errors occur during execution, any previous `errno` shall be preserved", "[Telemorph::reset]") {
    errno = ENOTTY;
    ConcreteTelemorph telemorph;
    telemorph.reset(nullptr, nullptr);
    REQUIRE(ENOTTY == errno);
}

TEST_CASE("Telemorph::reset - If `nullptr` is supplied to `uponComplete`, the `context` argument shall be discarded", "[Telemorph::reset]") {
    ConcreteTelemorph telemorph;
    void * const context_provided = &telemorph;
    telemorph.reset(nullptr, context_provided);
    REQUIRE(context_provided != telemorph.reset_context_arg);
}

TEST_CASE("Telemorph::reset - `nullptr` or unspecified `uponComplete` argument is replaced by internal callback", "[Telemorph::reset]") {
    ConcreteTelemorph telemorph;
    telemorph.reset(nullptr);
    REQUIRE(nullptr != telemorph.reset_uponComplete_arg);  // We do not know the address of private, static method, but we know it is not `nullptr`

    ConcreteTelemorph telemorph2;
    telemorph2.reset();
    REQUIRE(telemorph.reset_uponComplete_arg == telemorph2.reset_uponComplete_arg);  // Both variants of this test should result in the use of the same private, static method
}

TEST_CASE("Telemorph::samplingInterval - Invokes the underlying implementation", "[Telemorph::samplingInterval]") {
    ConcreteTelemorph telemorph;
    telemorph.samplingInterval(79);
    REQUIRE(telemorph.samplingInterval_invoked);
}

TEST_CASE("Telemorph::samplingInterval - Supplies `uponComplete` to the underlying implementation", "[Telemorph::samplingInterval]") {
    ConcreteTelemorph telemorph;
    const size_t expected_result = 79;
    telemorph.samplingInterval(expected_result);
    REQUIRE(expected_result == telemorph.samplingInterval_interval_ms_arg);
}

TEST_CASE("Telemorph::samplingInterval - An `interval_ms` argument less than 1, shall result in errno being set to `EDOM`", "[Telemorph::samplingInterval]") {
    errno = 0;
    ConcreteTelemorph telemorph;
    telemorph.samplingInterval(0);
    REQUIRE(EDOM == errno);
}

TEST_CASE("Telemorph::samplingInterval - An argument error shall prevent the underlying implementation from being invoked", "[Telemorph::samplingInterval]") {
    errno = 0;
    ConcreteTelemorph telemorph;
    telemorph.samplingInterval(0);
    CHECK(EDOM == errno);
    REQUIRE(!telemorph.samplingInterval_invoked);
}

TEST_CASE("Telemorph::samplingInterval - Any error code returned by the underlying implementation shall result in `errno` being set to that value", "[Telemorph::samplingInterval]") {
    errno = 0;
    ConcreteTelemorph telemorph;
    telemorph.samplingInterval_result = ENOTTY;
    telemorph.samplingInterval(79);
    REQUIRE(ENOTTY == errno);
}

TEST_CASE("Telemorph::samplingInterval - If no errors occur during execution, any previous `errno` shall be preserved", "[Telemorph::samplingInterval]") {
    errno = ENOTTY;
    ConcreteTelemorph telemorph;
    telemorph.samplingInterval(79);
    REQUIRE(ENOTTY == errno);
}

TEST_CASE("Telemorph::survey - Invokes the underlying implementation", "[Telemorph::survey]") {
    ConcreteTelemorph telemorph;
    telemorph.survey(nullptr, nullptr);
    REQUIRE(telemorph.survey_invoked);
}

TEST_CASE("Telemorph::survey - Supplies `uponComplete` to the underlying implementation", "[Telemorph::survey]") {
    ConcreteTelemorph telemorph;
    const signal_t expected_result = callback;
    telemorph.survey(expected_result, nullptr);
    REQUIRE(expected_result == telemorph.survey_uponComplete_arg);
}

TEST_CASE("Telemorph::survey - Supplies `context` to the underlying implementation", "[Telemorph::survey]") {
    ConcreteTelemorph telemorph;
    void * const expected_result = &telemorph;
    telemorph.survey(callback, expected_result);
    REQUIRE(expected_result == telemorph.survey_context_arg);
}

TEST_CASE("Telemorph::survey - Unspecified `context` argument, results in `nullptr` provided by default to the underlying implementation", "[Telemorph::survey]") {
    ConcreteTelemorph telemorph;
    telemorph.survey(callback);
    REQUIRE(nullptr == telemorph.survey_context_arg);
}

TEST_CASE("Telemorph::survey - Any error code returned by the underlying implementation shall result in `errno` being set to that value", "[Telemorph::survey]") {
    errno = 0;
    ConcreteTelemorph telemorph;
    telemorph.survey_result = ENOTTY;
    telemorph.survey(nullptr, nullptr);
    REQUIRE(ENOTTY == errno);
}

TEST_CASE("Telemorph::survey - If no errors occur during execution, any previous `errno` shall be preserved", "[Telemorph::survey]") {
    errno = ENOTTY;
    ConcreteTelemorph telemorph;
    telemorph.survey(nullptr, nullptr);
    REQUIRE(ENOTTY == errno);
}

TEST_CASE("Telemorph::survey - If `nullptr` is supplied to `uponComplete`, the `context` argument shall be discarded", "[Telemorph::survey]") {
    ConcreteTelemorph telemorph;
    void * const context_provided = &telemorph;
    telemorph.survey(nullptr, context_provided);
    REQUIRE(context_provided != telemorph.survey_context_arg);
}

TEST_CASE("Telemorph::survey - `nullptr` or unspecified `uponComplete` argument is replaced by internal callback", "[Telemorph::survey]") {
    ConcreteTelemorph telemorph;
    telemorph.survey(nullptr);
    REQUIRE(nullptr != telemorph.survey_uponComplete_arg);  // We do not know the address of private, static method, but we know it is not `nullptr`

    ConcreteTelemorph telemorph2;
    telemorph2.survey();
    REQUIRE(telemorph.survey_uponComplete_arg == telemorph2.survey_uponComplete_arg);  // Both variants of this test should result in the use of the same private, static method
}

TEST_CASE("Telemorph::version - Invokes the underlying implementation", "[Telemorph::version]") {
    ConcreteTelemorph telemorph;
    telemorph.version();
    REQUIRE(telemorph.version_invoked);
}

TEST_CASE("Telemorph::version - Returns the result of the underlying implementation", "[Telemorph::version]") {
    ConcreteTelemorph telemorph;
    const SemVer expected_result(1,2,3);
    telemorph.version_result = &expected_result;
    REQUIRE(expected_result == *telemorph.version());
}

/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */
