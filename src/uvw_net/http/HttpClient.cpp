#include "HttpClient.h"

#include <iostream>

#include <uv/errno.h>

#include "HttpRequest.h"
#include "HttpSession.h"

namespace uvw_net {
namespace http {

class HttpClientPrivate {
public:
    HttpClientPrivate(HttpClient& client_) : client(client_) {
        resolver->on<uvw::error_event>([this](const uvw::error_event& error, const uvw::get_addr_info_req&) {
            client.publish(error);
        });
        resolver->on<uvw::addr_info_event>([this](const uvw::addr_info_event& addrInfoEvent, const uvw::get_addr_info_req&) {
            addr = *(addrInfoEvent.data)->ai_addr;
        });

        resolver->addr_info(client._host, std::to_string(client._port));
    }

    ~HttpClientPrivate() {
        resolver->reset();
        resolver->cancel();
    }

    HttpClient& client;

    std::shared_ptr<uvw::get_addr_info_req> resolver = uvw::loop::get_default()->resource<uvw::get_addr_info_req>();
    std::shared_ptr<uvw::tcp_handle> tcp = uvw::loop::get_default()->resource<uvw::tcp_handle>();

    std::optional<sockaddr> addr;
};

HttpClient::HttpClient(const std::string& host, uint16_t port) :
    _host(host),
    _port(port),
    d(std::make_unique<HttpClientPrivate>(*this)) {
}

HttpClient::~HttpClient() {
}

void HttpClient::get(const HttpRequest& request, bool isFireAndForget) {
    if (_isRequestPending) {
        publish(uvw::error_event{(int)UV_ETIMEDOUT});
    }
    _currentSession = std::make_unique<HttpSession>(this);
    _currentSession->_request = request;
    _isRequestPending = !isFireAndForget;

    if (d->addr.has_value())
        _currentSession->_tcp->connect(d->addr.value());
    else
        std::cerr << "HttpClient> get: no address" << std::endl;
}

} // namespace http
} // namespace uvw_net
