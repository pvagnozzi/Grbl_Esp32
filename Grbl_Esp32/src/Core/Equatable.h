#pragma once
#ifndef __CORE_EQUATABLE_H__
#define __CORE_EQUATABLE_H__

#include "Types.h"

namespace Core {
    
    template <typename T>
    class Equatable {
    public:
        typedef typename Types<T>::ConstParameterType EqualsParameterType;

        virtual bool equals(EqualsParameterType parameter) const = 0;
    };

    template <typename T>
    inline bool operator ==(const Equatable<T>& eq, typename Equatable<T>::ConstParameterType value)  {
        return eq.equals(value);
    }

    template <typename T>
    inline bool operator !=(const Equatable<T>& eq, typename Equatable<T>::ConstParameterType value)  {
        return !eq.equals(value);
    }
};

#endif