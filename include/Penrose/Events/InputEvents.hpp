#ifndef PENROSE_EVENTS_INPUT_EVENTS_HPP
#define PENROSE_EVENTS_INPUT_EVENTS_HPP

#include <Penrose/Events/EventQueue.hpp>
#include <Penrose/Input/Input.hpp>

namespace Penrose {

    /**
     * \brief Key state updated event
     * \details Fired on any key state update.
     */
    struct PENROSE_API KeyStateUpdatedEvent {

        /**
         * \brief Input key
         */
        InputKey key;

        /**
         * \brief New input state
         */
        InputState state;
    };

    /**
     * \brief Mouse movement event
     * \details Fired on any mouse movement.
     */
    struct PENROSE_API MouseMovementEvent {

        /**
         * \brief Mouse position
         */
        InputMousePosition position;

        /**
         * \brief Mouse movement
         */
        InputMouseMovement movement;
    };

    /**
     * \brief Mouse wheel scroll event
     * \details Fired on any mouse wheel scroll
     */
    struct PENROSE_API MouseScrollEvent {

        /**
         * \brief Mouse wheel scroll
         */
        InputMouseScroll scroll;
    };

    /**
     * \brief Input event queue
     */
    using InputEventQueue = EventQueue<KeyStateUpdatedEvent, MouseMovementEvent, MouseScrollEvent>;
}

#endif // PENROSE_EVENTS_INPUT_EVENTS_HPP
