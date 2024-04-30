#pragma once

#include <memory>

#include <uvw/dns.h>
#include <uvw/loop.h>
#include <uvw/tcp.h>

#include <uvw_net/http/HttpRequest.h>
#include <uvw_net/http/HttpResponseParser.h>

namespace uvw_net {
namespace http {

class HttpClient;

class HttpSession {
public:
    HttpSession(HttpClient* client);
    ~HttpSession();

private:
    std::shared_ptr<uvw::tcp_handle> _tcp = uvw::loop::get_default()->resource<uvw::tcp_handle>();

    HttpClient* _client;
    HttpRequest _request;
    HttpResponseParser _parser;

    friend class HttpClient;
};

} // namespace http
} // namespace uvw_net
