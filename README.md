# Overview
`uvw_net` is an event based networking library for
[`uvw`](https://github.com/skypjack/uvw) written in modern C++.
It is available as a compilable static library.

## Features
* HTTP Client
* Modbus Client
* SunSpec Client
* DNS Service Discovery
* Modbus Device Discovery
* SunSpec Device Discovery

## API
API is attempted to be consistent with [uvw API](https://skypjack.github.io/uvw/)

## Code Example

### HTTP Client
```cpp
#include <iostream>

#include <uvw/timer.h>
#include <uvw_net/http/HttpClient.h>
#include <uvw_net/http/HttpRequest.h>
#include <uvw_net/http/HttpResponse.h>

using namespace uvw_net::http;

int main() {
    // Create a HttpClient
    HttpClient client;
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
```

### Device and Service Discovery
```cpp
#include <iostream>

#include <magic_enum_iostream.hpp>

#include <uvw/timer.h>
#include <uvw_net/dns_sd/DnsServiceDiscovery.h>
#include <uvw_net/modbus/ModbusDiscovery.h>
#include <uvw_net/sunspec/SunSpecDiscovery.h>

using namespace uvw_net::dns_sd;
using namespace uvw_net::modbus;
using namespace uvw_net::sunspec;

using magic_enum::iostream_operators::operator<<;

int main() {
    DnsServiceDiscovery dnsDiscovery;
    dnsDiscovery.discover("_http._tcp.local");
    dnsDiscovery.on<DnsRecordDataSrv>([](const DnsRecordDataSrv& data, const DnsServiceDiscovery&) {
        const auto host = data.target.substr(0, data.target.find("."));
        std::cout << "http service found> host: " << host << ", port: " << data.port << std::endl;
    });

    SunSpecDiscovery sunspecDiscovery;
    sunspecDiscovery.on<SunSpecClientPtr>([](SunSpecClientPtr thing, const SunSpecDiscovery&) {
        std::stringstream supportedModels;
        std::stringstream unsupportedModels;
        for (const auto& kv : thing->models()) {
            auto m = magic_enum::enum_cast<SunSpecModel::Id>(kv.first);
            if (m.has_value())
                supportedModels << magic_enum::enum_name(m.value())  << " (" << kv.first << "), ";
            //else
            //    unsupportedModels << kv.first << ", ";
        }
        std::cout << "sunspec thing found> id: " << thing->sunSpecId()
                  << ", host: " << thing->host()
                  << ", unitId: " << (uint32_t)thing->unitId()
                  << ", supportedModels: " << supportedModels.str() << std::endl;
                  //<< "unsupportedModels: " << unsupportedModels.str();
    });

    ModbusDiscovery modbusDiscovery;
    modbusDiscovery.discover();
    // ModbusDiscovery spawns ready/connected ModbusThings
    modbusDiscovery.on<ModbusClientPtr>([&](ModbusClientPtr thing, const ModbusDiscovery&) {
        std::cout << "modbus thing found> ip: " << thing->ip() << std::endl;
        sunspecDiscovery.discover(thing);
    });

    // Start a timeout
    auto timer = uvw::loop::get_default()->resource<uvw::timer_handle>();
    timer->on<uvw::timer_event>([](const auto&, auto&) {
        uvw::loop::get_default()->stop();
    });
    timer->start(uvw::timer_handle::time{30000}, uvw::timer_handle::time{0});

    // Run the event loop
    return uvw::loop::get_default()->run();
}
```

# Building

## Requirements
To be able to build and use `uvw_net` you need a C++20 compiler.

Note that `uvw` is part of the dependencies of the project and is cloned
by `CMake`. So, you don't have to install it when `uvw_net`
libraries are compiled through `CMake`.
