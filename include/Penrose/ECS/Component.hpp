#ifndef PENROSE_ECS_COMPONENT_HPP
#define PENROSE_ECS_COMPONENT_HPP

#include <string>
#include <typeindex>
#include <typeinfo>

#include <Penrose/Utils/TypeUtils.hpp>

namespace Penrose {

    struct ComponentInfo {
        std::type_index type;
        std::string name;
    };

    class ComponentBase {
    public:
        virtual ~ComponentBase() = default;

        [[nodiscard]] virtual ComponentInfo getType() const = 0;
    };

    template<typename Self>
    class Component : public ComponentBase {
    public:
        ~Component() override = default;

        [[nodiscard]] ComponentInfo getType() const final { return type(); }

        [[nodiscard]] constexpr static Self *create() {
            static_assert(std::is_default_constructible_v<Self>);

            return new Self();
        }

        template<typename ...Args>
        [[nodiscard]] constexpr static Self *create(Args &&...args) {
            static_assert(std::is_constructible_v<Self, Args...>);

            return new Self(std::forward<decltype(args)>(args)...);
        }

        [[nodiscard]] /* TODO: constexpr */ static ComponentInfo type() {
            std::type_index type = typeid(Self);

            return {
                    .type = type,
                    .name = demangle(type.name())
            };
        }
    };
}

#endif // PENROSE_ECS_COMPONENT_HPP
