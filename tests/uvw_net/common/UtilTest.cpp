#include <catch2/catch_test_macros.hpp>

#include <uvw/loop.h>
#include <uvw_net/common/Util.h>
#include <uvw_net/common/WarningEvent.h>

using namespace uvw_net::util;

TEST_CASE("Enums are printed", "[common][util]") {
    REQUIRE(toString(uvw_net::WarningEvent::IllegalSize) == "illegalSize");
}

TEST_CASE("Call is executed on next uvw event loop iteration", "[common][util]") {
    bool isCalled = false;
    callLater([&]() {
        isCalled = !isCalled;
    });
    REQUIRE(isCalled == false);
    uvw::loop::get_default()->run(uvw::loop::run_mode::ONCE);
    REQUIRE(isCalled == true);
    uvw::loop::get_default()->run(uvw::loop::run_mode::ONCE);
    REQUIRE(isCalled == true);
}
