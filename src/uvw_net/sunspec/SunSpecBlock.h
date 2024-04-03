#pragma once

#include <map>

#include "SunSpecDataPoint.h"

namespace uvw_net {
namespace sunspec {

template <class T>
class SunSpecBlock {
public:
    SunSpecBlock();

    template <class U>
    SunSpecBlock(const SunSpecBlock<U>& other);

    bool operator==(const SunSpecBlock& other) const;
    bool operator!=(const SunSpecBlock& other) const;

    SunSpecBlock& operator=(const SunSpecBlock& other);

    T& operator[](sunspec::DataPoint i);

    const std::map<sunspec::DataPoint, T>& data() const;

protected:
    std::map<sunspec::DataPoint, T> m_data;
};

} // namespace sunspec
} // namespace uvw_net
