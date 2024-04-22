#include <catch2/catch_test_macros.hpp>

#include <uvw/loop.h>
#define private public
#include <uvw_net/http/HttpClient.h>
#include <uvw_net/http/HttpRequest.h>

using namespace uvw_net::http;

SCENARIO("HttpClient", "[http][HttpClient]") {
    HttpClient client;
    REQUIRE(client._isRequestPending == false);
    REQUIRE(client._currentSession == nullptr);
    int errorCode = 0;
    client.on<uvw::error_event>([&](auto& error, auto&) {
        errorCode = error.code();
    });
    HttpRequest request;
    request.body = "someBody";

    GIVEN("An HttpRequest is pending") {
        client._isRequestPending = true;
        WHEN("A GET request is sent") {
            client.get(request);
            uvw::loop::get_default()->run(uvw::loop::run_mode::ONCE);
            THEN("The response will be a timeout") {
                REQUIRE(errorCode == -ETIMEDOUT);
                REQUIRE(client._isRequestPending == true);
                REQUIRE(client._currentSession != nullptr);
            }
        }
    }
    GIVEN("An HttpRequest is not pending") {
        WHEN("A GET request is sent") {
            client.get(request);
            uvw::loop::get_default()->run(uvw::loop::run_mode::ONCE);
            THEN("The response will be a timeout") {
                REQUIRE(errorCode == 0);
                REQUIRE(client._isRequestPending == true);
                REQUIRE(client._currentSession != nullptr);
            }
        }
    }
}
