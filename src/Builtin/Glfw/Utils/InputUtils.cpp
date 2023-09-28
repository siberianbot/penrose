#include "src/Builtin/Glfw/Utils/InputUtils.hpp"

#include <GLFW/glfw3.h>

#include <Penrose/Common/EngineError.hpp>

namespace Penrose {

    InputKey fromGlfwKeyboardKey(int key) {
        switch (key) {
            case GLFW_KEY_0:
                return InputKey::_0;
            case GLFW_KEY_1:
                return InputKey::_1;
            case GLFW_KEY_2:
                return InputKey::_2;
            case GLFW_KEY_3:
                return InputKey::_3;
            case GLFW_KEY_4:
                return InputKey::_4;
            case GLFW_KEY_5:
                return InputKey::_5;
            case GLFW_KEY_6:
                return InputKey::_6;
            case GLFW_KEY_7:
                return InputKey::_7;
            case GLFW_KEY_8:
                return InputKey::_8;
            case GLFW_KEY_9:
                return InputKey::_9;

            case GLFW_KEY_A:
                return InputKey::A;
            case GLFW_KEY_B:
                return InputKey::B;
            case GLFW_KEY_C:
                return InputKey::C;
            case GLFW_KEY_D:
                return InputKey::D;
            case GLFW_KEY_E:
                return InputKey::E;
            case GLFW_KEY_F:
                return InputKey::F;
            case GLFW_KEY_G:
                return InputKey::G;
            case GLFW_KEY_H:
                return InputKey::H;
            case GLFW_KEY_I:
                return InputKey::I;
            case GLFW_KEY_J:
                return InputKey::J;
            case GLFW_KEY_K:
                return InputKey::K;
            case GLFW_KEY_L:
                return InputKey::L;
            case GLFW_KEY_M:
                return InputKey::M;
            case GLFW_KEY_N:
                return InputKey::N;
            case GLFW_KEY_O:
                return InputKey::O;
            case GLFW_KEY_P:
                return InputKey::P;
            case GLFW_KEY_Q:
                return InputKey::Q;
            case GLFW_KEY_R:
                return InputKey::R;
            case GLFW_KEY_S:
                return InputKey::S;
            case GLFW_KEY_T:
                return InputKey::T;
            case GLFW_KEY_U:
                return InputKey::U;
            case GLFW_KEY_V:
                return InputKey::V;
            case GLFW_KEY_W:
                return InputKey::W;
            case GLFW_KEY_X:
                return InputKey::X;
            case GLFW_KEY_Y:
                return InputKey::Y;
            case GLFW_KEY_Z:
                return InputKey::Z;

            case GLFW_KEY_SPACE:
                return InputKey::Space;
            case GLFW_KEY_APOSTROPHE:
                return InputKey::Apostrophe;
            case GLFW_KEY_COMMA:
                return InputKey::Comma;
            case GLFW_KEY_MINUS:
                return InputKey::Minus;
            case GLFW_KEY_PERIOD:
                return InputKey::Period;
            case GLFW_KEY_SLASH:
                return InputKey::Slash;
            case GLFW_KEY_SEMICOLON:
                return InputKey::Semicolon;
            case GLFW_KEY_EQUAL:
                return InputKey::Equal;
            case GLFW_KEY_LEFT_BRACKET:
                return InputKey::LeftBracket;
            case GLFW_KEY_RIGHT_BRACKET:
                return InputKey::RightBracket;
            case GLFW_KEY_BACKSLASH:
                return InputKey::Backslash;
            case GLFW_KEY_GRAVE_ACCENT:
                return InputKey::GraveAccent;

            case GLFW_KEY_ESCAPE:
                return InputKey::Escape;
            case GLFW_KEY_ENTER:
                return InputKey::Enter;
            case GLFW_KEY_TAB:
                return InputKey::Tab;
            case GLFW_KEY_BACKSPACE:
                return InputKey::Backspace;
            case GLFW_KEY_LEFT:
                return InputKey::Left;
            case GLFW_KEY_RIGHT:
                return InputKey::Right;
            case GLFW_KEY_UP:
                return InputKey::Up;
            case GLFW_KEY_DOWN:
                return InputKey::Down;
            case GLFW_KEY_INSERT:
                return InputKey::Insert;
            case GLFW_KEY_DELETE:
                return InputKey::Delete;
            case GLFW_KEY_HOME:
                return InputKey::Home;
            case GLFW_KEY_END:
                return InputKey::End;
            case GLFW_KEY_PAGE_UP:
                return InputKey::PageUp;
            case GLFW_KEY_PAGE_DOWN:
                return InputKey::PageDown;
            case GLFW_KEY_PRINT_SCREEN:
                return InputKey::PrintScreen;
            case GLFW_KEY_PAUSE:
                return InputKey::Pause;
            case GLFW_KEY_MENU:
                return InputKey::Menu;
            case GLFW_KEY_NUM_LOCK:
                return InputKey::NumLock;
            case GLFW_KEY_CAPS_LOCK:
                return InputKey::CapsLock;
            case GLFW_KEY_SCROLL_LOCK:
                return InputKey::ScrollLock;

            case GLFW_KEY_F1:
                return InputKey::F1;
            case GLFW_KEY_F2:
                return InputKey::F2;
            case GLFW_KEY_F3:
                return InputKey::F3;
            case GLFW_KEY_F4:
                return InputKey::F4;
            case GLFW_KEY_F5:
                return InputKey::F5;
            case GLFW_KEY_F6:
                return InputKey::F6;
            case GLFW_KEY_F7:
                return InputKey::F7;
            case GLFW_KEY_F8:
                return InputKey::F8;
            case GLFW_KEY_F9:
                return InputKey::F9;
            case GLFW_KEY_F10:
                return InputKey::F10;
            case GLFW_KEY_F11:
                return InputKey::F11;
            case GLFW_KEY_F12:
                return InputKey::F12;
            case GLFW_KEY_F13:
                return InputKey::F13;
            case GLFW_KEY_F14:
                return InputKey::F14;
            case GLFW_KEY_F15:
                return InputKey::F15;
            case GLFW_KEY_F16:
                return InputKey::F16;
            case GLFW_KEY_F17:
                return InputKey::F17;
            case GLFW_KEY_F18:
                return InputKey::F18;
            case GLFW_KEY_F19:
                return InputKey::F19;
            case GLFW_KEY_F20:
                return InputKey::F20;
            case GLFW_KEY_F21:
                return InputKey::F21;
            case GLFW_KEY_F22:
                return InputKey::F22;
            case GLFW_KEY_F23:
                return InputKey::F23;
            case GLFW_KEY_F24:
                return InputKey::F24;

            case GLFW_KEY_LEFT_CONTROL:
                return InputKey::LeftControl;
            case GLFW_KEY_RIGHT_CONTROL:
                return InputKey::RightControl;
            case GLFW_KEY_LEFT_SHIFT:
                return InputKey::LeftShift;
            case GLFW_KEY_RIGHT_SHIFT:
                return InputKey::RightShift;
            case GLFW_KEY_LEFT_ALT:
                return InputKey::LeftAlt;
            case GLFW_KEY_RIGHT_ALT:
                return InputKey::RightAlt;
            case GLFW_KEY_LEFT_SUPER:
                return InputKey::LeftSuper;
            case GLFW_KEY_RIGHT_SUPER:
                return InputKey::RightSuper;

            case GLFW_KEY_KP_0:
                return InputKey::NP0;
            case GLFW_KEY_KP_1:
                return InputKey::NP1;
            case GLFW_KEY_KP_2:
                return InputKey::NP2;
            case GLFW_KEY_KP_3:
                return InputKey::NP3;
            case GLFW_KEY_KP_4:
                return InputKey::NP4;
            case GLFW_KEY_KP_5:
                return InputKey::NP5;
            case GLFW_KEY_KP_6:
                return InputKey::NP6;
            case GLFW_KEY_KP_7:
                return InputKey::NP7;
            case GLFW_KEY_KP_8:
                return InputKey::NP8;
            case GLFW_KEY_KP_9:
                return InputKey::NP9;
            case GLFW_KEY_KP_DECIMAL:
                return InputKey::NPDecimal;
            case GLFW_KEY_KP_ENTER:
                return InputKey::NPEnter;
            case GLFW_KEY_KP_EQUAL:
                return InputKey::NPEqual;
            case GLFW_KEY_KP_ADD:
                return InputKey::NPAdd;
            case GLFW_KEY_KP_SUBTRACT:
                return InputKey::NPSubtract;
            case GLFW_KEY_KP_MULTIPLY:
                return InputKey::NPMultiply;
            case GLFW_KEY_KP_DIVIDE:
                return InputKey::NPDivide;

            default:
                return InputKey::Unknown;
        }
    }

    InputKey fromGlfwMouseButton(int button) {
        switch (button) {
            case GLFW_MOUSE_BUTTON_1:
                return InputKey::MB0;
            case GLFW_MOUSE_BUTTON_2:
                return InputKey::MB1;
            case GLFW_MOUSE_BUTTON_3:
                return InputKey::MB2;
            case GLFW_MOUSE_BUTTON_4:
                return InputKey::MB3;
            case GLFW_MOUSE_BUTTON_5:
                return InputKey::MB4;
            case GLFW_MOUSE_BUTTON_6:
                return InputKey::MB5;
            case GLFW_MOUSE_BUTTON_7:
                return InputKey::MB6;
            case GLFW_MOUSE_BUTTON_8:
                return InputKey::MB7;

            default:
                return InputKey::Unknown;
        }
    }

    InputState fromGlfwAction(int action) {
        switch (action) {
            case GLFW_RELEASE:
                return InputState::Released;

            case GLFW_PRESS:
            case GLFW_REPEAT:
                return InputState::Pressed;

            default:
                throw EngineError("Not supported action {:#x}", action);
        }
    }
}
