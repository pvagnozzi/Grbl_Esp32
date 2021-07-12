#pragma once
#ifndef __CORE_COMPARABLE_H__
#define __CORE_COMPARABLE_H__

#include "Types.h"

namespace Core {
    
    template <typename T>
    class Comparable {
    public:
        typedef typename Type<T>::ConstParameterType ConstParameterType;

        virtual int compare(ConstParameterType parameter) const = 0;        
    };    

    template <typename T>
    inline bool operator ==(const Comparable<T>& a, typename Comparable<T>::ConstParameterType b) {
        return a.compare(b) == 0;
    }

    template <typename T>
    inline bool operator !=(const Comparable<T>& a, typename Comparable<T>::ConstParameterType b) {
        return a.compare(b) != 0;
    }

    template <typename T>
    inline bool operator >(const Comparable<T>& a, typename Comparable<T>::ConstParameterType b) {
        return a.compare(b) > 0;
    }

    template <typename T>
    inline bool operator >=(const Comparable<T>& a, typename Comparable<T>::ConstParameterType b) {
        return a.compare(b) >= 0;
    }

    template <typename T>
    inline bool operator <(const Comparable<T>& a, typename Comparable<T>::ConstParameterType b) {
        return a.compare(b) < 0;
    }

    template <typename T>
    inline bool operator <=(const Comparable<T>& a, typename Comparable<T>::ConstParameterType b) {
        return a.compare(b) <= 0;
    }

};

#endif