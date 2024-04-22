#pragma once

#include <memory>
#include <optional>

#include <uvw_net/http/HttpResponse.h>

namespace uvw_net {
namespace http {

class HttpResponseParser {
public:
    HttpResponseParser();
    ~HttpResponseParser();

    std::optional<HttpResponse> parse(const char* data, size_t size);

private:
    std::unique_ptr<class HttpResponseParserPrivate> _p;
};

} // namespace http
} // namespace uvw_net
