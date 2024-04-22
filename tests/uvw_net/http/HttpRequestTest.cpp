#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include <cstring>

#define private public
#include <uvw_net/http/HttpRequest.h>

using namespace Catch::Matchers;
using namespace uvw_net::http;

TEST_CASE("HttpRequest GET gets serialized to raw buffer", "[http][HttpRequest]") {
    HttpRequest request;
    request.host = "example.com";
    request.path = "/path";
    request.body = "someBody";

    auto [buffer, size] = request.toBuffer();
    const char data[] = "GET /path HTTP/1.1\r\nHost: example.com\r\nUser-Agent: uvw-net\r\n\r\n";
    REQUIRE_THAT(buffer.get(), Equals(data));
}
