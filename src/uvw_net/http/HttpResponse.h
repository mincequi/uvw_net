#pragma once

#include <list>
#include <string>
#include <utility>

namespace uvw_net {
namespace http {

class HttpResponse {
public:
    std::list<std::pair<std::string, std::string>> headers;
    int statusCode = 0;
    std::string description;
    std::string body;

    bool messageComplete = false;
};

} // namespace http
} // namespace uvw_net
