#ifndef PENROSE_ECS_SYSTEM_HPP
#define PENROSE_ECS_SYSTEM_HPP

#include <string_view>
#include <type_traits>

namespace Penrose {

    class ResourceSet;

    class System {
    public:
        virtual ~System() = default;

        virtual void init() { /* nothing to do */ }

        virtual void update() { /* nothing to do */ }

        virtual void destroy() { /* nothing to do */ }
    };

    template<typename T>
    concept IsSystem = std::is_base_of<System, T>::value &&
                       (requires(ResourceSet *resources, T) {
                           { T::name() } -> std::same_as<std::string_view>;
                           { T(resources) };
                       });
}

#endif // PENROSE_ECS_SYSTEM_HPP
