#ifndef PENROSE_EVENTS_CUSTOM_EVENT_HPP
#define PENROSE_EVENTS_CUSTOM_EVENT_HPP

#include <memory>
#include <string>
#include <type_traits>

#include <Penrose/Events/Event.hpp>

namespace Penrose {

    class CustomEventData {
    public:
        virtual ~CustomEventData() = default;
    };

    struct CustomEventArgs {
        std::string type;
        std::shared_ptr<CustomEventData> data;
    };

    using CustomEvent = Event<EventType::CustomEvent, CustomEventArgs>;

    template<typename T>
    concept IsCustomEvent = std::is_base_of<CustomEventData, T>::value &&
                            requires(T) {{ T::name() } -> std::same_as<std::string>; };

    template<IsCustomEvent T>
    [[nodiscard]] CustomEventArgs makeCustomEventArgs(T * data) {
        return {
                .type = T::name(),
                .data = std::shared_ptr<CustomEventData>(data)
        };
    }
}

#endif // PENROSE_EVENTS_CUSTOM_EVENT_HPP
