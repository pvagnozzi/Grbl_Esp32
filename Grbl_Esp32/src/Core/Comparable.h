#pragma once
#ifndef __COMPARABLE_H__
#define __COMPARABLE_H__

#include "Object.h"

namespace Core {

    template <typename T>
    class Comparable {
    public:
        virtual int compare(typename SubTypes<T>::ConstParameterType source) const noexcept = 0;
    };

    template <typename T>
    inline bool operator ==(const Comparable<T>& a, typename SubTypes<T>::ConstParameterType b) {
        return a.compare(b) == 0;
    }

    template <typename T>
    inline bool operator !=(const Comparable<T>& a, typename SubTypes<T>::ConstParameterType b) {
        return a.compare(b) != 0;
    }

    template <typename T>
    inline bool operator >(const Comparable<T>& a, typename SubTypes<T>::ConstParameterType b) {
        return a.compare(b) > 0;
    }

    template <typename T>
    inline bool operator >=(const Comparable<T>& a, typename SubTypes<T>::ConstParameterType b) {
        return a.compare(b) >= 0;
    }

        template <typename T>
    inline bool operator <(const Comparable<T>& a, typename SubTypes<T>::ConstParameterType b) {
        return a.compare(b) < 0;
    }

    template <typename T>
    inline bool operator <=(const Comparable<T>& a, typename SubTypes<T>::ConstParameterType b) {
        return a.compare(b) <= 0;
    }
}

#endif
