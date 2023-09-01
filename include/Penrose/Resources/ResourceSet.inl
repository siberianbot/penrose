#ifndef PENROSE_RESOURCES_RESOURCE_SET_INL
#define PENROSE_RESOURCES_RESOURCE_SET_INL

#include <algorithm>

namespace Penrose {

    template<IsResource T, class... Interfaces>
    T *ResourceSet::add(std::optional<ResourceList::iterator> before) {
        auto resource = this->construct<T>();
        auto iterator = this->addToList(resource, before);

        this->addToMap<T, Interfaces...>(iterator);

        return resource;
    }

    template<class T>
    std::optional<ResourceSet::ResourceList::iterator> ResourceSet::tryGetIteratorOf() const {
        auto idx = std::type_index(typeid(T));
        auto it = this->_resourceMap.find(idx);

        if (it == this->_resourceMap.end()) {
            return std::nullopt;
        }

        return it->second;
    }

    template<class T>
    T *ResourceSet::get() const {
        auto idx = std::type_index(typeid(T));

        return dynamic_cast<T *>(this->get(idx));
    }

    template<class T>
    std::vector<T *> ResourceSet::getAll() const {
        std::vector<T *> resources;
        auto idx = std::type_index(typeid(T));
        auto [begin, end] = this->_resourceMap.equal_range(idx);

        for (auto it = begin; it != end; it++) {
            resources.push_back(dynamic_cast<T *>(it->second->get()));
        }

        return resources;
    }

    template<class T>
    Lazy<T> ResourceSet::getLazy() const {
        return Lazy<T>([this]() { return this->get<T>(); });
    }

    template<class T>
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

    template<IsResource T>
    void ResourceSet::addToMap(ResourceList::iterator it) {
        this->addToMap(std::type_index(typeid(T)), it);
    }

    template<IsResource T, class TInterface, class... Interfaces>
    requires std::is_base_of<TInterface, T>::value && (!std::is_same<TInterface, T>::value)
    void ResourceSet::addToMap(ResourceList::iterator it) {
        this->addToMap(std::type_index(typeid(TInterface)), it);

        this->addToMap<T, Interfaces...>(it);
    }
}

#endif // PENROSE_RESOURCES_RESOURCE_SET_INL
