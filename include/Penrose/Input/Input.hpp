#ifndef PENROSE_INPUT_INPUT_HPP
#define PENROSE_INPUT_INPUT_HPP

#include <tuple>

namespace Penrose {

    /**
     * \brief Relative mouse position
     * \details Both axis are limited within [-1.0, 1.0]. Position is calculated from top left corner of surface.
     */
    using InputMousePosition = std::tuple<float, float>;

    /**
     * \brief Mouse movement: difference from previous and current position
     */
    using InputMouseMovement = std::tuple<float, float>;

    /**
     * \brief Mouse wheel scroll
     */
    using InputMouseScroll = std::tuple<float, float>;

    /**
     * \brief Input key state
     */
    enum class InputState {
        Pressed,
        Released
    };

    /**
     * \brief Input key (keyboard, mouse, gamepad, etc)
     */
    enum class InputKey {
        Unknown,

        // clang-format off
        // numbers
        _0, _1, _2, _3, _4,
        _5, _6, _7, _8, _9,

        // letters
        A, B, C, D, E, F,
        G, H, I, J, K, L,
        M, N, O, P, Q, R,
        S, T, U, V, W, X,
        Y, Z,

        // printable symbols
        Space,
        Apostrophe,     /* ' */
        Comma,          /* , */
        Minus,          /* - */
        Period,         /* . */
        Slash,          /* / */
        Semicolon,      /* ; */
        Equal,          /* = */
        LeftBracket,    /* [ */
        RightBracket,   /* ] */
        Backslash,      /* \ */
        GraveAccent,    /* ` */

        // functional keys
        Escape, Enter, Tab, Backspace,
        Left, Right, Up, Down,
        Insert, Delete, Home, End, PageUp, PageDown,
        PrintScreen, Pause, Menu,
        NumLock, CapsLock, ScrollLock,

        F1, F2, F3, F4, F5, F6,
        F7, F8, F9, F10, F11, F12,
        F13, F14, F15, F16, F17, F18,
        F19, F20, F21, F22, F23, F24,

        LeftControl, RightControl,
        LeftShift, RightShift,
        LeftAlt, RightAlt,
        LeftSuper, RightSuper,

        // numpad
        NP0, NP1, NP2, NP3, NP4,
        NP5, NP6, NP7, NP8, NP9,
        NPDecimal, NPEnter, NPEqual,
        NPAdd, NPSubtract, NPMultiply, NPDivide,

        // mouse
        MB0, MB1, MB2,
        MB3, MB4, MB5, MB6, MB7
        // clang-format on
    };

    /**
     * \brief Checks key is keyboard key
     * \param key Key
     * \return True if key is keyboard key
     */
    [[nodiscard]] bool isKeyboardKey(InputKey key);

    /**
     * \brief Checks key is mouse key
     * \param key Key
     * \return True if key is mouse key
     */
    [[nodiscard]] bool isMouseKey(InputKey key);
}

#endif // PENROSE_INPUT_INPUT_HPP
