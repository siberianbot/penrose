#ifndef PENROSE_INPUT_INPUT_HANDLER_HPP
#define PENROSE_INPUT_INPUT_HANDLER_HPP

#include <cstdint>
#include <map>

#include <Penrose/Api.hpp>
#include <Penrose/Events/InputEvents.hpp>
#include <Penrose/Input/Input.hpp>
#include <Penrose/Input/InputHook.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

namespace Penrose {

    class PENROSE_API InputHandler: public Resource<InputHandler> {
    public:
        explicit InputHandler(const ResourceSet *resources);
        ~InputHandler() override = default;

        void pushKeyStateUpdate(InputKey key, InputState state);
        void pushMouseMove(float x, float y);
        void pushScroll(float dx, float dy);

        [[nodiscard]] InputState getCurrentStateOf(InputKey key) const;

        [[nodiscard]] std::tuple<float, float> getCurrentMousePosition() const { return this->_mousePos; }

    private:
        ResourceProxy<InputEventQueue> _eventQueue;
        ResourceProxy<InputHook> _inputHooks;

        std::map<InputKey, InputState> _states;
        InputMousePosition _mousePos;
    };
}

#endif // PENROSE_INPUT_INPUT_HANDLER_HPP
