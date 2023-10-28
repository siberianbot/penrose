#ifndef PENROSE_RESOURCES_RESOURCE_HPP
#define PENROSE_RESOURCES_RESOURCE_HPP

#include <string>
#include <typeindex>
#include <typeinfo>

#include <Penrose/Api.hpp>
#include <Penrose/Utils/TypeUtils.hpp>

namespace Penrose {

    enum class ResourceGroup {
        Engine,
        Performance,
        Backend,
        Windowing,
        Rendering,
        RenderOperator,
        UI,
        Assets,
        Events,
        ECSManager,
        Scene,
        ECSComponent,
        ECSSystem,
        Custom
    };

    struct ResourceInfo {
        std::type_index type;
        ResourceGroup group;
        std::string name;
    };

    class PENROSE_API ResourceBase {
    public:
        virtual ~ResourceBase() = default;

        [[nodiscard]] virtual ResourceInfo getType() const = 0;
    };

    template<typename Self, ResourceGroup Group>
    class PENROSE_API Resource : public ResourceBase {
    public:
        Resource() = default;
        Resource(const Resource &) = delete;
        Resource(Resource &&) = delete;
        Resource &operator=(const Resource &) = delete;
        Resource &operator=(Resource &&) = delete;

        ~Resource() override = default;

        [[nodiscard]] ResourceInfo getType() const final { return type(); }

        [[nodiscard]] constexpr static Self *create() {
            static_assert(std::is_default_constructible_v<Self>);

            return new Self();
        }

        template<typename ...Args>
        [[nodiscard]] constexpr static Self *create(Args &&...args) {
            static_assert(std::is_constructible_v<Self, Args...>);

            return new Self(std::forward<decltype(args)>(args)...);
        }

        [[nodiscard]] /* TODO: constexpr */ static ResourceInfo type() {
            std::type_index type = typeid(Self);

            return {
                    .type = type,
                    .group = Group,
                    .name = demangle(type.name())
            };
        }
    };

    template<typename Target, ResourceGroup Group>
    concept IsResource = std::is_base_of_v<Resource<Target, Group>, Target>;
}

#endif // PENROSE_RESOURCES_RESOURCE_HPP
