#include "Util.h"

#include <uvw/loop.h>
#include <uvw/timer.h>

namespace uvw_net {
namespace util {

void callLater(std::function<void()> f) {
    auto timer = uvw::loop::get_default()->resource<uvw::timer_handle>();
    timer->on<uvw::timer_event>([f = std::move(f)](auto&, auto&) {
        f();
    });
    timer->start(uvw::timer_handle::time{0}, uvw::timer_handle::time{0});
}

} // namespace util
} // namespace uvw_net
