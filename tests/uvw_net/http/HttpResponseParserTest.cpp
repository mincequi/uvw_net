#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>

#include <uvw_net/http/HttpResponseParser.h>

using Catch::Matchers::Equals;
using namespace uvw_net::http;

TEST_CASE("HttpResponseParser can parse raw buffer", "[http][HttpResponseParser]") {
    std::string http_response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Length: 13\r\n"
        "Connection: close\r\n"
        "\r\n"
        "Hello, World!";
    HttpResponseParser cut;
    auto response = cut.parse(http_response.data(), http_response.size());
    REQUIRE(response.has_value());
    REQUIRE(response->statusCode == 200);
    REQUIRE(response->headers.size() == 2);
    REQUIRE(response->headers.front().first == "Content-Length");
    REQUIRE(response->headers.front().second == "13");
    REQUIRE(response->headers.back().first == "Connection");
    REQUIRE(response->headers.back().second == "close");
    REQUIRE(response->body == "Hello, World!");
}
