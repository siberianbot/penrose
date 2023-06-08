#ifndef PENROSE_ECS_COMPONENTS_COMPONENT_HPP
#define PENROSE_ECS_COMPONENTS_COMPONENT_HPP

#include <type_traits>

#include "src/ECS/ECSBase.hpp"

namespace Penrose {

    class Component {
    public:
        virtual ~Component() = default;
    };

    template<typename T>
    concept IsComponent = std::is_base_of<Component, T>::value &&
                          std::is_default_constructible<T>::value &&
                          requires(T) {{ T::name() } -> std::same_as<ComponentName>; };
}

#endif // PENROSE_ECS_COMPONENTS_COMPONENT_HPP
