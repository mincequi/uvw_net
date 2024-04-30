#include "HttpSession.h"

#include <iostream>

#include <uvw_net/common/Util.h>

#include "HttpClient.h"
#include "HttpResponse.h"

namespace uvw_net {
namespace http {

HttpSession::HttpSession(HttpClient* client) :
    _client(client) {

    _tcp->on<uvw::error_event>([this](const uvw::error_event& error, uvw::tcp_handle&) {
        _client->publish(error);
    });
    _tcp->on<uvw::connect_event>([this](const uvw::connect_event&, uvw::tcp_handle& tcp) {
        auto[buffer, length] = _request.toBuffer();
        tcp.write(std::move(buffer), length);
    });
    // Start reading after writing
    _tcp->on<uvw::write_event>([](const uvw::write_event&, uvw::tcp_handle& tcp) {
        tcp.read();
    });
    _tcp->on<uvw::data_event>([this](const uvw::data_event& event, uvw::tcp_handle&) {
        auto response = _parser.parse(event.data.get(), event.length);
        if (response) {
            _client->publish(*response);
            _client->_isRequestPending = false;
        }
    });
    _tcp->on<uvw::close_event>([this](const uvw::close_event&, uvw::tcp_handle&) {
        std::cerr << "HttpSession> close" << std::endl;
    });
}

HttpSession::~HttpSession() {
    _tcp->reset();
    _tcp->close();
}

} // namespace http
} // namespace uvw_net
