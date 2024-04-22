#include "HttpResponseParser.h"

#include <llhttp.h>

namespace uvw_net {
namespace http {

class HttpResponseParserPrivate {
public:
    HttpResponseParserPrivate() {
        llhttp_settings_init(&_settings);
        _settings.on_body = onBody;
        _settings.on_header_field = onHeaderField;
        _settings.on_header_value = onHeaderValue;
        _settings.on_message_complete = onMessageComplete;
        _settings.on_status = onStatus;
        llhttp_init(&_parser, HTTP_RESPONSE, &_settings);
        _parser.data = this;
    }

    static int onBody(llhttp_t* self, const char* at, size_t length) {
        ((HttpResponseParserPrivate*)self->data)->_response.body += std::string(at, length);
        return 0;
    }

    static int onHeaderField(llhttp_t* self, const char* at, size_t length) {
        auto body = std::string(at, length);
        ((HttpResponseParserPrivate*)self->data)->_response.headers.push_back({std::string(at, length), ""});
        return 0;
    }

    static int onHeaderValue(llhttp_t* self, const char* at, size_t length) {
        auto body = std::string(at, length);
        ((HttpResponseParserPrivate*)self->data)->_response.headers.back().second.assign(at, length);
        return 0;
    }

    static int onMessageComplete(llhttp_t* self) {
        ((HttpResponseParserPrivate*)self->data)->_isComplete = true;
        return 0;
    }

    static int onStatus(llhttp_t* self, const char* at, size_t length) {
        ((HttpResponseParserPrivate*)self->data)->_response.statusCode = self->status_code;
        return 0;
    }

    llhttp_t _parser;
    llhttp_settings_t _settings;
    HttpResponse _response;
    bool _isComplete = false;
};

HttpResponseParser::HttpResponseParser() {
    _p = std::make_unique<HttpResponseParserPrivate>();
}

HttpResponseParser::~HttpResponseParser() {}

std::optional<HttpResponse> HttpResponseParser::parse(const char* data, size_t size) {
    auto err = llhttp_execute(&_p->_parser, data, size);
    if (err != HPE_OK || !_p->_isComplete) {
        return {};
    }
    return _p->_response;
}

} // namespace http
} // namespace uvw_net
