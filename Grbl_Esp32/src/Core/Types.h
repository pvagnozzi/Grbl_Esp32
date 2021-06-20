#pragma once
#ifndef __TYPES_H__
#define __TYPES_H__

#include <inttypes.h>
#include <type_traits>

#define PARAMETER_TYPE(x)           typedef SubTypes<x>::ParameterType
#define PARAMETER_TYPE_CONST(x)     typedef SubTypes<x>::Const-ParameterType

namespace Core {

    // Machine //
    typedef uint8_t BYTE;
    typedef uint16_t WORD;
    typedef uint32_t DWORD;
    typedef uint64_t QWORD;

    // Signed //
    typedef int8_t INT8;
    typedef int16_t INT16;
    typedef int32_t INT32;
    typedef int64_t INT64;

    // Unsigned //
    typedef uint8_t UINT8;
    typedef uint16_t UINT16;
    typedef uint32_t UINT32;
    typedef uint64_t UINT64;

    // General Signed //
    typedef short int SHORTINT;
    typedef int INT;
    typedef long int LONGINT;

    // General Unsigned //
    typedef unsigned short int USHORTINT;
    typedef unsigned int UINT;
    typedef unsigned long int ULONGINT;

    // Float //
    typedef float FLOAT;
    typedef float SINGLE;
    typedef double DOUBLE;
    typedef double REAL;

    // SubTypes //
    template <typename T>
    struct SubTypes {
        typedef T Type;
        static constexpr bool isClass = std::is_class<Type>::value;

        typedef typename std::add_const<Type>::type ConstType;

        // Reference //
        typedef typename std::add_lvalue_reference<Type>::type ReferenceType;
        typedef typename std::add_const<ReferenceType>::type ConstReferenceType;

        // Pointer //
        typedef typename std::add_pointer<Type>::type PointerType;
        typedef typename std::add_const<PointerType>::type ConstPointerType;

        // RReference //
        typedef typename std::add_rvalue_reference<Type>::type RReferenceType;

        typedef typename std::conditional<isClass, ConstReferenceType, ConstType>::value ConstParameterType;
        typedef typename std::conditional<isClass, ReferenceType, Type>::value ParameterType;
    };

}

#endif

