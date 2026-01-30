#include "HttpRequest.h"

#include <algorithm>
#include <sstream>

namespace uvw_net {
namespace http {

HttpRequest::HttpRequest() {
}

std::pair<std::unique_ptr<char[]>, size_t> HttpRequest::toBuffer() {
    // Write the request to the socket
    std::stringstream ss;
    ss << method;
    ss << " ";
    ss << path;
    ss << " ";
    ss << "HTTP/1.1\r\n";

    // Write headers
    if (method != "GET" && method != "HEAD") {
        ss << "Content-Length: " << body.size() << "\r\n";
    }

    ss << "Host: " << host << "\r\n";
    ss << "Connection: close\r\n";
    ss << "User-Agent: uvw-net" << "\r\n";

    for (auto&& it : headers) {
        ss << it.first << ": " << it.second << "\r\n";
    }
    ss << "\r\n";

    if (method != "GET" && method != "HEAD") {
        ss << body;
        ss << "\r\n";
    }

    const auto str = ss.str();
    auto buffer = std::make_unique<char[]>(str.length());
    std::copy_n(str.c_str(), str.length(), buffer.get());

    return std::make_pair(std::move(buffer), str.length());
}

} // namespace http
} // namespace uvw_net
