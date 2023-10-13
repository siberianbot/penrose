#ifndef PENROSE_EVENTS_EVENT_QUEUE_HPP
#define PENROSE_EVENTS_EVENT_QUEUE_HPP

#include <array>
#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <vector>

#include <Penrose/Api.hpp>
#include <Penrose/Events/Event.hpp>
#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/Resource.hpp>
#include <Penrose/Resources/Updatable.hpp>

namespace Penrose {

    class PENROSE_API EventQueue : public Resource, public Initializable, public Updatable {
    public:
        using HandlerSignature = void(const EventBase *);
        using Handler = std::function<HandlerSignature>;
        using HandlerIdx = std::size_t;

        EventQueue() = default;
        EventQueue(const EventQueue &) = delete;
        EventQueue(EventQueue &&) = delete;
        EventQueue &operator=(const EventQueue &) = delete;
        EventQueue &operator=(EventQueue &&) = delete;

        ~EventQueue() override = default;

        void init() override { /* nothing to do */ }

        void destroy() override;

        void update(float) override;

        [[nodiscard]] HandlerIdx addHandler(EventTypeMask mask, Handler &&handler);
        void removeHandler(HandlerIdx idx);

        template<EventType Type, typename Data>
        [[nodiscard]] HandlerIdx addHandler(std::function<void(const Event<Type, Data> *)> &&handler);

        template<EventType Type, typename Data>
        void pushEvent(Data data);

    private:
        constexpr static const std::size_t EVENT_QUEUE_SIZE = 2;

        struct HandlerEntry {
            EventTypeMask mask;
            Handler handler;
        };

        std::array<std::vector<std::unique_ptr<EventBase>>, EVENT_QUEUE_SIZE> _eventQueues;
        std::size_t _currentEventQueueIdx;

        HandlerIdx _lastHandlerIdx = 0;
        std::map<HandlerIdx, HandlerEntry> _handlers;

        void pushEvent(std::unique_ptr<EventBase> &&event);
    };
}

#include "EventQueue.inl"

#endif // PENROSE_EVENTS_EVENT_QUEUE_HPP
