#ifndef PENROSE_INPUT_INPUT_HOOK_HPP
#define PENROSE_INPUT_INPUT_HOOK_HPP

#include <Penrose/Input/Input.hpp>

namespace Penrose {

    class InputHook {
    public:
        virtual ~InputHook() = default;

        [[nodiscard]] virtual bool onKeyStateUpdate(InputKey key, InputState state) = 0;
    };
}

#endif // PENROSE_INPUT_INPUT_HOOK_HPP
