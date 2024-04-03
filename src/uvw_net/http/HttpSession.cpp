#include "HttpSession.h"

#include "HttpClient.h"
#include "HttpResponse.h"

namespace uvw_net {
namespace http {

HttpSession::HttpSession(HttpClient* client) :
    _client(client) {
    _resolver->on<uvw::error_event>([this](const uvw::error_event& error, const uvw::get_addr_info_req&) {
        _client->publish(error);
    });
    _resolver->on<uvw::addr_info_event>([this](const uvw::addr_info_event& addrInfoEvent, const uvw::get_addr_info_req&) {
        sockaddr addr = *(addrInfoEvent.data)->ai_addr;
        _tcp->connect(addr);
    });

    _tcp->on<uvw::error_event>([this](const uvw::error_event& error, uvw::tcp_handle& tcp) {
        _client->publish(error);
    });
    _tcp->on<uvw::connect_event>([this](const uvw::connect_event&, uvw::tcp_handle& tcp) {
        auto[buffer, length] = _request.serialize();
        tcp.write(std::move(buffer), length);
    });
    // Start reading after writing
    _tcp->on<uvw::write_event>([](const uvw::write_event&, uvw::tcp_handle& tcp) {
        tcp.read();
    });
    _tcp->on<uvw::data_event>([this](const uvw::data_event& event, uvw::tcp_handle& tcp) {
        auto response = _parser.parse(event.data.get(), event.length);
        if (response) {
            _client->publish(*response);
            _client->_isRequestPending = false;
        }
    });
}

HttpSession::~HttpSession() {
    _resolver->reset();
    _resolver->cancel();
    _tcp->reset();
    _tcp->close();
}

} // namespace http
} // namespace uvw_net
