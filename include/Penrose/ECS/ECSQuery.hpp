#ifndef PENROSE_ECS_ECS_QUERY_HPP
#define PENROSE_ECS_ECS_QUERY_HPP

#include <functional>
#include <vector>

#include <Penrose/ECS/ECSEntry.hpp>

namespace Penrose {

    using ECSQueryPredicate = std::function<bool(const ECSEntry &)>;

    class ECSQuery {
    public:
        constexpr ECSQuery() = default;

        template<IsComponent T>
        [[nodiscard]] constexpr ECSQuery &component() {
            auto name = T::name();
            this->_predicates.emplace_back([name](const ECSEntry &entry) {
                return entry.componentName == name;
            });

            return *this;
        }

        [[nodiscard]] constexpr ECSQuery &entity(Entity entity) {
            this->_predicates.emplace_back([entity](const ECSEntry &entry) {
                return entry.entity == entity;
            });

            return *this;
        }

        [[nodiscard]] constexpr const std::vector<ECSQueryPredicate> &getPredicates() const {
            return this->_predicates;
        }

    private:
        std::vector<ECSQueryPredicate> _predicates;
    };
}

#endif // PENROSE_ECS_ECS_QUERY_HPP
