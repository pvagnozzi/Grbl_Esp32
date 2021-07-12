#include "Object.h"

int Core::Object::hashCode() const noexcept {
    return (int)(reinterpret_cast<long long>(this));
}

bool Core::Object::equals(const Object& other) const noexcept {
    return this == &other;
}