#include <Penrose/Resources/ResourceSet.hpp>

#include <ranges>

#include <fmt/core.h>

#include <Penrose/Common/EngineError.hpp>
#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/Runnable.hpp>

namespace Penrose {

    void ResourceSet::initAll() {
        for (const auto &resource: this->_resources) {
            auto initializable = dynamic_cast<Initializable *>(resource.get());

            if (initializable == nullptr) {
                continue;
            }

            initializable->init();
        }
    }

    void ResourceSet::destroyAll() {
        for (const auto &resource: std::ranges::reverse_view(this->_resources)) {
            auto initializable = dynamic_cast<Initializable *>(resource.get());

            if (initializable == nullptr) {
                continue;
            }

            initializable->destroy();
        }
    }

    void ResourceSet::runAll() {
        for (const auto &resource: this->_resources) {
            auto runnable = dynamic_cast<Runnable *>(resource.get());

            if (runnable == nullptr) {
                continue;
            }

            runnable->run();
        }
    }

    void ResourceSet::stopAll() {
        for (const auto &resource: std::ranges::reverse_view(this->_resources)) {
            auto runnable = dynamic_cast<Runnable *>(resource.get());

            if (runnable == nullptr) {
                continue;
            }

            runnable->stop();
        }
    }

    ResourceSet::ResourceList::const_iterator ResourceSet::getBeginIterator() const {
        return this->_resources.begin();
    }

    ResourceSet::ResourceList::iterator ResourceSet::addToList(Resource *resource,
                                                               std::optional<ResourceList::iterator> before) {
        auto position = before.has_value() ? *before : this->_resources.end();

        return this->_resources.emplace(position, resource);
    }

    void ResourceSet::addToMap(std::type_index idx, std::list<std::unique_ptr<Resource>>::iterator it) {
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
