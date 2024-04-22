#include <catch2/catch_test_macros.hpp>

#include <uvw_net/dns_sd/DnsRecordDataSrv.h>

using namespace uvw_net::dns_sd;

TEST_CASE("DnsRecordDataSrv can be parsed from raw buffer", "[dns_sd][DnsRecordDataSrv]") {
    std::vector<uint8_t> buffer{
        0x00, 0x01,
        0x00, 0x02,
        0x00, 0x50,
        0x04, 'h', 'o', 's', 't',
        0x05, 'l', 'o', 'c', 'a', 'l',
        0x00
    };

    auto record = DnsRecordDataSrv::fromBuffer(buffer.data(), buffer.data(), buffer.size());
    REQUIRE(record->priority == 1);
    REQUIRE(record->weight == 2);
    REQUIRE(record->port == 80);
    REQUIRE(record->target == "host.local.");

}
