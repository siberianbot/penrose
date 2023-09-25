#ifndef PENROSE_ECS_COMPONENT_HPP
#define PENROSE_ECS_COMPONENT_HPP

#include <string>
#include <string_view>
#include <type_traits>

namespace Penrose {

    class Component {
    public:
        virtual ~Component() = default;

        [[nodiscard]] virtual std::string getName() const = 0;
    };

    template<typename T>
    concept IsComponent = std::is_base_of<Component, T>::value &&
                          requires(T) {{ T::name() } -> std::same_as<std::string_view>; };
}

#endif // PENROSE_ECS_COMPONENT_HPP
