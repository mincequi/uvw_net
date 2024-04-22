#include <catch2/catch_test_macros.hpp>

#include <uvw_net/dns_sd/DnsParser.h>

using namespace uvw_net::dns_sd;

TEST_CASE("DnsParser can parse raw name buffer", "[dns_sd][DnsParser]") {
    std::vector<uint8_t> buffer{
        0x03, 'w', 'w', 'w',
        0x07, 'e', 'x', 'a', 'm', 'p', 'l', 'e',
        0x03, 'c', 'o', 'm',
        0x00
    };

    auto [length, name] = DnsParser::parseName(buffer.data(), buffer.data(), buffer.data() + buffer.size());
    REQUIRE(length == buffer.size());
    REQUIRE(name == "www.example.com.");
}
