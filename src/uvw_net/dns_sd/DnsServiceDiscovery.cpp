#include "DnsServiceDiscovery.h"

#include "DnsMessage.h"
#include "DnsQuestion.h"
#include "DnsRecordDataSrv.h"

namespace uvw_net {
namespace dns_sd {

DnsServiceDiscovery::DnsServiceDiscovery(const std::string& name)
    : _name(name) {
    // Add trailing dot if missing
    if (!_name.ends_with(".")) {
        _name += ".";
    }

    _udp->on<uvw::udp_data_event>([this](const uvw::udp_data_event& event, uvw::udp_handle& udp) {
        const auto message = DnsMessage::fromBuffer({event.data.get(), event.length});
        // Check if it's a response and has mandatory sections
        if (!message || !message->isResponse || message->answers.empty() /*|| message->additionalAnswers.empty()*/) {
            return;
        }

        std::optional<DnsResourceRecordDataPtr> ptrData;
        std::optional<DnsResourceRecordDataA> aData;
        std::optional<DnsResourceRecordDataSrv> srvData;
        std::optional<DnsResourceRecordDataTxt> txtData;

        for (auto* vec : { &message->answers, &message->authorityAnswers, &message->additionalAnswers }) {
            for (const auto& a : *vec) {
                if (!a.name.ends_with(_name)) {
                    continue;
                }

                if (std::holds_alternative<DnsResourceRecordDataPtr>(a.data)) {
                    ptrData = std::get<DnsResourceRecordDataPtr>(a.data);
                } else if (std::holds_alternative<DnsResourceRecordDataA>(a.data)) {
                    aData = std::get<DnsResourceRecordDataA>(a.data);
                } else if (std::holds_alternative<DnsResourceRecordDataSrv>(a.data)) {
                    srvData = std::get<DnsResourceRecordDataSrv>(a.data);
                } else if (std::holds_alternative<DnsResourceRecordDataTxt>(a.data)) {
                    txtData = std::get<DnsResourceRecordDataTxt>(a.data);
                }
            }
        }

        if (!ptrData) {
            return;
        }

        publish(MdnsResponse{
            .ptrData = *ptrData,
            .aData = aData,
            .srvData = srvData,
            .txtData = txtData
        });
    });

    _udp->bind("0.0.0.0", 5353, uvw::udp_handle::udp_flags::REUSEADDR);
    uv_udp_set_membership(_udp->raw(), "224.0.0.251", NULL, static_cast<uv_membership>(uvw::udp_handle::membership::JOIN_GROUP));
    _udp->recv();
}

void DnsServiceDiscovery::discover() {
    // https://mislove.org/teaching/cs4700/spring11/handouts/project1-primer.pdf
    DnsQuestion question;
    question.name = _name.substr(0, _name.size() - 1); // Remove trailing dot
    DnsMessage message;
    message.questions.push_back(question);
    const auto buffer = message.toBuffer();

    _udp->send("224.0.0.251", 5353, (char*)buffer.data(), buffer.size());
}

} // namespace dns_sd
} // namespace uvw_net
