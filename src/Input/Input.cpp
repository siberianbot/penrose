#include <Penrose/Input/Input.hpp>

#include <algorithm>
#include <array>

namespace Penrose {

    constexpr static const std::array<InputKey, 117> KEYBOARD_KEYS = {
            InputKey::_0, InputKey::_1, InputKey::_2, InputKey::_3, InputKey::_4,
            InputKey::_5, InputKey::_6, InputKey::_7, InputKey::_8, InputKey::_9,

            InputKey::A, InputKey::B, InputKey::C, InputKey::D, InputKey::E, InputKey::F,
            InputKey::G, InputKey::H, InputKey::I, InputKey::J, InputKey::K, InputKey::L,
            InputKey::M, InputKey::N, InputKey::O, InputKey::P, InputKey::Q, InputKey::R,
            InputKey::S, InputKey::T, InputKey::U, InputKey::V, InputKey::W, InputKey::X,
            InputKey::Y, InputKey::Z,

            InputKey::Space, InputKey::Apostrophe, InputKey::Comma, InputKey::Minus,
            InputKey::Period, InputKey::Slash, InputKey::Semicolon, InputKey::Equal,
            InputKey::LeftBracket, InputKey::RightBracket, InputKey::Backslash, InputKey::GraveAccent,

            InputKey::Escape, InputKey::Enter, InputKey::Tab, InputKey::Backspace,
            InputKey::Left, InputKey::Right, InputKey::Up, InputKey::Down,
            InputKey::Insert, InputKey::Delete, InputKey::Home, InputKey::End,
            InputKey::PageUp, InputKey::PageDown, InputKey::PrintScreen, InputKey::Pause,
            InputKey::Menu, InputKey::NumLock, InputKey::CapsLock, InputKey::ScrollLock,

            InputKey::F1, InputKey::F2, InputKey::F3, InputKey::F4, InputKey::F5, InputKey::F6,
            InputKey::F7, InputKey::F8, InputKey::F9, InputKey::F10, InputKey::F11, InputKey::F12,
            InputKey::F13, InputKey::F14, InputKey::F15, InputKey::F16, InputKey::F17, InputKey::F18,
            InputKey::F19, InputKey::F20, InputKey::F21, InputKey::F22, InputKey::F23, InputKey::F24,

            InputKey::LeftControl, InputKey::RightControl, InputKey::LeftShift, InputKey::RightShift,
            InputKey::LeftAlt, InputKey::RightAlt, InputKey::LeftSuper, InputKey::RightSuper,

            InputKey::NP0, InputKey::NP1, InputKey::NP2, InputKey::NP3, InputKey::NP4,
            InputKey::NP5, InputKey::NP6, InputKey::NP7, InputKey::NP8, InputKey::NP9,
            InputKey::NPDecimal, InputKey::NPEnter, InputKey::NPEqual,
            InputKey::NPAdd, InputKey::NPSubtract, InputKey::NPMultiply, InputKey::NPDivide
    };

    constexpr static const std::array<InputKey, 8> MOUSE_KEYS = {
            InputKey::MB0, InputKey::MB1, InputKey::MB2, InputKey::MB3,
            InputKey::MB4, InputKey::MB5, InputKey::MB6, InputKey::MB7
    };

    bool isKeyboardKey(InputKey key) {
        return std::find(KEYBOARD_KEYS.begin(), KEYBOARD_KEYS.end(), key) != KEYBOARD_KEYS.end();
    }

    bool isMouseKey(InputKey key) {
        return std::find(MOUSE_KEYS.begin(), MOUSE_KEYS.end(), key) != MOUSE_KEYS.end();
    }
}
