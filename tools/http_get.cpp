#include <iostream>

#include <uvw/timer.h>
#include <uvw_net/http/HttpClient.h>
#include <uvw_net/http/HttpRequest.h>
#include <uvw_net/http/HttpResponse.h>

using namespace uvw_net::http;

int main() {
    HttpClient client("go-echarger_876432");
    client.on<HttpResponse>([](const HttpResponse& response, const HttpClient&) {
        std::cout << response.body << std::endl;
    });
    HttpRequest request;
    request.host = "go-echarger_876432";
    request.path = "/api/status";
    client.get(request);

    // Start a timeout
    auto timer = uvw::loop::get_default()->resource<uvw::timer_handle>();
    timer->on<uvw::timer_event>([](const auto&, auto&) {
        uvw::loop::get_default()->stop();
    });
    timer->start(uvw::timer_handle::time{2000}, uvw::timer_handle::time{0});

    // Run the event loop
    return uvw::loop::get_default()->run();
}
