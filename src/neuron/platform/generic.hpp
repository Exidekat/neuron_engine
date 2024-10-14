#pragma once

#include "neuron/pre.hpp"
#include "neuron/utils.hpp"

#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <variant>

#include <glm/glm.hpp>


#define CALLBACKF(N, ...)                                                                                                                                                          \
  public:                                                                                                                                                                          \
    using F_##N = void(__VA_ARGS__);                                                                                                                                               \
    inline void set_on_##N(const std::function<void(__VA_ARGS__)> &f) {                                                                                                            \
        m_on_##N = f;                                                                                                                                                              \
    };                                                                                                                                                                             \
    inline void unset_on_##N() {                                                                                                                                                   \
        m_on_##N = std::nullopt;                                                                                                                                                   \
    };                                                                                                                                                                             \
                                                                                                                                                                                   \
  protected:                                                                                                                                                                       \
    template <typename... Args>                                                                                                                                                    \
    inline void call_##N##_callback(Args... a) {                                                                                                                                   \
        if (m_on_##N.has_value())                                                                                                                                                  \
            (*m_on_##N)(a...);                                                                                                                                                     \
    };                                                                                                                                                                             \
                                                                                                                                                                                   \
  private:                                                                                                                                                                         \
    std::optional<std::function<F_##N>> m_on_##N = std::nullopt;                                                                                                                   \
                                                                                                                                                                                   \
  public:

namespace neuron {
    class Window;

    // most significant byte is used to diff between keys with pure ascii representation and those without (i.e. control keys like LeftControl, Home) as well as between numbers and
    // numpad.
    enum class KeyCode : uint16_t {
        // Letters
        A = 0x41,
        B = 0x42,
        C = 0x43,
        D = 0x44,
        E = 0x45,
        F = 0x46,
        G = 0x47,
        H = 0x48,
        I = 0x49,
        J = 0x4A,
        K = 0x4B,
        L = 0x4C,
        M = 0x4D,
        N = 0x4E,
        O = 0x4F,
        P = 0x50,
        Q = 0x51,
        R = 0x52,
        S = 0x53,
        T = 0x54,
        U = 0x55,
        V = 0x56,
        W = 0x57,
        X = 0x58,
        Y = 0x59,
        Z = 0x5A,

        // Numbers
        Num0 = 0x30,
        Num1 = 0x31,
        Num2 = 0x32,
        Num3 = 0x33,
        Num4 = 0x34,
        Num5 = 0x35,
        Num6 = 0x36,
        Num7 = 0x37,
        Num8 = 0x38,
        Num9 = 0x39,

        // Numpad Numbers
        Numpad0 = 0x0130,
        Numpad1 = 0x0131,
        Numpad2 = 0x0132,
        Numpad3 = 0x0133,
        Numpad4 = 0x0134,
        Numpad5 = 0x0135,
        Numpad6 = 0x0136,
        Numpad7 = 0x0137,
        Numpad8 = 0x0138,
        Numpad9 = 0x0139,

        // Special Characters
        Space          = 0x20,
        Hyphen         = 0x2D,
        Equals         = 0x3D,
        Grave          = 0x60,
        Tab            = 0x9,
        Comma          = 0x2C,
        Period         = 0x2E,
        Slash          = 0x2F,
        Backslash      = 0x5C,
        Semicolon      = 0x3B,
        Apostrophe     = 0x27,
        LeftBracket    = 0x5B,
        RightBracket   = 0x5C,
        NumpadDivide   = 0x012F,
        NumpadMultiply = 0x012A,
        NumpadSubtract = 0x012D,
        NumpadAdd      = 0x012B,
        NumpadEquals   = 0x013D,
        NumpadPeriod   = 0x012E,

        // Control Keys
        CapsLock     = 0x0200,
        LeftShift    = 0x0280,
        LeftControl  = 0x0281,
        LeftSuper    = 0x0282,
        LeftAlt      = 0x0283,
        RightShift   = 0x0240,
        RightControl = 0x0241,
        RightSuper   = 0x0242,
        RightAlt     = 0x0243,
        Backspace    = 0x0201,
        PrintScreen  = 0x0202,
        ScrollLock   = 0x0203,
        Pause        = 0x0204,
        Insert       = 0x0205,
        Delete       = 0x0206,
        Home         = 0x0207,
        End          = 0x0208,
        PageUp       = 0x0209,
        PageDown     = 0x020A,
        Enter        = 0x020B,
        NumpadEnter  = 0x030B,
        Left         = 0x020D,
        Right        = 0x020E,
        Up           = 0x020F,
        Down         = 0x0210,
        World1       = 0x211,
        World2       = 0x212,
        Escape       = 0x213,
        Menu         = 0x214,
        NumLock      = 0x215,

        // Function Keys
        F1  = 0x0400,
        F2  = 0x0401,
        F3  = 0x0402,
        F4  = 0x0403,
        F5  = 0x0404,
        F6  = 0x0405,
        F7  = 0x0406,
        F8  = 0x0407,
        F9  = 0x0408,
        F10 = 0x0409,
        F11 = 0x040C,
        F12 = 0x040D,
        F13 = 0x040E,
        F14 = 0x040F,
        F15 = 0x0410,
        F16 = 0x0411,
        F17 = 0x0412,
        F18 = 0x0413,
        F19 = 0x0414,
        F20 = 0x0415,
        F21 = 0x0416,
        F22 = 0x0417,
        F23 = 0x0418,
        F24 = 0x0419,
        F25 = 0x041A,

        // Generic Keys
        Shift   = 0x02C0,
        Control = 0x02C1,
        Super   = 0x02C2,
        Alt     = 0x02C3,

        Invalid = 0xFFFF,
    };

    struct KeyMods {
        bool shift     : 1;
        bool control   : 1;
        bool alt       : 1;
        bool super     : 1;
        bool caps_lock : 1;
        bool num_lock  : 1;
    };

    enum class GenericSpecialKey : uint8_t {};

    struct WindowFixedPosition {
        glm::ivec2 position;
    };

    struct WindowCenterMonitor {
        size_t monitor_index;
    };

    struct WindowCenterHoveredMonitor {};

    using WindowPlacement = std::variant<std::monostate, WindowFixedPosition, WindowCenterMonitor, WindowCenterHoveredMonitor>;

    constexpr auto WINDOW_PLACEMENT_USE_DEFAULT            = std::monostate{};
    constexpr auto WINDOW_PLACEMENT_CENTER_HOVERED_MONITOR = WindowCenterHoveredMonitor{};

    enum class WindowPlacementMethod : size_t {
        USE_DEFAULT            = variant_index<WindowPlacement, std::monostate>,
        FIXED_POSITION         = variant_index<WindowPlacement, WindowFixedPosition>,
        CENTER_MONITOR         = variant_index<WindowPlacement, WindowCenterMonitor>,
        CENTER_HOVERED_MONITOR = variant_index<WindowPlacement, WindowCenterHoveredMonitor>,
    };

    struct WindowDescription {
        std::string     title;
        glm::uvec2      size;
        WindowPlacement placement;

        bool resizable;
    };

    class NEURON_API Platform {
      public:
        virtual ~Platform() = default;

        virtual std::weak_ptr<Window> create_window(const WindowDescription &description) = 0;
        [[nodiscard]] virtual size_t  get_window_count() const                            = 0;

        virtual void run_event_loop()  = 0;
        virtual void step_event_loop() = 0;

        static const std::unique_ptr<Platform> &get();

        static void init();
        static void cleanup();
    };

    class NEURON_API Window : public std::enable_shared_from_this<Window> {
      protected:
        Window(/* TODO: does anything need to be here */);

      public:
        virtual ~Window();

        /**
         * It is not suggested to store the window as a shared pointer. It is allowed, and internally everything will work perfectly fine.
         * However,
         *
         * @param description The information used to create a window (title, size, position, etc...)
         * @return A std::weak_ptr to the Window object.
         */
        static std::weak_ptr<Window> create(const WindowDescription &description);

        virtual glm::uvec2 get_inner_size() const = 0;

        virtual void trigger_close() = 0;

        CALLBACKF(resize, const glm::uvec2 &new_size);
        CALLBACKF(close_request, bool *close);
        CALLBACKF(close);
        CALLBACKF(key_pressed, KeyCode keycode, const KeyMods &mods, unsigned int scancode, bool is_repeat);
        CALLBACKF(key_released, KeyCode keycode, const KeyMods &mods, unsigned int scancode);
        CALLBACKF(button_pressed, unsigned int button, const glm::ivec2& pos, const KeyMods& mods);
        CALLBACKF(button_released, unsigned int button, const glm::ivec2& pos, const KeyMods& mods);
        CALLBACKF(mouse_moved, const glm::ivec2& old_position, const glm::ivec2& new_position, const glm::ivec2& delta);
        CALLBACKF(character_typed, unsigned int unicode_codepoint); // needs to be long because keyboards and computers can generate characters which can't be represented by UTF-8 in a single byte.
        CALLBACKF(mouse_enter);
        CALLBACKF(mouse_leave);
        CALLBACKF(focus);
        CALLBACKF(unfocus);
        CALLBACKF(scrolled, const glm::ivec2 &pos, const glm::ivec2 &scroll_delta);
    };

    inline constexpr bool is_key_ascii_compatible(const KeyCode &kc) {
        return (static_cast<uint16_t>(kc) & 0x700) == 0x00;
    };

    inline constexpr bool is_key_numpad(const KeyCode &kc) {
        return (static_cast<uint16_t>(kc) & 0x100) == 0x0100;
    };

    inline constexpr bool is_key_special(const KeyCode &kc) {
        return (static_cast<uint16_t>(kc) & 0x200) == 0x0200;
    };

    inline constexpr bool is_key_function(const KeyCode &kc) {
        return (static_cast<uint16_t>(kc) & 0x400) == 0x0400;
    };

    inline constexpr KeyCode key_sided_to_generic(const KeyCode &kc) {
        uint16_t i = static_cast<uint16_t>(kc);
        if ((i & 0x0240) == 0x0240) {
            return static_cast<KeyCode>(i | 0x80);
        }

        if ((i & 0x0280) == 0x0280) {
            return static_cast<KeyCode>(i | 0x40);
        }

        return kc;
    };

    inline constexpr char key_code_to_char(const KeyCode &kc) { // specifically have to check for numpad enter because it is a non-character numpad key.
        if (kc != KeyCode::NumpadEnter && (is_key_ascii_compatible(kc) || is_key_numpad(kc))) {
            return static_cast<char>(static_cast<uint16_t>(kc) & 0xFF); // ascii compatible and numpad keys all store the ascii code in the lower byte.
        }

        return '\0';
    };

    inline constexpr bool is_keycode_valid(const KeyCode &kc) {
        return kc == KeyCode::Invalid;
    };
} // namespace neuron

#undef CALLBACKF
