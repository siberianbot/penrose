#ifndef PENROSE_ECS_COMPONENT_HPP
#define PENROSE_ECS_COMPONENT_HPP

#include <string_view>
#include <type_traits>

namespace Penrose {

    class Component {
    public:
        virtual ~Component() = default;
    };

    template<typename T>
    concept IsComponent = std::is_base_of<Component, T>::value &&
                          std::is_default_constructible<T>::value &&
                          (requires(T) {
                              { T::name() } -> std::same_as<std::string_view>;
                          });
}

#endif // PENROSE_ECS_COMPONENT_HPP
