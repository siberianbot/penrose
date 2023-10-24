#include <Penrose/Input/InputHandler.hpp>

#include <Penrose/Utils/OptionalUtils.hpp>

namespace Penrose {

    InputHandler::InputHandler(ResourceSet *resources)
            : _eventQueue(resources->get<InputEventQueue>()),
              _inputHooks(resources->get<InputHook>()) {
        //
    }

    void InputHandler::pushKeyStateUpdate(InputKey key, InputState state) {
        for (const auto &hook: this->_inputHooks) {
            if (!hook->onKeyStateUpdate(key, state)) {
                return;
            }
        }

        this->_states.insert_or_assign(key, state);

        this->_eventQueue->push<KeyStateUpdatedEvent>(key, state);
    }

    void InputHandler::pushMouseMove(float x, float y) {
        auto [xOld, yOld] = this->_mousePos;

        this->_mousePos = {
                std::clamp(x, -1.0f, 1.0f),
                std::clamp(y, -1.0f, 1.0f)
        };

        auto pos = this->_mousePos;
        auto delta = std::make_tuple(x - xOld, y - yOld);

        this->_eventQueue->push<MouseMovementEvent>(
                std::move(pos),
                std::move(delta));
    }

    void InputHandler::pushScroll(float dx, float dy) {
        this->_eventQueue->push<MouseScrollEvent>(dx, dy);
    }

    InputState InputHandler::getCurrentStateOf(InputKey key) const {
        return tryGet(this->_states, key).value_or(InputState::Released);
    }
}
