#pragma once

#include <optional>

#include <llhttp.h>

#include <uvw_net/http/HttpResponse.h>

namespace uvw_net {
namespace http {

class HttpResponseParser {
public:
    HttpResponseParser();

    std::optional<HttpResponse> parse(const char* data, size_t size);

private:
    static int onBody(llhttp_t* self, const char* at, size_t length);
    static int onHeaderField(llhttp_t* self, const char* at, size_t length);
    static int onHeaderValue(llhttp_t* self, const char* at, size_t length);
    static int onMessageComplete(llhttp_t* self);

    llhttp_t _parser;
    llhttp_settings_t _settings;
    HttpResponse _response;
    bool _isComplete = false;
};

} // namespace http
} // namespace uvw_net
