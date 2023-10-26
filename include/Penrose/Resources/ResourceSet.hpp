#ifndef PENROSE_RESOURCES_RESOURCE_SET_HPP
#define PENROSE_RESOURCES_RESOURCE_SET_HPP

#include <list>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <typeindex>
#include <typeinfo>
#include <type_traits>

#include <Penrose/Api.hpp>
#include <Penrose/Common/EngineError.hpp>
#include <Penrose/Resources/Resource.hpp>
#include <Penrose/Utils/OptionalUtils.hpp>

namespace Penrose {

    class PENROSE_API ResourceSet {
    public:
        template<typename Target>
        class Proxy {
        public:
            using Iterator = std::list<Target *>::iterator;

            explicit Proxy(ResourceSet *resources)
                    : _resources(resources),
                      _instances(std::nullopt) {
                //
            }

            [[nodiscard]] bool isPresent() {
                this->resolve();

                return this->_instances.has_value() && !this->_instances->empty();
            }

            // TODO: should not be available to caller
            [[nodiscard]] Target *get() {
                if (!this->isPresent()) {
                    throw EngineError("Resource {} is not presented in resource set", demangle(typeid(Target).name()));
                }

                return this->_instances->front();
            }

            [[nodiscard]] Iterator begin() {
                this->resolve();

                return this->_instances->begin();
            }

            [[nodiscard]] Iterator end() {
                this->resolve();

                return this->_instances->end();
            }

            [[nodiscard]] Target *operator->() {
                return this->get();
            }

        private:
            ResourceSet *_resources;
            std::optional<std::list<Target *>> _instances;

            void resolve() {
                if (this->_instances.has_value()) {
                    return;
                }

                this->_instances = this->_resources->template resolve<Target>();
            }
        };

        template<typename Target, ResourceGroup Group> requires IsResource<Target, Group>
        class Registration {
        public:
            explicit Registration(ResourceSet *resources)
                    : _resources(resources),
                      _implements({typeid(Target)}),
                      _before(std::nullopt) {
                //
            }

            template<typename Type>
            [[nodiscard]] Registration &before() {
                this->_before = typeid(Type);

                return *this;
            }

            template<typename Interface>
            requires std::is_base_of_v<Interface, Target>
            [[nodiscard]] Registration &implements() {
                this->_implements.insert(typeid(Interface));

                return *this;
            }

            Target *done() {
                return this->_resources->template insert<Target, Group>(
                        std::forward<decltype(this->_implements)>(this->_implements),
                        std::forward<decltype(this->_before)>(this->_before));
            }

        private:
            ResourceSet *_resources;
            std::set<std::type_index> _implements;
            std::optional<std::type_index> _before;
        };

        ResourceSet() = default;

        template<typename Target>
        [[nodiscard]] Proxy<Target> get() {
            return Proxy<Target>(this);
        }

        template<typename Target, ResourceGroup Group>
        [[nodiscard]] Registration<Target, Group> add() {
            return Registration<Target, Group>(this);
        }

    private:
        using ResourceList = std::list<std::unique_ptr<ResourceBase>>;

        ResourceList _instances;
        std::map<std::type_index, std::map<ResourceGroup, std::list<ResourceList::iterator>>> _typeMap;

        template<typename Target>
        [[nodiscard]] std::list<Target *> resolve() {
            std::type_index idx = typeid(Target);

            auto it = this->_typeMap.find(idx);

            if (it == this->_typeMap.end()) {
                return {};
            }

            std::list<Target *> instances;

            for (const auto &[_, list]: it->second) {
                for (const auto &ptr: list) {
                    instances.emplace_back(dynamic_cast<Target *>(ptr->get()));
                }
            }

            return instances;
        }

        template<typename Target, ResourceGroup Group>
        requires IsResource<Target, Group>
        [[nodiscard]] constexpr Target *construct() {
            if constexpr (std::is_constructible_v<Target, ResourceSet *>) {
                return Target::create(this);
            } else {
                return Target::create();
            }
        }

        template<typename Target, ResourceGroup Group>
        Target *insert(std::set<std::type_index> &&implements,
                       std::optional<std::type_index> &&before) {

            auto instance = this->construct<Target, Group>();
            this->insert(std::unique_ptr<ResourceBase>(instance),
                         Group,
                         std::forward<decltype(implements)>(implements),
                         std::forward<decltype(before)>(before));

            return instance;
        }

        void insert(std::unique_ptr<ResourceBase> &&instance,
                    ResourceGroup group,
                    std::set<std::type_index> &&implements,
                    std::optional<std::type_index> &&before);
    };

    template<typename Target>
    using ResourceProxy = ResourceSet::Proxy<Target>;
}

#endif // PENROSE_RESOURCES_RESOURCE_SET_HPP
