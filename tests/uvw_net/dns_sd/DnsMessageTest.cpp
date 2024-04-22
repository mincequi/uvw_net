#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>

#include <uvw_net/dns_sd/DnsMessage.h>

using Catch::Matchers::Equals;
using namespace uvw_net::dns_sd;

TEST_CASE("DnsMessage with Questions is serialized", "[dns_sd][DnsMessage]") {
    DnsMessage message;

    auto buffer = message.toBuffer();
    REQUIRE(buffer.size() == 12);

    DnsQuestion question1{"1.2.3", DnsRecordType::A, DnsRecordClass::CHAOS};
    DnsQuestion question2{"4.5.6", DnsRecordType::AAAA, DnsRecordClass::Hesiod};
    const std::vector<DnsQuestion> questions{question1, question2};
    message.questions = questions;
    // message.answers = std::vector<DnsAnswer>(2);
    // message.authorityAnwsers = std::vector<DnsAnswer>(3);
    // message.additionalAnwsers = std::vector<DnsAnswer>(4);
    buffer = message.toBuffer();
    REQUIRE(buffer.size() == 30);

    auto msg = DnsMessage::fromBuffer(buffer.data(), buffer.size());
    REQUIRE(msg->questions.size() == 2);
}

TEST_CASE("DnsMessage which is too short fails to deserialize", "[dns_sd][DnsMessage]") {
    // Create raw buffer with SRV answer
    std::vector<uint8_t> buffer{
        0x00, 0x01, // ID
        0x84, 0x00, // Flags
        0x00, 0x01, // Questions
        0x00, 0x02, // Answers
        0x00, 0x03, // Authority Answers
        0x00, 0x04, // Additional Answers
        0x03, 'w', 'w', 'w',
        0x07, 'e', 'x', 'a', 'm', 'p', 'l', 'e',
        0x03, 'c', 'o', 'm',
        0x00,
        0x00, 0x0f, // SRV
        0x00, 0x01, // IN
        0x00, 0x02, // Priority
        0x00, 0x03, // Weight
        0x00, 0x50, // Port
        0x04, 'h', 'o', 's', 't',
        0x05, 'l', 'o', 'c', 'a', 'l',
        0x00
    };

    REQUIRE(DnsMessage::fromBuffer(buffer.data(), buffer.size()) == std::nullopt);
}

TEST_CASE("DnsMessage with SRV answer is deserialized", "[dns_sd][DnsMessage]") {
    std::vector<uint8_t> dnsResponse = {
        0x12, 0x34, // Transaction ID
        0x84, 0x00, // Flags: Response, Authoritative Answer, No error
        0x00, 0x01, // Question Count
        0x00, 0x01, // Answer Count
        0x00, 0x00, // Authority Count
        0x00, 0x00, // Additional Count
        // Question Section
        0x07, 'e', 'x', 'a', 'm', 'p', 'l', 'e', // "example"
        0x03, 'c', 'o', 'm',                     // "com"
        0x00,                                    // Null terminator for the domain name
        0x00, 0x21, // Query Type: SRV
        0x00, 0x01, // Query Class: IN (Internet)
        // Answer Section
        0xC0, 0x0C, // Pointer to the name in the Question Section
        0x00, 0x21, // Type: SRV
        0x00, 0x01, // Class: IN
        0x00, 0x00, 0x0E, 0x10, // TTL: 3600 seconds
        0x00, 0x09, // Data length: 9 bytes for SRV record
        0x00, 0x05, // Priority: 5
        0x00, 0x10, // Weight: 16
        0x13, 0x88, // Port: 5000
        0x03, 's', 'i', 'p', 0x07, 'e', 'x', 'a', 'm', 'p', 'l', 'e', 0x03, 'c', 'o', 'm', 0x00 // Target: "sip.example.com"
    };

    auto msg = DnsMessage::fromBuffer(dnsResponse.data(), dnsResponse.size());
    REQUIRE(msg->header.transactionId == 0x1234);
    REQUIRE(msg->header.isResponse);
    REQUIRE(msg->header.isAuthoritative);
    REQUIRE(msg->questions.size() == 1);
    REQUIRE(msg->answers.size() == 1);
    REQUIRE(msg->authorityAnswers.size() == 0);
    REQUIRE(msg->additionalAnswers.size() == 0);
    REQUIRE(msg->questions[0].name == "example.com.");
    REQUIRE(msg->questions[0].type == DnsRecordType::SRV);
    REQUIRE(msg->questions[0].cls == DnsRecordClass::Internet);
    REQUIRE(msg->answers[0].name == "example.com.");
    REQUIRE(msg->answers[0].type == DnsRecordType::SRV);
    REQUIRE(msg->answers[0].cls == DnsRecordClass::Internet);
    REQUIRE(msg->answers[0].ttl == 3600);
    auto srv = std::get<DnsRecordDataSrv>(msg->answers[0].data);
    REQUIRE(srv.priority == 5);
    REQUIRE(srv.weight == 16);
    REQUIRE(srv.port == 5000);
    REQUIRE(srv.target == "sip.");
}
