#pragma once
#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "Types.h"

namespace Core {

    ////////////
    // Object //
    ////////////

    class Object {
    protected:
        typedef unsigned long ObjectPointer;

        inline ObjectPointer pointer() const noexcept {
            return reinterpret_cast<ObjectPointer>(this);
        }
    public:
        virtual int hashCode() const noexcept;
        virtual bool equals(const Object& other) const noexcept;

        inline bool operator ==(const Object& other) const noexcept {
            if (hashCode() != other.hashCode()) {
                return false;
            }

            return equals(other);
        }

        inline bool operator !=(const Object& other) const noexcept {
            if (hashCode() == other.hashCode()) {
                return false;
            }

            return !equals(other);
        }
    };

    ///////////////
    // Operators //
    ///////////////

    template<typename T>
    inline bool operator ==(const Object& a, const T& b) {
        return a.equals(reinterpret_cast<const Object&>(b));
    }

    template<typename T>
    inline bool operator !=(const Object& a, const T& b) {
        return !a.equals(reinterpret_cast<const Object&>(b));
    }

    template<typename T>
    inline bool operator ==(const T& a, const Object& b) {
        return b.equals(reinterpret_cast<const Object&>(a));
    }

    template<typename T>
    inline bool operator !=(const T& a, const Object& b) {
        return !b.equals(reinterpret_cast<const Object&>(a));
    }
}

#endif
