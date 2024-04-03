#pragma once

#include <algorithm>

#include "SunSpecBlock.h"
#include "SunSpecDataPoint.h"

namespace uvw_net {
namespace sunspec {

template <class T>
class MeasuredValue : public sunspec::SunSpecBlock<T> {
public:
    MeasuredValue() {}
    MeasuredValue(T value);

    MeasuredValue& operator=(const MeasuredValue& other);
    MeasuredValue& operator=(T value);

    T operator()(sunspec::DataPoint dp) const;

    bool isDirty() const;

private:
    bool m_isDirty = true;
};

} // namespace sunspec
} // namespace uvw_net
