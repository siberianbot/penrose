#ifndef PENROSE_RESOURCES_RESOURCE_SET_HPP
#define PENROSE_RESOURCES_RESOURCE_SET_HPP

#include <list>
#include <map>
#include <memory>
#include <optional>
#include <typeindex>
#include <type_traits>
#include <vector>

#include <Penrose/Resources/Resource.hpp>

namespace Penrose {

    class ResourceSet;

    template<typename T>
    concept IsDefaultConstructableResource = IsResource<T> &&
                                             std::is_default_constructible<T>::value;

    template<typename T>
    concept IsConstructableWithResourceSetResource = IsResource<T> &&
                                                     (requires(ResourceSet *resources) {
                                                         T(resources);
                                                     });

    class ResourceSet {
    public:
        using ResourceList = std::list<std::unique_ptr<Resource>>;

        void initAll();
        void destroyAll();

        template<IsResource TBase, IsResource TImpl = TBase>
        requires std::is_base_of<TBase, TImpl>::value
        TBase *add(std::optional<ResourceList::iterator> before = std::nullopt);

        template<IsResource T>
        [[nodiscard]] T *get() const;

        template<IsResource T>
        [[nodiscard]] std::vector<T *> getAll() const;

    private:
        ResourceList _resources;
        std::multimap<std::type_index, ResourceList::iterator> _resourceMap;

        template<IsResource T>
        [[nodiscard]] constexpr T *construct();

        void add(std::type_index idx,
                 Resource *resource,
                 std::optional<ResourceList::iterator> before);
        [[nodiscard]] Resource *get(const std::type_index &idx) const;
    };
}

#include "ResourceSet.inl"

#endif // PENROSE_RESOURCES_RESOURCE_SET_HPP
