#ifndef PENROSE_UTILS_DESTRUCTIBLE_OBJECT_HPP
#define PENROSE_UTILS_DESTRUCTIBLE_OBJECT_HPP

#include <functional>
#include <optional>

namespace Penrose {

    template<typename T>
    class DestructibleObject {
    public:
        using Destructor = std::function<void(T &)>;

    private:
        std::optional<T> _instance;
        Destructor _destructor;

    public:
        DestructibleObject(T instance, Destructor destructor);
        DestructibleObject(DestructibleObject &&rhs) noexcept;
        ~DestructibleObject();

        [[nodiscard]] T &getInstance() { return this->_instance.value(); }

        [[nodiscard]] const T &getInstance() const { return this->_instance.value(); }
    };
}

#include "DestructibleObject.inl"

#endif // PENROSE_UTILS_DESTRUCTIBLE_OBJECT_HPP
