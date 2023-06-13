#ifndef PENROSE_UTILS_DESTRUCTIBLE_OBJECT_INL
#define PENROSE_UTILS_DESTRUCTIBLE_OBJECT_INL

#include <utility>

namespace Penrose {

    template<typename T>
    DestructibleObject<T>::DestructibleObject(T instance, Destructor destructor)
            : _instance(instance),
              _destructor(std::move(destructor)) {
        //
    }

    template<typename T>
    DestructibleObject<T>::DestructibleObject(DestructibleObject &&rhs) noexcept {
        this->_instance = rhs._instance;
        this->_destructor = rhs._destructor;

        rhs._instance = std::nullopt;
    }

    template<typename T>
    DestructibleObject<T>::~DestructibleObject() {
        if (!this->_instance.has_value()) {
            return;
        }

        this->_destructor(*this->_instance);
        this->_instance = std::nullopt;
    }
}

#endif // PENROSE_UTILS_DESTRUCTIBLE_OBJECT_INL
