#ifndef PENROSE_EVENTS_INPUT_EVENT_HPP
#define PENROSE_EVENTS_INPUT_EVENT_HPP

#include <tuple>
#include <variant>

#include <Penrose/Events/Event.hpp>
#include <Penrose/Input/Input.hpp>

namespace Penrose {

    enum class InputEventType {
        KeyStateUpdated,
        MouseMoved,
        Scroll
    };

    using InputKeyState = std::tuple<InputKey, InputState>;
    using InputMouseMove = std::tuple<float, float>;
    using InputScroll = std::tuple<float, float>;

    struct InputEventArgs {
        InputEventType type;
        InputKeyState keyState;
        InputMouseMove mouseMove;
        InputScroll scroll;
    };

    using InputEvent = Event<EventType::InputEvent, InputEventArgs>;
}

#endif // PENROSE_EVENTS_INPUT_EVENT_HPP
