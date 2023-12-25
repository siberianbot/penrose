#include <Penrose/Input/InputHandler.hpp>

#include <algorithm>

#include <Penrose/Utils/OptionalUtils.hpp>

namespace Penrose {

    InputHandler::InputHandler(const ResourceSet *resources)
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

        this->_eventQueue->push(KeyStateUpdatedEvent {
            .key = key,
            .state = state,
        });
    }

    void InputHandler::pushMouseMove(float x, float y) {
        auto [xOld, yOld] = this->_mousePos;

        this->_mousePos = {std::clamp(x, -1.0f, 1.0f), std::clamp(y, -1.0f, 1.0f)};

        this->_eventQueue->push(MouseMovementEvent {
            .position = this->_mousePos,
            .movement = {x - xOld, y - yOld},
        });
    }

    void InputHandler::pushScroll(float dx, float dy) {
        this->_eventQueue->push(MouseScrollEvent {
            .scroll = {dx, dy},
        });
    }

    InputState InputHandler::getCurrentStateOf(InputKey key) const {
        return tryGet(this->_states, key).value_or(InputState::Released);
    }
}
