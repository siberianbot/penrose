#ifndef PENROSE_ECS_COMPONENT_FACTORY_HPP
#define PENROSE_ECS_COMPONENT_FACTORY_HPP

#include <string>
#include <type_traits>

#include <Penrose/ECS/Component.hpp>

namespace Penrose {

    class ComponentFactory {
    public:
        virtual ~ComponentFactory() = default;

        [[nodiscard]] virtual std::string getName() const = 0;

        [[nodiscard]] virtual Component *makeComponent() = 0;
    };

    template<IsComponent T> requires std::is_default_constructible<T>::value
    class GenericComponentFactory : public ComponentFactory {
    public:
        ~GenericComponentFactory() override = default;

        [[nodiscard]] std::string getName() const override { return T::name(); }

        [[nodiscard]] Component *makeComponent() override { return new T(); }
    };
}

#endif // PENROSE_ECS_COMPONENT_FACTORY_HPP
