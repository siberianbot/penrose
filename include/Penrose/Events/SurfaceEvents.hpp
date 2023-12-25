#ifndef PENROSE_EVENTS_SURFACE_EVENTS_HPP
#define PENROSE_EVENTS_SURFACE_EVENTS_HPP

#include <memory>

#include <Penrose/Events/EventQueue.hpp>
#include <Penrose/Rendering/Surface.hpp>
#include <Penrose/Types/Size.hpp>

namespace Penrose {

    /**
     * \brief Surface closed event
     * \details Fired when surface is closed
     */
    struct PENROSE_API SurfaceClosedEvent {

        /**
         * \brief Target surface
         */
        std::shared_ptr<Surface> surface;
    };

    /**
     * \brief Surface resized event
     * \details Fired when surface is resized
     */
    struct PENROSE_API SurfaceResizedEvent {

        /**
         * \brief Target surface
         */
        std::shared_ptr<Surface> surface;

        /**
         * \brief New size of surface
         */
        Size size;
    };

    /**
     * \brief Surface event queue
     */
    using SurfaceEventQueue = EventQueue<SurfaceClosedEvent, SurfaceResizedEvent>;
}

#endif // PENROSE_EVENTS_SURFACE_EVENTS_HPP
