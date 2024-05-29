#include <catch2/catch_test_macros.hpp>

#define protected public
#define private public
#include <uvw_net/http/HttpClient.h>
#include <uvw_net/http/HttpSession.h>

using namespace uvw;
using namespace uvw_net::http;

TEST_CASE("HttpSession publishes HttpClient events", "[http][HttpSession]") {
    int lastError = 0;
    HttpResponse lastResponse;
    HttpClient client("localhost");
    client._isRequestPending = true;
    client.on<error_event>([&](auto& error, auto&) {
        lastError = error.code();
    });
    client.on<HttpResponse>([&](auto& response, auto&) {
        lastResponse = response;
    });

    uvw_net::http::HttpSession cut(&client);

    //SECTION("Error from resolver") {
    //    cut._resolver->publish(error_event(1));
    //    REQUIRE(lastError == 1);
    //}

    SECTION("Error from tcp") {
        cut._client->publish(error_event(2));
        REQUIRE(lastError == 2);
    }

    SECTION("Data event handling") {
        std::string mockData =
            "HTTP/1.1 200 OK\r\n"
            "Content-Length: 8\r\n"
            "Connection: close\r\n"
            "\r\n"
            "someBody";
        auto buffer = std::make_unique<char[]>(mockData.size());
        std::copy(mockData.begin(), mockData.end(), buffer.get());
        uvw::data_event dataEvent(std::move(buffer), mockData.size());

        cut._tcp->publish(std::move(dataEvent));

        REQUIRE(lastResponse.statusCode == 200);
        REQUIRE(client._isRequestPending == false);
    }
}
