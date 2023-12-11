#ifndef PENROSE_EVENTS_SURFACE_EVENTS_HPP
#define PENROSE_EVENTS_SURFACE_EVENTS_HPP

#include <memory>

#include <Penrose/Types/Size.hpp>
#include <Penrose/Events/EventQueue.hpp>
#include <Penrose/Rendering/Surface.hpp>

namespace Penrose {

    template<typename Self>
    class SurfaceEvent : public Event<Self> {
    public:
        ~SurfaceEvent() override = default;

        [[nodiscard]] const std::shared_ptr<Surface> &getSurface() const { return this->_surface; }

    protected:
        explicit SurfaceEvent(std::shared_ptr<Surface> &&surface)
                : _surface(surface) {
            //
        }

    private:
        std::shared_ptr<Surface> _surface;
    };

    class SurfaceCloseRequestedEvent : public SurfaceEvent<SurfaceCloseRequestedEvent> {
    public:
        explicit SurfaceCloseRequestedEvent(std::shared_ptr<Surface> &&surface)
                : SurfaceEvent<SurfaceCloseRequestedEvent>(std::forward<decltype(surface)>(surface)) {
            //
        }
    };

    class SurfaceResizedEvent : public SurfaceEvent<SurfaceResizedEvent> {
    public:
        explicit SurfaceResizedEvent(std::shared_ptr<Surface> &&surface,
                                     Size &&size)
                : SurfaceEvent<SurfaceResizedEvent>(std::forward<decltype(surface)>(surface)),
                  _size(size) {
            //
        }

        [[nodiscard]] const Size &getSize() const { return this->_size; }

    private:
        Size _size;
    };

    using SurfaceEventQueue = EventQueue<
            SurfaceCloseRequestedEvent,
            SurfaceResizedEvent>;
}

#endif // PENROSE_EVENTS_SURFACE_EVENTS_HPP
