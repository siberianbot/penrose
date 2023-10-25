#ifndef PENROSE_ECS_COMPONENT_FACTORY_HPP
#define PENROSE_ECS_COMPONENT_FACTORY_HPP

#include <string>
#include <type_traits>

#include <Penrose/Api.hpp>
#include <Penrose/ECS/Component.hpp>
#include <Penrose/Resources/Resource.hpp>

namespace Penrose {

    class PENROSE_API ComponentFactory {
    public:
        virtual ~ComponentFactory() = default;

        [[nodiscard]] virtual ComponentInfo getComponentType() const = 0;

        [[nodiscard]] virtual ComponentBase *makeComponent() = 0;
    };

    template<typename C> requires std::is_base_of_v<Component<C>, C> && std::is_default_constructible_v<C>
    class PENROSE_API GenericComponentFactory : public Resource<GenericComponentFactory<C>>,
                                                public ComponentFactory {
    public:
        ~GenericComponentFactory() override = default;

        [[nodiscard]] ComponentInfo getComponentType() const override { return C::type(); }

        [[nodiscard]] ComponentBase *makeComponent() override { return C::create(); }
    };
}

#endif // PENROSE_ECS_COMPONENT_FACTORY_HPP
