#include <Penrose/Events/Event.hpp>

namespace Penrose {

    Event makeEvent(const EventType &type) {
        return {
                .type = type,
                .value = std::nullopt
        };
    }

    Event makeEvent(const EventType &type, const EventValue &value) {
        return {
                .type = type,
                .value = value
        };
    }
}
