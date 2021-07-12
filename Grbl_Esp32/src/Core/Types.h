#pragma once
#ifndef __CORE_TYPES_H__
#define __CORE_TYPES_H__

#include <inttypes.h>
#include <type_traits>

namespace Core {
    
    typedef uint8_t  BYTE;
    typedef uint16_t WORD;
    typedef uint32_t DWORD;
    typedef uint64_t QWORD;

    typedef uint8_t  UINT8;
    typedef uint16_t UINT16;
    typedef uint32_t UINT32;
    typedef uint64_t UINT64;

    typedef int8_t  INT8;
    typedef int16_t INT16;
    typedef int32_t INT32;
    typedef int64_t INT64;

    typedef float  SINGLE;
    typedef double DOUBLE;

    template <typename T>
    struct Types {
        static constexpr bool isClass = std::is_class<T>::value;

        typedef T Type;

        typedef typename std::add_pointer<Type>::type PointerType;
        typedef typename std::add_lvalue_reference<Type>::type ReferenceType;
        typedef typename std::add_rvalue_reference<Type>::type RReferenceType;

        typedef typename std::add_const<Type>::type ConstType;
        typedef typename std::add_const<PointerType>::type ConstPointerType;
        typedef typename std::add_const<ReferenceType>::type ConstReferenceType;

        typedef typename std::conditional<isClass, ReferenceType, Type>::type ParameterType;
        typedef typename std::conditional<isClass, ConstReferenceType, ConstType>::type ConstParameterType;
        typedef typename std::conditional<isClass, ReferenceType, Type>::type ReturnType;
        typedef typename std::conditional<isClass, ConstReferenceType, ConstType>::type ConstReturnType;
    };
} // namespace Core


#endif