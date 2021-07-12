#pragma once
#ifndef __CORE_STRING_H__
#define __CORE_STRING_H__

#include <string.h>
#include <utility>
#include "Value.h"

namespace Core {
    
    typedef const char* RawString;
    typedef int StringSize;

    class String : public Value<RawString> {
    public:
        typedef Value<RawString> ValueType;
    private:
        StringSize m_length;

    public:
        inline String() :
            m_length(),
            ValueType() {
        }

        inline String(RawString value) :
            m_length(strlen(value)),
            ValueType(value) {
        }

        inline String(const String& value) :
            m_length(value.m_length),
            ValueType(value.value()) {
        }

        inline String(String&& value) :
            m_length(std::move(value.m_length)),
            ValueType(std::move(value.value())) {
        }   
                         
        inline StringSize length() const noexcept {
            return m_length;
        }            
    };
} // namespace Core


#endif