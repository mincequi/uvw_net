#pragma once

#include <optional>
#include <vector>

#include <uvw_net/dns_sd/DnsAnswer.h>
#include <uvw_net/dns_sd/DnsHeader.h>
#include <uvw_net/dns_sd/DnsQuestion.h>

namespace uvw_net {
namespace dns_sd {

struct DnsMessage {
	DnsHeader header = { 0 };
	std::vector<DnsQuestion> questions;
	std::vector<DnsAnswer> answers;
	std::vector<DnsAnswer> authorityAnswers;
	std::vector<DnsAnswer> additionalAnswers;

    static std::optional<DnsMessage> fromBuffer(const uint8_t* buf, size_t bufSize);
    std::vector<uint8_t> toBuffer() const;
};

} // namespace dns_sd
} // namespace uvw_net
