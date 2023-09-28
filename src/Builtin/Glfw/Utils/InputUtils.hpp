#ifndef PENROSE_BUILTIN_GLFW_UTILS_INPUT_UTILS_HPP
#define PENROSE_BUILTIN_GLFW_UTILS_INPUT_UTILS_HPP

#include <Penrose/Input/Input.hpp>

namespace Penrose {

    [[nodiscard]] InputKey fromGlfwKeyboardKey(int key);
    [[nodiscard]] InputKey fromGlfwMouseButton(int button);

    [[nodiscard]] InputState fromGlfwAction(int action);
}

#endif // PENROSE_BUILTIN_GLFW_UTILS_INPUT_UTILS_HPP
