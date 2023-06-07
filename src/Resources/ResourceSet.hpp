#ifndef PENROSE_RESOURCES_RESOURCE_SET_HPP
#define PENROSE_RESOURCES_RESOURCE_SET_HPP

#include <list>
#include <map>
#include <memory>
#include <optional>
#include <typeindex>
#include <type_traits>
#include <vector>

#include "src/Common/Initializable.hpp"
#include "src/Common/Lazy.hpp"
#include "src/Resources/Resource.hpp"

namespace Penrose {

    class ResourceSet;

    template<typename T>
    concept IsResource = std::is_base_of<Resource, T>::value;

    template<typename T>
    concept IsDefaultConstructableResource = IsResource<T> && std::is_default_constructible<T>::value;

    template<typename T>
    concept IsConstructableWithResourceSetResource = IsResource<T> && requires(ResourceSet *resources) {
        T(resources);
    };

    template<IsResource T>
    constexpr bool isInitializable() {
        return std::is_base_of<Initializable, T>::value;
    }

    class ResourceSet {
    private:
        using ResourcePtr = std::unique_ptr<Resource>;
        using ResourceList = std::list<ResourcePtr>;

        ResourceList _resources;
        std::map<std::type_index, ResourceList::pointer> _types;
        std::vector<ResourceList::pointer> _initializables;

        Resource *add(const std::type_index &type, ResourceSet::ResourcePtr &&resource, bool isInitializable);

        [[nodiscard]] std::optional<Resource *> tryGet(const std::type_index &type) const noexcept;
        [[nodiscard]] Resource *get(const std::type_index &type) const;

    public:
        template<IsDefaultConstructableResource T>
        T *add();

        template<IsConstructableWithResourceSetResource T>
        T *add();

        template<typename T>
        [[nodiscard]] std::optional<T *> tryGet() const noexcept;

        template<typename T>
        [[nodiscard]] T *get() const;

        template<typename T>
        [[nodiscard]] Lazy<T> getLazy() const noexcept;

        void initAll();
        void destroyAll();
    };
}

#include "ResourceSet.inl"

#endif // PENROSE_RESOURCES_RESOURCE_SET_HPP
