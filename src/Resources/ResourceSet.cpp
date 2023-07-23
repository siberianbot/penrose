#include <Penrose/Resources/ResourceSet.hpp>

#include <ranges>

#include <fmt/core.h>

#include <Penrose/Common/EngineError.hpp>

namespace Penrose {

    void ResourceSet::initAll() {
        for (const auto &resource: this->_resources) {
            auto initializable = dynamic_cast<InitializableResource *>(resource.get());

            if (initializable == nullptr) {
                continue;
            }

            initializable->init();
        }
    }

    void ResourceSet::destroyAll() {
        for (const auto &resource: std::ranges::reverse_view(this->_resources)) {
            auto initializable = dynamic_cast<InitializableResource *>(resource.get());

            if (initializable == nullptr) {
                continue;
            }

            initializable->destroy();
        }
    }

    void ResourceSet::add(std::type_index idx,
                          Resource *resource,
                          std::optional<ResourceList::iterator> before) {
        auto position = before.has_value() ? *before : this->_resources.end();
        auto it = this->_resources.emplace(position, resource);

        this->_resourceMap.emplace(idx, it);
    }

    Resource *ResourceSet::get(const std::type_index &idx) const {
        auto it = this->_resourceMap.find(idx);

        if (it != this->_resourceMap.end()) {
            return it->second->get();
        }

        throw EngineError(fmt::format("Required resource of type {} not found", idx.name()));
    }
}
