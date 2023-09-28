#ifndef PENROSE_INPUT_INPUT_HANDLER_HPP
#define PENROSE_INPUT_INPUT_HANDLER_HPP

#include <cstdint>
#include <map>

#include <Penrose/Events/EventQueue.hpp>
#include <Penrose/Input/Input.hpp>
#include <Penrose/Resources/Lazy.hpp>
#include <Penrose/Resources/Resource.hpp>

namespace Penrose {

    class ResourceSet;

    class InputHandler : public Resource {
    public:
        explicit InputHandler(ResourceSet *resources);
        ~InputHandler() override = default;

        void pushKeyStateUpdate(InputKey key, InputState state);
        void pushMouseMove(float xpos, float ypos);
        void pushScroll(float dx, float dy);

        [[nodiscard]] InputState getCurrentStateOf(InputKey key) const;

    private:
        Lazy<EventQueue> _eventQueue;

        std::map<InputKey, InputState> _states;
    };
}

#endif // PENROSE_INPUT_INPUT_HANDLER_HPP
