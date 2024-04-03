#include "DnsServiceDiscovery.h"

#include "DnsMessage.h"
#include "DnsQuestion.h"
#include "DnsRecordDataSrv.h"
#include "DnsRecordType.h"

#include <iostream>

namespace uvw_net {
namespace dns_sd {

DnsServiceDiscovery::DnsServiceDiscovery() {
    _udp->on<uvw::udp_data_event>([this](const uvw::udp_data_event& event, uvw::udp_handle& udp) {
        const auto msg = DnsMessage::fromBuffer((uint8_t*)event.data.get(), event.length);
        if (!msg) {
            //std::cerr << "unknown packet> size: " << event.length << std::endl;
            return;
        }
        for (const auto& a : msg->answers) {
            if (a.type != uvw_net::DnsRecordType::SRV ||
                    !a.name.ends_with(_name + ".") ||
                    !std::holds_alternative<DnsRecordDataSrv>(a.data)) continue;

            publish(std::get<DnsRecordDataSrv>(a.data));
        }
    });

    _udp->bind("0.0.0.0", 5353, uvw::udp_handle::udp_flags::REUSEADDR);
    uv_udp_set_membership(_udp->raw(), "224.0.0.251", NULL, static_cast<uv_membership>(uvw::udp_handle::membership::JOIN_GROUP));
    _udp->recv();
}

void DnsServiceDiscovery::discover(const std::string& name) {
    _name = name;
    // https://mislove.org/teaching/cs4700/spring11/handouts/project1-primer.pdf
    uvw_net::DnsQuestion question;
    question.name = _name; // "_http._tcp.local";
    question.type = uvw_net::DnsRecordType::SRV;
    DnsMessage message;
    message.questions.push_back(question);
    const auto buffer = message.toBuffer();

    _udp->send("224.0.0.251", 5353, (char*)buffer.data(), buffer.size());
}

} // namespace dns_sd
} // namespace uvw_net
