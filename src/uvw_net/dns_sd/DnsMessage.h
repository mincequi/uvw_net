#pragma once

#include <optional>
#include <vector>

#include <uvw_net/dns_sd/DnsOpcode.h>
#include <uvw_net/dns_sd/DnsQuestion.h>
#include <uvw_net/dns_sd/DnsRcode.h>
#include <uvw_net/dns_sd/DnsResourceRecord.h>

namespace uvw_net {
namespace dns_sd {

struct DnsMessage {
    uint16_t transactionId = 0;

    bool isResponse = false;
    DnsOpcode opcode = DnsOpcode::Query;
    bool isAuthoritative = false;
    bool isTruncated = false;
    bool isRecursionDesired = false;
    bool isRecursionAvailable = false;
    bool isAuthenticatedData = false;
    bool isCheckingDisabled = false;
    DnsRcode rcode = DnsRcode::NoError;

	std::vector<DnsQuestion> questions;
    std::vector<DnsResourceRecord> answers;
    std::vector<DnsResourceRecord> authorityAnswers;
    std::vector<DnsResourceRecord> additionalAnswers;

    static std::optional<DnsMessage> fromBuffer(std::string_view data);
    std::vector<uint8_t> toBuffer() const;
};

} // namespace dns_sd
} // namespace uvw_net
