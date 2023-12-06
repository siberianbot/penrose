#include <Penrose/Resources/ResourceSet.hpp>

namespace Penrose {

    void ResourceSet::insert(
        std::set<std::type_index> &&types, ResourceGroup group, std::unique_ptr<ResourceBase> &&instance
    ) noexcept {
        const auto it = this->_instances.insert(this->_instances.end(), std::forward<decltype(instance)>(instance));

        for (const auto &type: types) {
            this->_typeMap[type][group].emplace_back(it);
        }
    }

    std::optional<const ResourceSet::GroupMap *> ResourceSet::tryResolveByType(const std::type_index type
    ) const noexcept {
        const auto it = this->_typeMap.find(type);

        if (it == this->_typeMap.end()) {
            return std::nullopt;
        }

        return &it->second;
    }
}
