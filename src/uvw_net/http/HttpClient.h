#pragma once

#include <string>

#include <uvw/emitter.h>

namespace uvw_net {
namespace http {

class HttpRequest;
class HttpResponse;

class HttpClient final: public uvw::emitter<HttpClient, HttpResponse> {
public:
    HttpClient(const std::string& host, uint16_t port = 80);
    ~HttpClient();

    void get(const HttpRequest& request, bool isFireAndForget = false);

private:
    std::string _host;
    uint16_t _port = 80;
    std::unique_ptr<class HttpClientPrivate> const d;


    bool _isRequestPending = false;
    std::unique_ptr<class HttpSession> _currentSession;

    friend class HttpClientPrivate;
    friend class HttpSession;
};

using HttpClientPtr = std::shared_ptr<HttpClient>;

} // namespace http
} // namespace uvw_net
