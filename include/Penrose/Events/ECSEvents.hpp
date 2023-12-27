#ifndef PENROSE_EVENTS_ECS_EVENTS_HPP
#define PENROSE_EVENTS_ECS_EVENTS_HPP

#include <memory>

#include <Penrose/ECS/Component.hpp>
#include <Penrose/ECS/Entity.hpp>
#include <Penrose/Events/EventQueue.hpp>

namespace Penrose {

    /**
     * \brief Entity created event
     * \details Fired after entity creation
     */
    struct PENROSE_API EntityCreatedEvent final {

        /**
         * \brief Target entity
         */
        Entity entity;
    };

    /**
     * \brief Entity destroyed event
     * \details Fired after entity destruction
     */
    struct PENROSE_API EntityDestroyedEvent final {

        /**
         * \brief Target entity
         */
        Entity entity;
    };

    /**
     * \brief Component created event
     * \details Fired after component creation
     */
    struct PENROSE_API ComponentCreatedEvent final {

        /**
         * \brief Target entity
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

    /**
     * \brief Component destruction event
     * \details Fired after component destruction
     */
    struct PENROSE_API ComponentDestroyedEvent final {

        /**
         * \brief Target entity
         */
        Entity entity;

        /**
         * \brief Type of component
         */
        ComponentType componentType;
    };

    /**
     * \brief ECS event queue
     */
    using ECSEventQueue = EventQueue<
        EntityCreatedEvent, EntityDestroyedEvent, ComponentCreatedEvent, ComponentDestroyedEvent>;
}

#endif // PENROSE_EVENTS_ECS_EVENTS_HPP
