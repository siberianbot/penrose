#ifndef PENROSE_EVENTS_INPUT_EVENTS_HPP
#define PENROSE_EVENTS_INPUT_EVENTS_HPP

#include <tuple>

#include <Penrose/Events/EventQueue.hpp>
#include <Penrose/Input/Input.hpp>

namespace Penrose {

    template<typename Self>
    class InputEvent : public Event<Self> {
    public:
        ~InputEvent() override = default;
    };

    class KeyStateUpdatedEvent : public InputEvent<KeyStateUpdatedEvent> {
    public:
        KeyStateUpdatedEvent(InputKey key, InputState state)
                : _key(key),
                  _state(state) {
            //
        }

        ~KeyStateUpdatedEvent() override = default;

        [[nodiscard]] const InputKey &getKey() const { return this->_key; }

        [[nodiscard]] const InputState &getState() const { return this->_state; }

    private:
        InputKey _key;
        InputState _state;
    };

    class MouseMovementEvent : public InputEvent<MouseMovementEvent> {
    public:
        MouseMovementEvent(std::tuple<float, float> &&position,
                           std::tuple<float, float> &&delta)
                : _position(position),
                  _delta(delta) {
            //
        }

        ~MouseMovementEvent() override = default;

        [[nodiscard]] const std::tuple<float, float> &getPosition() const { return this->_position; }

        [[nodiscard]] const std::tuple<float, float> &getDelta() const { return this->_delta; }

    private:
        std::tuple<float, float> _position;
        std::tuple<float, float> _delta;
    };

    class MouseScrollEvent : public InputEvent<MouseScrollEvent> {
    public:
        MouseScrollEvent(float dx, float dy)
                : _offset(dx, dy) {
            //
        }

        ~MouseScrollEvent() override = default;

        [[nodiscard]] const std::tuple<float, float> &getOffset() const { return this->_offset; }

    private:
        std::tuple<float, float> _offset;
    };

    using InputEventQueue = EventQueue<
            KeyStateUpdatedEvent,
            MouseMovementEvent,
            MouseScrollEvent>;
}

#endif // PENROSE_EVENTS_INPUT_EVENTS_HPP
