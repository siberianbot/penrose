#ifndef PENROSE_RESOURCES_RESOURCE_SET_INL
#define PENROSE_RESOURCES_RESOURCE_SET_INL

#include <algorithm>

namespace Penrose {

    template<IsResource TBase, IsResource TImpl>
    requires std::is_base_of<TBase, TImpl>::value
    TBase *ResourceSet::add(std::optional<ResourceList::iterator> before) {
        auto resource = this->construct<TImpl>();
        auto iterator = this->addToList(resource, before);

        auto baseIdx = std::type_index(typeid(TBase));
        this->addToMap(baseIdx, iterator);

        if constexpr (!std::is_same_v<TBase, TImpl>) {
            auto implIdx = std::type_index(typeid(TImpl));
            this->addToMap(implIdx, iterator);
        }

        return resource;
    }

    template<IsResource T>
    std::optional<ResourceSet::ResourceList::iterator> ResourceSet::tryGetIteratorOf() const {
        auto idx = std::type_index(typeid(T));
        auto it = this->_resourceMap.find(idx);

        if (it == this->_resourceMap.end()) {
            return std::nullopt;
        }

        return it->second;
    }

    template<IsResource T>
    T *ResourceSet::get() const {
        auto idx = std::type_index(typeid(T));

        return dynamic_cast<T *>(this->get(idx));
    }

    template<IsResource T>
    std::vector<T *> ResourceSet::getAll() const {
        std::vector<T *> resources;
        auto idx = std::type_index(typeid(T));
        auto [begin, end] = this->_resourceMap.equal_range(idx);

        for (auto it = begin; it != end; it++) {
            resources.push_back(dynamic_cast<T *>(it->second->get()));
        }

        return resources;
    }

    template<IsResource T>
    Lazy<T> ResourceSet::getLazy() const {
        return Lazy<T>([this]() { return this->get<T>(); });
    }

    template<IsResource T>
    LazyCollection<T> ResourceSet::getAllLazy() const {
        return LazyCollection<T>([this]() { return this->getAll<T>(); });
    }

    template<IsResource T>
    requires IsDefaultConstructableResource<T> || IsConstructableWithResourceSetResource<T>
    constexpr T *ResourceSet::construct() {
        if constexpr (IsConstructableWithResourceSetResource<T>) {
            return new T(this);
        } else {
            return new T();
        }
    }
}

#endif // PENROSE_RESOURCES_RESOURCE_SET_INL
