#include <Penrose/Input/InputHandler.hpp>

#include <Penrose/Events/InputEvent.hpp>
#include <Penrose/Resources/ResourceSet.hpp>
#include <Penrose/Utils/OptionalUtils.hpp>

namespace Penrose {

    InputHandler::InputHandler(ResourceSet *resources)
            : _eventQueue(resources->getLazy<EventQueue>()),
              _inputHooks(resources->getAllLazy<InputHook>()) {
        //
    }

    void InputHandler::pushKeyStateUpdate(InputKey key, InputState state) {
        for (const auto &hook: this->_inputHooks) {
            if (!hook->onKeyStateUpdate(key, state)) {
                return;
            }
        }

        this->_states.insert_or_assign(key, state);

        auto data = InputEventArgs{
                .type = InputEventType::KeyStateUpdated,
                .keyState = InputKeyState{key, state}
        };

        this->_eventQueue->pushEvent<EventType::InputEvent>(data);
    }

    void InputHandler::pushMouseMove(float x, float y) {
        auto [xOld, yOld] = this->_mousePos;

        this->_mousePos = {
                std::clamp(x, 0.0f, 1.0f),
                std::clamp(y, 0.0f, 1.0f)
        };

        auto data = InputEventArgs{
                .type = InputEventType::MouseMoved,
                .mousePos = this->_mousePos,
                .mousePosDelta = InputMousePos{
                        x - xOld,
                        y - yOld
                }
        };

        this->_eventQueue->pushEvent<EventType::InputEvent>(data);
    }

    void InputHandler::pushScroll(float dx, float dy) {
        auto data = InputEventArgs{
                .type = InputEventType::Scroll,
                .scroll = InputScroll{dx, dy}
        };

        this->_eventQueue->pushEvent<EventType::InputEvent>(data);
    }

    InputState InputHandler::getCurrentStateOf(InputKey key) const {
        return tryGet(this->_states, key).value_or(InputState::Released);
    }
}
