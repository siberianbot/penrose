#ifndef PENROSE_ECS_COMPONENT_HPP
#define PENROSE_ECS_COMPONENT_HPP

#include <string_view>
#include <type_traits>

namespace Penrose {

    class Component {
    public:
        virtual ~Component() = default;

        [[nodiscard]] bool isDirty() const { return this->_dirty; }

        void setDirty(bool dirty) {
            this->_dirty = dirty;
        }

    protected:
        bool _dirty = true;
    };

    template<typename T>
    concept IsComponent = std::is_base_of<Component, T>::value &&
                          std::is_default_constructible<T>::value &&
                          (requires(T) {
                              { T::name() } -> std::same_as<std::string_view>;
                          });
}

#endif // PENROSE_ECS_COMPONENT_HPP
