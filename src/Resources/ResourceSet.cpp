#include <Penrose/Resources/ResourceSet.hpp>

#include <Penrose/Common/EngineError.hpp>

namespace Penrose {

    void ResourceSet::insert(std::unique_ptr<ResourceBase> &&instance,
                             ResourceGroup group,
                             std::set<std::type_index> &&implements,
                             std::optional<std::type_index> &&before) {

        auto iterator = this->_instances.insert(this->_instances.end(), std::forward<decltype(instance)>(instance));

        for (const auto &implementType: implements) {
            auto &implementations = this->_typeMap[implementType][group];
            using IteratorType = decltype(implementations.begin());

            auto position = flatMap(
                    before, [&implementations](const std::type_index &type) -> std::optional<IteratorType> {
                        for (auto it = implementations.begin();
                             it != implementations.end(); it++) {
                            if ((**it)->getType().type != type) {
                                continue;
                            }

                            return it;
                        }

                        return std::nullopt;
                    });

            implementations.insert(position.value_or(implementations.end()), iterator);
        }
    }
}
