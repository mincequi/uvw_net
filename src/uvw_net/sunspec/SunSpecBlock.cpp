#include "SunSpecBlock.h"

#include "SunSpecMeasuredValue.h"
#include "SunSpecTypes.h"

namespace uvw_net {
namespace sunspec {

template <class T>
SunSpecBlock<T>::SunSpecBlock() {
}

template <class T>
template <class U>
SunSpecBlock<T>::SunSpecBlock(const SunSpecBlock<U>& other) {
    for (const auto& v : other.data()) {
        m_data[v.first] = v.second;
    }
}

template <class T>
bool SunSpecBlock<T>::operator==(const SunSpecBlock& other) const {
    return m_data == other.m_data;
}

template <class T>
bool SunSpecBlock<T>::operator!=(const SunSpecBlock& other) const {
    return m_data != other.m_data;
}

template <class T>
SunSpecBlock<T>& SunSpecBlock<T>::operator=(const SunSpecBlock& other) {
    for (const auto& kv : other.m_data) {
        m_data[kv.first] = kv.second;
    }

    return *this;
}

template <class T>
T& SunSpecBlock<T>::operator[](sunspec::DataPoint i) {
    return m_data[i];
}

template <class T>
const std::map<sunspec::DataPoint, T>& SunSpecBlock<T>::data() const {
    return m_data;
}

// Explicit template instantiation
template class SunSpecBlock<int32_t>;
template class SunSpecBlock<double>;
template class SunSpecBlock<MeasuredValue<int32_t>>;
template class SunSpecBlock<MeasuredValue<double>>;
template class SunSpecBlock<InverterOperatingStatus>;
template SunSpecBlock<MeasuredValue<double>>::SunSpecBlock(const SunSpecBlock<double>&);
template SunSpecBlock<MeasuredValue<InverterOperatingStatus>>::SunSpecBlock(const SunSpecBlock<InverterOperatingStatus>&);

} // namespace sunspec
} // namespace uvw_net
