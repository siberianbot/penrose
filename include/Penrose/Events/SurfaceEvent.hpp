#ifndef PENROSE_EVENTS_SURFACE_EVENT_HPP
#define PENROSE_EVENTS_SURFACE_EVENT_HPP

#include <memory>

#include <Penrose/Events/Event.hpp>
#include <Penrose/Rendering/Surface.hpp>

namespace Penrose {

    enum class SurfaceEventType {
        CloseRequested
    };

    struct SurfaceEventArgs {
        SurfaceEventType type;
        std::shared_ptr<Surface> surface;
    };

    using SurfaceEvent = Event<EventType::SurfaceEvent, SurfaceEventArgs>;
}

#endif // PENROSE_EVENTS_SURFACE_EVENT_HPP
