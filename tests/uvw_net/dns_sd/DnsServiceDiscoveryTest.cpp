#include <catch2/catch_test_macros.hpp>

#include <uvw_net/dns_sd/DnsMessage.h>
#define private public
#include <uvw_net/dns_sd/DnsServiceDiscovery.h>

using namespace uvw_net::dns_sd;

// There is no meaningful test for this class possible.
/*TEST_CASE("DnsServiceDiscovery sends discovery", "[dns_sd][DnsServiceDiscovery]") {
    DnsServiceDiscovery cut;
    //cut._udp = uvw::loop::get_default()->resource<uvw::udp_handle>();
    cut._udp->multicast_loop(true);
    //cut._udp->bind("127.0.0.1", 5353, uvw::udp_handle::udp_flags::REUSEADDR);
    cut._udp->bind("0.0.0.0", 5353, uvw::udp_handle::udp_flags::REUSEADDR);
    uv_udp_set_membership(cut._udp->raw(), "224.0.0.251", NULL, static_cast<uv_membership>(uvw::udp_handle::membership::JOIN_GROUP));
    bool discoverySent = false;
    cut._udp->on<uvw::udp_data_event>([&](const uvw::udp_data_event& event, uvw::udp_handle& udp) {
        const auto msg = DnsMessage::fromBuffer((uint8_t*)event.data.get(), event.length);
        if (msg->questions.size() == 1 &&
            msg->questions[0].name == "_http._tcp.local." &&
            msg->questions[0].type == DnsRecordType::SRV) discoverySent = true;
    });

    cut.discover("_http._tcp.local");
    cut._udp->recv();
    uvw::loop::get_default()->run(uvw::loop::run_mode::ONCE);
    REQUIRE(discoverySent == true);
}

TEST_CASE("DnsServiceDiscovery receives answer", "[dns_sd][DnsServiceDiscovery]") {
    DnsServiceDiscovery cut;
    cut.on<DnsRecordDataSrv>([&](const DnsRecordDataSrv& record) {
        REQUIRE(record.priority == 1);
        REQUIRE(record.weight == 2);
        REQUIRE(record.port == 80);
        REQUIRE(record.target == "host.local.");
    });
}*/
