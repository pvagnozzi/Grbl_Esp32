#pragma once
#ifndef __CORE_OBJECT_H__
#define __CORE_OBJECT_H__

namespace Core {
    
    class Object {
    public:
        Object() = default;
        Object(const Object&) = delete;
        Object(Object&&) = delete;
        virtual ~Object() = default;

        virtual int hashCode() const noexcept;
        virtual bool equals(const Object& other) const noexcept;
    }; 

    inline bool operator ==(const Object& a, const Object& b) noexcept {
        return (a.hashCode() == b.hashCode()) && (a.equals(b));
    }

    inline bool operator !=(const Object& a, const Object& b) noexcept {
        return (a.hashCode() != b.hashCode()) || (!a.equals(b));
    }
} // namespace name


#endif