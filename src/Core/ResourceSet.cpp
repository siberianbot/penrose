#include "ResourceSet.hpp"

#include <ranges>

namespace Penrose {

    void ResourceSet::add(const std::type_index &type, const std::shared_ptr<Resource> &resource) {
        auto it = this->_types.find(type);

        if (it != this->_types.end()) {
            throw EngineError(fmt::format("Attempt to override resource of type {}", type.name()));
        }

        this->_resources.push_back(resource);
        this->_types.emplace(type, this->_resources.back());
    }

    std::optional<std::shared_ptr<Resource>> ResourceSet::tryGet(const std::type_index &type) {
        auto it = this->_types.find(type);

        if (it == this->_types.end()) {
            return std::nullopt;
        }

        return it->second;
    }

    void ResourceSet::initAll() {
        for (auto &resource: this->_resources) {
            resource->init();
        }
    }

    void ResourceSet::destroyAll() {
        for (auto &resource: std::ranges::reverse_view(this->_resources)) {
            resource->destroy();
        }
    }
}
