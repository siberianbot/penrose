#ifndef PENROSE_EVENTS_EVENT_HPP
#define PENROSE_EVENTS_EVENT_HPP

#include <type_traits>

#include <Penrose/Events/EventType.hpp>

namespace Penrose {

    class EventBase {
    public:
        virtual ~EventBase() = default;

        [[nodiscard]] virtual EventType getType() const = 0;
    };

    template<EventType Type, typename Data>
    class Event : public EventBase {
    public:
        explicit Event(Data &&data) : _data(data) {
            //
        }

        [[nodiscard]] EventType getType() const override { return Type; }

        [[nodiscard]] const Data &getArgs() const { return this->_data; }

        [[nodiscard]] static Event<Type, Data> construct(Data &&data) {
            return Event<Type, Data>(data);
        }

    private:
        Data _data;
    };
}

#endif // PENROSE_EVENTS_EVENT_HPP
