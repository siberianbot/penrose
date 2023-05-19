#ifndef PENROSE_RESOURCES_RESOURCE_SET_INL
#define PENROSE_RESOURCES_RESOURCE_SET_INL

namespace Penrose {

    template<IsDefaultConstructableResource T>
    void ResourceSet::add() {
        this->add(std::type_index(typeid(T)), std::make_unique<T>(), isInitializable<T>());
    }

    template<IsConstructableWithResourceSetResource T>
    void ResourceSet::add() {
        this->add(std::type_index(typeid(T)), std::make_unique<T>(this), isInitializable<T>());
    }

    template<typename T>
    std::optional<T *> ResourceSet::tryGet() const noexcept {
        auto type = std::type_index(typeid(T));
        auto resource = this->tryGet(type);

        if (!resource.has_value()) {
            return std::nullopt;
        }

        return dynamic_cast<T *>(resource.value());
    }

    template<typename T>
    T *ResourceSet::get() const {
        auto type = std::type_index(typeid(T));

        return dynamic_cast<T *>(this->get(type));
    }

    template<typename T>
    Lazy<T> ResourceSet::getLazy() const noexcept {
        return Lazy<T>([this]() { return this->get<T>(); });
    }
}

#endif // PENROSE_RESOURCES_RESOURCE_SET_INL
