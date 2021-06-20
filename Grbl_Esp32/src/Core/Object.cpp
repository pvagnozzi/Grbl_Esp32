#include "Object.h"

namespace Core {

    int Object::hashCode() const noexcept {
        return (int)pointer();
    }

    bool Object::equals(const Object& other) const noexcept {
        return pointer() == other.pointer ();
    }
}
