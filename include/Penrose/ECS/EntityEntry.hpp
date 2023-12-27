#ifndef PENROSE_ECS_ENTITY_ENTRY_HPP
#define PENROSE_ECS_ENTITY_ENTRY_HPP

#include <memory>

#include <Penrose/ECS/Component.hpp>
#include <Penrose/ECS/Entity.hpp>

namespace Penrose {

    /**
     * \brief Result of EntityIterator
     */
    struct PENROSE_API EntityEntry {

        /**
         * \brief Entity
         */
        Entity entity;

        /**
         * \brief Type of component
         */
        ComponentType componentType;

        /**
         * \brief Component instance
         */
        std::shared_ptr<ComponentPtr> component;
    };
}

#endif // PENROSE_ECS_ENTITY_ENTRY_HPP
