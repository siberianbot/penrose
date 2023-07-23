#ifndef PENROSE_RESOURCES_RESOURCE_SET_INL
#define PENROSE_RESOURCES_RESOURCE_SET_INL

namespace Penrose {

    template<IsResource TBase, IsResource TImpl>
    requires std::is_base_of<TBase, TImpl>::value
    TBase *ResourceSet::add(std::optional<ResourceList::iterator> before) {
        auto idx = std::type_index(typeid(TBase));
        auto resource = this->construct<TImpl>();

        this->add(idx, resource, before);

        return resource;
    }

    template<IsResource T>
    T *ResourceSet::get() const {
        auto idx = std::type_index(typeid(T));

        return dynamic_cast<T *>(this->get(idx));
    }

    template<IsResource T>
    constexpr T *ResourceSet::construct() {
        static_assert(IsDefaultConstructableResource<T> || IsConstructableWithResourceSetResource<T>,
                      "Resource is not constructable");

        if constexpr (IsConstructableWithResourceSetResource<T>) {
            return new T(this);
        } else {
            return new T();
        }
    }
}

#endif // PENROSE_RESOURCES_RESOURCE_SET_INL
