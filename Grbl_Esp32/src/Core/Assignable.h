#pragma once
#ifndef __ASSIGNBALE_H__
#define __ASSIGNABLE_H__

#include "Object.h"

namespace Core {

    template <typename T>
    class Assignable {
    public:
        virtual void assign(typename SubTypes<T>::ConstParameterType value) noexcept = 0;
    };
}

#endif
