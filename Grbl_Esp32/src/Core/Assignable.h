#pragma once
#ifndef __CORE_ASSIGNABLE_H__
#define __CORE_ASSIGNABLE_H__

#include "Types.h"

namespace Core {
    
    template <typename T>
    class Assignable {
    public:
        typedef typename Types<T>::ConstParameterType AssignableParameterType;

        virtual void assign(AssignableParameterType) = 0;
    }; 

} // namespace Core


#endif