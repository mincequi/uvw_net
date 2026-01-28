#include "DnsMessage.h"

#include <cstring>
#include <arpa/inet.h>

#include <magic_enum.hpp>

#include "DnsParser.h"
#include "DnsRecordClass.h"
#include "DnsRecordDataSrv.h"

namespace uvw_net {
namespace dns_sd {

std::vector<std::string> split(const std::string& str, char delimiter) {
    size_t begin = 0;
    size_t end;
    std::vector<std::string> labels;

    while ((end = str.find(delimiter, begin)) != std::string::npos) {
        labels.push_back(str.substr(begin, end - begin));
        begin = end + 1;
    }
    labels.push_back(str.substr(begin));

    return labels;
}

class DnsRecordParser {
public:
    DnsRecordParser(std::string_view buffer) {
        _begin = (uint8_t*)buffer.data();
        _end = _begin + buffer.size();
        assert(_begin < _end);
    }

	std::tuple<size_t, DnsQuestion> parseQuestion(size_t offset) {
		if (_begin + offset + DnsQuestion::minSize > _end) {
			return { 0, {} };
		}
		auto [rsize, name] = DnsParser::parseName(_begin, _begin + offset, _end);
		if (!rsize) {
			return { 0, {} };
		}
		if (_begin + offset + rsize + 4 > _end) {
			return { 0, {} };
		}

		DnsQuestion r;
		r.name = std::move(name);
        r.type = read<DnsResourceRecordType>((uint16_t*)(_begin + offset + rsize));
		rsize += sizeof(uint16_t);
        r.cls = read<DnsResourceRecordClass>((uint16_t *)(_begin + offset + rsize));
		rsize += sizeof(uint16_t);

		return { rsize, std::move(r) };
	}

    std::tuple<size_t, DnsResourceRecord> parseAnswer(size_t offset) {
        if (_begin + offset + DnsResourceRecord::minSize > _end) {
			return { 0, {} };
		}
		auto [rsize, name] = DnsParser::parseName(_begin, _begin + offset, _end);
		if (!rsize) {
			return { 0, {} };
		}
		if (_begin + offset + rsize + 10 > _end) {
			return { 0, {} };
		}

        DnsResourceRecord r;
		r.name = std::move(name);
        DnsResourceRecordType type = read<DnsResourceRecordType>((uint16_t*)(_begin + offset + rsize));
		rsize += sizeof(uint16_t);
        DnsResourceRecordClass cls = read<DnsResourceRecordClass>((uint16_t *)(_begin + offset + rsize));
		rsize += sizeof(uint16_t);
		r.ttl = ntohl(*(uint32_t *)(_begin + offset + rsize));
		rsize += sizeof(uint32_t);
		const auto dataSize = ntohs(*(uint16_t *)(_begin + offset + rsize));
		rsize += sizeof(uint16_t);
		if (_begin + offset + rsize + dataSize > _end) {
			return { 0, {} };
		}
		// resolve data
        auto data = readDnsRecordData(type, _begin + offset + rsize, dataSize);
		if (!data) return { 0, {} };
		r.data = std::move(*data);
		rsize += dataSize;

		return { rsize, std::move(r) };
	}

private:
    template <class T>
    T read(const uint16_t* p) {
        return magic_enum::enum_cast<T>(ntohs(*p)).value_or(T::Invalid);
    }

    std::optional<DnsResourceRecordData> readDnsRecordData(DnsResourceRecordType type, const uint8_t* dataPtr, uint16_t dataSize) {
        switch (type) {
        case DnsResourceRecordType::A: {
            //
            // +0x00 IPv4 (4 bytes)
            //
            if (dataSize != sizeof(DnsResourceRecordDataA)) {
                break;
            }
            return *(DnsResourceRecordDataA*)dataPtr;
        } break;
            /* TODO
            case DnsRecordType::AAAA: {
                //
                // +0x00 IPv6 (16 bytes)
                //
                AAAAData data;
                if (dataLength != data.size()) {
                    break;
                }
                memcpy(&data[0], dataPtr, data.size());
                return { true, std::move(data) };
            } break;*/
        case DnsResourceRecordType::SOA: {
            //
            // +0x00 Primary name server (VARIANT, 2 bytes at least)
            //	...
            // +0x02 Responsible authority's mailbox (VARIANT, 2 bytes at least)
            //  ...
            // +0x04 Serial number
            // +0x08 Refresh interval
            // +0x0C Retry interval
            // +0x10 Expire limit
            // +0x14 Minimum TTL
            //
            if (dataSize < 0x18) {
                return {};
            }
            size_t offset = 0;

            auto [primaySvrLen, primarySvr] = DnsParser::parseName(_begin, dataPtr + offset, _end);
                    if (!primaySvrLen || dataSize < 0x16 + primaySvrLen) {
                return {};
            }
            offset += primaySvrLen;

            auto [mailboxLen, mailbox] = DnsParser::parseName(_begin, dataPtr + offset, _end);
                    if (!mailboxLen || dataSize != 0x14 + primaySvrLen + mailboxLen) {
                return {};
            }
            offset += mailboxLen;

            // read other fields
            SOAData r;
            r.primaryServer = std::move(primarySvr);
            r.administrator = std::move(mailbox);
            r.serialNo = ntohl(*(uint16_t *)(dataPtr + offset));
            r.refresh = ntohl(*(uint16_t *)(dataPtr + offset + sizeof(uint32_t)));
            r.retry = ntohl(*(uint16_t *)(dataPtr + offset + sizeof(uint32_t) * 2));
            r.expire = ntohl(*(uint16_t *)(dataPtr + offset + sizeof(uint32_t) * 3));
            r.defaultTtl = ntohl(*(uint16_t *)(dataPtr + offset + sizeof(uint32_t) * 4));
            return std::move(r);
        } break;
        case DnsResourceRecordType::MX: {
            //
            // +0x00 Preference
            // +0x02 Exchange Server Name
            //
            MXData data;
            if (dataSize < sizeof(uint16_t) * 2) {
                return {};
            }
            data.preference = ntohs(*(uint16_t *)dataPtr);
            auto [nameSize, name] = DnsParser::parseName(_begin, dataPtr + sizeof(uint16_t), _end);
                    if (nameSize + sizeof(uint16_t) != dataSize) {
                return {};
            }
            data.exchange = std::move(name);
            return std::move(data);
        } break;
        case DnsResourceRecordType::NS:
        case DnsResourceRecordType::CNAME:
        case DnsResourceRecordType::PTR: {
            //
            // +0x00 Host name
            //
            auto [nameSize, name] = DnsParser::parseName(_begin, dataPtr, _end);
                    if (nameSize != dataSize) {
                return {};
            }
            return std::move(name);
        } break;
        case DnsResourceRecordType::TXT: {
            //
            // +0x00 Text length
            // +0x01 Text (VARIANT length)
            //
            // TODO
            if (dataSize <= 1 /*|| dataLength != *(uint8_t *)dataPtr + 1*/) {
                return {};
            }
            DnsResourceRecordDataTxt data;
            data.size = *(uint8_t *)dataPtr;
            data.txt = std::string((char *)dataPtr + 1, dataSize - 1);
            return std::move(data);
        } break;
        case DnsResourceRecordType::SRV:
            return DnsResourceRecordDataSrv::fromBuffer(_begin, dataPtr, dataSize);
        default: {
            // Unsupported record type (for now)
            DnsResourceRecordDataOctectStream payload;
            if (dataSize) {
                payload.resize(dataSize);
                if (payload.empty()) {
                    return {};
                }
                memcpy(&payload[0], dataPtr, dataSize);
            }
            return std::move(payload);
        } break;
        }
        return {};
    }


    uint8_t* _begin = nullptr;
    uint8_t* _end = nullptr;
};

std::optional<DnsMessage> DnsMessage::fromBuffer(std::string_view buffer) {
    // +------------+
    // | Header     |
    // +------------+
    // | Question   |
    // +------------+
    // | Answer     |
    // +------------+
    // | Authority  |
    // +------------+
    // | Additional |
    // +------------+

    if (buffer.size() < 12) {
        return {};
    }

    DnsMessage message;

    uint16_t transactionId;
    std::memcpy(&transactionId, buffer.data(), sizeof(uint16_t));
    message.transactionId = ntohs(transactionId);

    // Flags: bytes 2-3
    uint16_t flags;
    std::memcpy(&flags, buffer.data() + 2, sizeof(uint16_t));
    flags = ntohs(flags);

    // Extract bits
    message.isResponse           = (flags >> 15) & 0x1;       // QR
    message.opcode               = static_cast<DnsOpcode>((flags >> 11) & 0xF); // OPCODE
    message.isAuthoritative      = (flags >> 10) & 0x1;       // AA
    message.isTruncated          = (flags >> 9)  & 0x1;       // TC
    message.isRecursionDesired   = (flags >> 8)  & 0x1;       // RD
    message.isRecursionAvailable = (flags >> 7)  & 0x1;       // RA
    message.isAuthenticatedData  = (flags >> 5)  & 0x1;       // AD
    message.isCheckingDisabled   = (flags >> 4)  & 0x1;       // CD
    message.rcode                = static_cast<DnsRcode>(flags & 0xF); // RCODE

    auto questionCount   = ntohs(*reinterpret_cast<const uint16_t*>(buffer.data() + 4));
    auto answerCount     = ntohs(*reinterpret_cast<const uint16_t*>(buffer.data() + 6));
    auto authorityCount  = ntohs(*reinterpret_cast<const uint16_t*>(buffer.data() + 8));
    auto additionalCount = ntohs(*reinterpret_cast<const uint16_t*>(buffer.data() + 10));

    size_t offset = 12;
    DnsRecordParser parser(buffer);
    for (auto i = 0; i < questionCount; ++i) {
        auto [qsize, question] = parser.parseQuestion(offset);
                if (!qsize) {
            return {};
        }
        message.questions.emplace_back(std::move(question));
        offset += qsize;
    }

    if (message.isResponse) {
        for (auto i = 0; i < answerCount; ++i) {
            auto [rsize, record] = parser.parseAnswer(offset);
                    if (!rsize) {
                return {};
            }
            offset += rsize;
            message.answers.emplace_back(std::move(record));
        }
        for (auto i = 0; i < authorityCount; ++i) {
            auto [rsize, record] = parser.parseAnswer(offset);
                    if (!rsize) {
                return {};
            }
            offset += rsize;
            message.authorityAnswers.emplace_back(std::move(record));
        }
        for (auto i = 0; i < additionalCount; ++i) {
            auto [rsize, record] = parser.parseAnswer(offset);
                    if (!rsize) {
                return {};
            }
            offset += rsize;
            message.additionalAnswers.emplace_back(std::move(record));
        }
    }

    return message;
}

std::vector<uint8_t> DnsMessage::toBuffer() const {
    std::vector<uint8_t> buffer;
    buffer.resize(12); // make room for header and counts

    // 1 Transaction ID
    uint16_t tid = htons(transactionId);
    std::memcpy(buffer.data(), &tid, 2);

    // 2 Flags
    uint16_t flags = 0;
    flags |= (isResponse           ? 1 : 0) << 15;  // QR
    flags |= (static_cast<uint16_t>(opcode) & 0xF) << 11; // OPCODE
    flags |= (isAuthoritative      ? 1 : 0) << 10;  // AA
    flags |= (isTruncated          ? 1 : 0) << 9;   // TC
    flags |= (isRecursionDesired   ? 1 : 0) << 8;   // RD
    flags |= (isRecursionAvailable ? 1 : 0) << 7;   // RA
    flags |= (isAuthenticatedData  ? 1 : 0) << 5;   // AD
    flags |= (isCheckingDisabled   ? 1 : 0) << 4;   // CD
    flags |= (static_cast<uint16_t>(rcode) & 0xF);  // RCODE
    uint16_t netFlags = htons(flags);
    std::memcpy(buffer.data() + 2, &netFlags, 2);

    // 3 Section counts
    uint16_t questionCount   = htons(static_cast<uint16_t>(questions.size()));
    uint16_t answerCount     = htons(static_cast<uint16_t>(answers.size()));
    uint16_t authorityCount  = htons(static_cast<uint16_t>(authorityAnswers.size()));
    uint16_t additionalCount = htons(static_cast<uint16_t>(additionalAnswers.size()));
    std::memcpy(buffer.data() + 4, &questionCount, 2);
    std::memcpy(buffer.data() + 6, &answerCount, 2);
    std::memcpy(buffer.data() + 8, &authorityCount, 2);
    std::memcpy(buffer.data() + 10, &additionalCount, 2);

    uint16_t offset = 12;
    for (const auto& q : questions) {
        const auto labels = split(q.name, '.');
        for (const auto& l : labels) {
            buffer.resize(offset + l.size() + 1);
            *(buffer.data() + offset) = (uint8_t)l.size();
            std::memcpy(buffer.data() + offset + 1, l.data(), l.size());
            offset += l.size()+1;
        }
        buffer.resize(offset + 1);
        *(buffer.data() + offset) = (uint8_t)0;
        offset += 1;
        buffer.resize(offset + 4);
        const auto type = htons((uint16_t)q.type);
        const auto cls = htons((uint16_t)q.cls);
        std::memcpy(buffer.data() + offset, &type, 2);
        std::memcpy(buffer.data() + offset + 2, &cls, 2);
    }

    return buffer;
}

} // namespace dns_sd
} // namespace uvw_net
