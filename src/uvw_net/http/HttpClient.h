#pragma once

#include <uvw/emitter.h>

namespace uvw_net {
namespace http {

class HttpRequest;
class HttpResponse;

class HttpClient final: public uvw::emitter<HttpClient, HttpResponse> {
public:
    HttpClient();
    ~HttpClient();

    void get(const HttpRequest& request, bool isFireAndForget = false);

private:
    bool _isRequestPending = false;
    std::unique_ptr<class HttpSession> _currentSession;

    friend class HttpSession;
};

using HttpClientPtr = std::shared_ptr<HttpClient>;

} // namespace http
} // namespace uvw_net
