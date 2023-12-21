#ifndef PENROSE_RESOURCES_RESOURCE_SET_HPP
#define PENROSE_RESOURCES_RESOURCE_SET_HPP

#include <list>
#include <map>
#include <memory>
#include <optional>
#include <ranges>
#include <set>
#include <type_traits>
#include <typeindex>
#include <typeinfo>

#include <Penrose/Common/EngineError.hpp>
#include <Penrose/Resources/Resource.hpp>
#include <Penrose/Resources/ResourceGroup.hpp>
#include <Penrose/Utils/TypeUtils.hpp>

namespace Penrose {

    /**
     * \brief Resource set
     * \details Resource set is used as dependency container for any Resource<> inheritor and provides a single lifetime
     * for each resource instance.
     */
    class PENROSE_API ResourceSet {
    public:
        /**
         * \brief Wrapper around resource instances with lazy resolution
         * \details Internally, every proxies tries to resolve type instances from container. That's why instances of
         * this type are not intended to be constant.
         * \tparam Target Type of proxied resource
         */
        template <typename Target>
        class Proxy {
        public:
            using InstanceList = std::list<Target *>;
            using Iterator = typename InstanceList::iterator;

            /**
             * \brief Create proxy in non-resolved state (instance resolve will occur on first usage)
             * \param resources Pointer to resource set instance
             */
            explicit Proxy(const ResourceSet *resources)
                : _resources(resources),
                  _instances(std::nullopt) {
                //
            }

            /**
             * \brief Create proxy in resolved state (instance resolve will not occur anymore)
             * \param instances Collection of instances
             */
            explicit Proxy(InstanceList &&instances)
                : _resources(nullptr),
                  _instances(instances) {
                //
            }

            Proxy(const Proxy &) = default;
            Proxy(Proxy &&) = default;
            Proxy &operator=(const Proxy &) = default;
            Proxy &operator=(Proxy &&) = default;

            /**
             * \brief Resolve instances and check if any instance of target type is available in source dependency
             * container
             * \return Instance presence in dependency container
             */
            [[nodiscard]] bool isPresent() {
                this->resolve();

                return this->_instances.has_value() && !this->_instances->empty();
            }

            /**
             * \brief Resolve instances and return begin iterator over collection of resolved instances
             * \return Begin iterator of resolved instances collection
             */
            [[nodiscard]] Iterator begin() {
                this->resolve();

                return this->_instances->begin();
            }

            /**
             * \brief Resolve instances and return end iterator over collection of resolved instances
             * \return End iterator of resolved instances collection
             */
            [[nodiscard]] Iterator end() {
                this->resolve();

                return this->_instances->end();
            }

            /**
             * \brief Resolve instances and return pointer to first instance
             * \return First resolved instance
             * \note It is not recommended to use this method to pass instances to child objects, copy/move proxy
             * instead
             */
            [[nodiscard]] Target *get() {
                if (!this->isPresent()) {
                    throw EngineError("No instances of type {} available", getTypeName<Target>());
                }

                return this->_instances->front();
            }

            /**
             * \brief Resolve instances and return pointer to first instance
             * \return First resolved instance
             */
            [[nodiscard]] Target *operator->() { return this->get(); }

        private:
            const ResourceSet *_resources;
            std::optional<std::list<Target *>> _instances;

            void resolve() {
                if (this->_instances.has_value() || this->_resources == nullptr) {
                    return;
                }

                this->_instances = this->_resources->tryResolveAll<Target>();
            }
        };

        /**
         * \brief Resource registration data for dependency container
         * \tparam Target Target type of resource registration
         */
        template <IsResource Target>
        class Registration {
        public:
            explicit Registration(ResourceSet *resources)
                : _resources(resources),
                  _group(ResourceGroup::Custom),
                  _types({typeid(Target)}) {
                //
            }

            /**
             * \brief Set required resource group for resource registration
             * \param group Required resource group
             * \return This instance
             */
            Registration &group(ResourceGroup group) {
                this->_group = group;

                return *this;
            }

            /**
             * \brief Add implemented type for resource registration
             * \tparam Interface Implemented type
             * \return This instance
             */
            template <typename Interface>
            requires std::is_base_of_v<Interface, Target>
            Registration &implements() {
                this->_types.emplace(typeid(Interface));

                return *this;
            }

            /**
             * \brief Complete resource registration and add instance of resource into dependency container
             * \return Instance of target
             */
            Target *done() {
                auto instance = this->construct();

                this->_resources->insert(std::move(this->_types), this->_group, std::unique_ptr<Target>(instance));

                return instance;
            }

        private:
            ResourceSet *_resources;
            ResourceGroup _group;
            std::set<std::type_index> _types;

            [[nodiscard]] constexpr Target *construct() const {
                if constexpr (std::is_constructible_v<Target, const ResourceSet *>) {
                    return Target::create(this->_resources);
                } else {
                    return Target::create();
                }
            }
        };

        ResourceSet() = default;

        ResourceSet(const ResourceSet &) = delete;
        ResourceSet(ResourceSet &&) = delete;
        ResourceSet &operator=(const ResourceSet &) = delete;
        ResourceSet &operator=(ResourceSet &&) = delete;

        /**
         * \brief Get proxy of resource or implemented interface from dependency container
         * \tparam Target Type of required resource or implemented interface
         * \return Instance of type Target
         */
        template <typename Target>
        [[nodiscard]] Proxy<Target> get() const {
            return Proxy<Target>(this);
        }

        /**
         * \brief Try to resolve one instance of target type
         * \tparam Target Target type of resource (may be interface)
         * \param type Concrete type of resource (optional)
         * \return First available instance of target type or nothing
         */
        template <typename Target>
        [[nodiscard]] std::optional<Target *> tryResolveOne(std::optional<std::type_index> &&type = std::nullopt)
            const noexcept {
            const auto targetType = type.value_or(typeid(Target));
            const auto map = this->tryResolveByType(targetType);

            if (!map.has_value() || (*map)->empty()) {
                return std::nullopt;
            }

            const auto &list = (*map)->begin()->second;

            if (list.empty()) {
                return std::nullopt;
            }

            const auto it = list.begin();

            return dynamic_cast<Target *>((*it)->get());
        }

        /**
         * \brief Resolve one instance of target type or throw exception if no instances available
         * \tparam Target Target type of resource (may be interface)
         * \param type Concrete type of resource (optional)
         * \return First available instance of target type
         */
        template <typename Target>
        [[nodiscard]] Target *resolveOne(std::optional<std::type_index> &&type = std::nullopt) const {
            auto instance = this->tryResolveOne<Target>(std::forward<decltype(type)>(type));

            if (!instance.has_value()) {
                throw EngineError("No instances of type {} available", getTypeName<Target>());
            }

            return *instance;
        }

        /**
         * \brief Resolve all instances of target type
         * \tparam Target Target type
         * \return Collection of instances of target type
         */
        template <typename Target>
        [[nodiscard]] std::list<Target *> tryResolveAll() const noexcept {
            const auto map = this->tryResolveByType(typeid(Target));

            if (!map.has_value()) {
                return {};
            }

            std::list<Target *> instances;

            for (const auto &list: **map | std::views::values) {
                for (const auto &ptr: list) {
                    instances.push_back(dynamic_cast<Target *>(ptr->get()));
                }
            }

            return instances;
        }

        /**
         * \brief Begin addition of resource in dependency container
         * \tparam Target Type of resource
         * \return Registration of resource
         */
        template <IsResource Target>
        [[nodiscard]] Registration<Target> add() {
            return Registration<Target>(this);
        }

    private:
        using InstanceList = std::list<std::unique_ptr<ResourceBase>>;
        using GroupMap = std::map<ResourceGroup, std::list<InstanceList::iterator>>;

        InstanceList _instances;
        std::map<std::type_index, GroupMap> _typeMap;

        void insert(
            std::set<std::type_index> &&types, ResourceGroup group, std::unique_ptr<ResourceBase> &&instance
        ) noexcept;

        [[nodiscard]] std::optional<const GroupMap *> tryResolveByType(std::type_index type) const noexcept;
    };

    //! \copydoc ResourceSet::Proxy
    template <typename Target>
    using ResourceProxy = ResourceSet::Proxy<Target>;
}

#endif // PENROSE_RESOURCES_RESOURCE_SET_HPP
