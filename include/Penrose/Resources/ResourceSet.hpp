#ifndef PENROSE_RESOURCES_RESOURCE_SET_HPP
#define PENROSE_RESOURCES_RESOURCE_SET_HPP

#include <list>
#include <map>
#include <memory>
#include <optional>
#include <typeindex>
#include <type_traits>
#include <vector>

#include <Penrose/Resources/Lazy.hpp>
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

        void runAll();
        void stopAll();

        template<IsResource T, class ...Interfaces>
        T *add(std::optional<ResourceList::iterator> before = std::nullopt);

        template<class T>
        [[nodiscard]] std::optional<ResourceList::iterator> tryGetIteratorOf() const;

        [[nodiscard]] ResourceList::const_iterator getBeginIterator() const;

        template<class T>
        [[nodiscard]] T *get() const;

        template<class T>
        [[nodiscard]] std::vector<T *> getAll() const;

        template<class T>
        [[nodiscard]] Lazy<T> getLazy() const;

        template<class T>
        [[nodiscard]] LazyCollection<T> getAllLazy() const;

    private:
        ResourceList _resources;
        std::multimap<std::type_index, ResourceList::iterator> _resourceMap;

        template<IsResource T>
        requires IsDefaultConstructableResource<T> || IsConstructableWithResourceSetResource<T>
        [[nodiscard]] constexpr T *construct();

        ResourceList::iterator addToList(Resource *resource, std::optional<ResourceList::iterator> before);

        template<IsResource T>
        void addToMap(ResourceList::iterator it);

        template<IsResource T, class TInterface, class ...Interfaces>
        requires std::is_base_of<TInterface, T>::value && (!std::is_same<TInterface, T>::value)
        void addToMap(ResourceList::iterator it);

        void addToMap(std::type_index idx, ResourceList::iterator it);

        [[nodiscard]] Resource *get(const std::type_index &idx) const;
    };
}

#include "ResourceSet.inl"

#endif // PENROSE_RESOURCES_RESOURCE_SET_HPP
