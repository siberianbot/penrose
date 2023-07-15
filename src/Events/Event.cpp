#include <Penrose/Events/Event.hpp>

#include <algorithm>
#include <array>

namespace Penrose {

    std::optional<const ComponentEventValue *> Event::tryGetComponentEvent() const {
        static constexpr const std::array<EventType, 2> REQUIRED_TYPES = {
                EventType::ComponentCreated,
                EventType::ComponentDestroyed
        };

        if (std::find(REQUIRED_TYPES.begin(), REQUIRED_TYPES.end(), this->type) == REQUIRED_TYPES.end()) {
            return std::nullopt;
        }

        if (auto componentEvent = std::get_if<ComponentEventValue>(&*this->value)) {
            return componentEvent;
        }

        return std::nullopt;
    }

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
