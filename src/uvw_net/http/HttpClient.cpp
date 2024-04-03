#include "HttpClient.h"

#include <uv/errno.h>

#include "HttpRequest.h"
#include "HttpSession.h"

namespace uvw_net {
namespace http {

HttpClient::HttpClient() {
}

HttpClient::~HttpClient() {
}

void HttpClient::get(const HttpRequest& request, bool isFireAndForget) {
    if (_isRequestPending) {
        publish(uvw::error_event{(int)UV_ETIMEDOUT});
    }
    _currentSession = std::make_unique<HttpSession>(this);
    _currentSession->_request = request;
    _currentSession->_resolver->addr_info(request.host, std::to_string(request.port));
    _isRequestPending = !isFireAndForget;
}

} // namespace http
} // namespace uvw_net
