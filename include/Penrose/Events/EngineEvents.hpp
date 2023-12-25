#ifndef PENROSE_EVENTS_ENGINE_EVENTS_HPP
#define PENROSE_EVENTS_ENGINE_EVENTS_HPP

#include <Penrose/Events/EventQueue.hpp>

namespace Penrose {

    /**
     * \brief Engine destroy request event
     * \details This event is used to stop engine. Always handled by engine.
     */
    struct PENROSE_API EngineDestroyRequestEvent {
        //
    };

    /**
     * \brief Engine event queue
     */
    using EngineEventQueue = EventQueue<EngineDestroyRequestEvent>;
}

#endif // PENROSE_EVENTS_ENGINE_EVENTS_HPP
