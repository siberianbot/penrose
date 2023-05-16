#ifndef PENROSE_CORE_RESOURCE_SET_HPP
#define PENROSE_CORE_RESOURCE_SET_HPP

#include <list>
#include <map>
#include <memory>
#include <optional>
#include <typeinfo>
#include <typeindex>
#include <type_traits>

#include <fmt/core.h>

#include "src/Common/EngineError.hpp"
#include "src/Core/Resource.hpp"

namespace Penrose {

    class ResourceSet;

    template<typename T>
    concept IsResourceImpl = std::is_base_of<Resource, T>::value;

    template<typename T>
    concept IsDefaultConstructableResource = IsResourceImpl<T> && std::is_default_constructible<T>::value;

    template<typename T>
    concept IsConstructableWithResourceSetResource = IsResourceImpl<T> && requires(ResourceSet *resources) {
        T(resources);
    };

    class ResourceSet {
    private:
        using ResourceList = std::list<std::shared_ptr<Resource>>;

        std::map<std::type_index, ResourceList::reference> _types;
        ResourceList _resources;

        void add(const std::type_index &type, const std::shared_ptr<Resource> &resource);
        std::optional<std::shared_ptr<Resource>> tryGet(const std::type_index &type);

    public:
        template<IsDefaultConstructableResource T>
        void add() {
            this->add(std::type_index(typeid(T)), std::make_shared<T>());
        }

        template<IsConstructableWithResourceSetResource T>
        void add() {
            this->add(std::type_index(typeid(T)), std::make_shared<T>(this));
        }

        template<IsResourceImpl T>
        std::optional<std::weak_ptr<T>> tryGet() noexcept {
            auto idx = std::type_index(typeid(T));
            auto resource = this->tryGet(idx);

            if (!resource.has_value()) {
                return std::nullopt;
            }

            return std::dynamic_pointer_cast<T>(resource.value());
        }

        template<IsResourceImpl T>
        std::optional<std::weak_ptr<T>> get() {
            auto idx = std::type_index(typeid(T));
            auto resource = this->tryGet(idx);

            if (!resource.has_value()) {
                throw EngineError(fmt::format("Required resource of type {} not found", idx.name()));
            }

            return std::dynamic_pointer_cast<T>(resource.value());
        }

        void initAll();
        void destroyAll();
    };
}

#endif // PENROSE_CORE_RESOURCE_SET_HPP
