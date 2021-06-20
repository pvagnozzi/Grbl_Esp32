#pragma once
#ifndef __EQUATABLE_H__
#define __EQUATABLE_H__

#include "Object.h"

namespace Core {

    template <typename T>
    class Equatable {
    public:
        virtual bool equals(typename SubTypes<T>::ConstParameterType source) const noexcept = 0;
    };

    template <typename T>
    inline bool operator ==(const Equatable<T>& a, typename SubTypes<T>::ConstParameterType b) {
        return a.equals(b);
    }

    template <typename T>
    inline bool operator !=(const Equatable<T>& a, typename SubTypes<T>::ConstParameterType b) {
        return !a.equals(b);
    }
}

#endif
