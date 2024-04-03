#pragma once

#include <map>
#include <string_view>

namespace uvw_net {

using DnsLabels = std::map<uint16_t, std::string_view>;

} // namespace uvw_net
