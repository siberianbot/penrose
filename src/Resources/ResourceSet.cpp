#include <Penrose/Resources/ResourceSet.hpp>

#include <ranges>

#include <fmt/core.h>

#include <Penrose/Common/EngineError.hpp>

namespace Penrose {

    Resource *ResourceSet::add(const std::type_index &type, ResourceSet::ResourcePtr &&resource, bool isInitializable) {
        auto it = this->_types.find(type);

        if (it != this->_types.end()) {
            throw EngineError(fmt::format("Attempt to override resource of type {}", type.name()));
        }

        this->_resources.push_back(std::move(resource));

        auto ptr = &this->_resources.back();

        this->_types.emplace(type, ptr);

        if (isInitializable) {
            this->_initializables.push_back(ptr);
        }

        return ptr->get();
    }

    std::optional<Resource *> ResourceSet::tryGet(const std::type_index &type) const noexcept {
        auto it = this->_types.find(type);

        if (it == this->_types.end()) {
            return std::nullopt;
        }

        return it->second->get();
    }

    Resource *ResourceSet::get(const std::type_index &type) const {
        auto resource = this->tryGet(type);

        if (!resource.has_value()) {
            throw EngineError(fmt::format("Required resource of type {} not found", type.name()));
        }

        return resource.value();
    }

    void ResourceSet::initAll() {
        for (const auto &resource: this->_initializables) {
            dynamic_cast<Initializable *>(resource->get())->init();
        }
    }

    void ResourceSet::destroyAll() {
        for (const auto &resource: std::ranges::reverse_view(this->_initializables)) {
            dynamic_cast<Initializable *>(resource->get())->destroy();
        }
    }
}
