#include <cstring>
#ifdef __unix__
#include "x11.hpp"

#include <glm/gtx/string_cast.hpp>
#include <iostream>

#include <X11/keysym.h>

#include <X11/XKBlib.h>

namespace neuron {
    inline X11Platform *_get_platform() {
        return dynamic_cast<X11Platform *>(Platform::get().get());
    }

    constexpr uint32_t _kn(const char s[4]) {
        return static_cast<uint32_t>(s[3]) << 24 | static_cast<uint32_t>(s[2]) << 16 | static_cast<uint32_t>(s[1]) << 8 | static_cast<uint32_t>(s[0]);
    }

    struct KNS {
        char s[4];

        constexpr KNS(char const (&p)[1]) {
            s[0] = p[0];
            s[1] = 0;
            s[2] = 0;
            s[3] = 0;
        }

        constexpr KNS(char const (&p)[2]) {
            s[0] = p[0];
            s[1] = p[1];
            s[2] = 0;
            s[3] = 0;
        }

        constexpr KNS(char const (&p)[3]) {
            s[0] = p[0];
            s[1] = p[1];
            s[2] = p[2];
            s[3] = 0;
        }

        constexpr KNS(char const (&p)[4]) {
            s[0] = p[0];
            s[1] = p[1];
            s[2] = p[2];
            s[3] = p[3];
        }

        constexpr KNS(char const (&p)[5]) {
            s[0] = p[0];
            s[1] = p[1];
            s[2] = p[2];
            s[3] = p[3];
        }
    };

    template <KNS S>
    constexpr uint32_t operator""_kn() {
        return _kn(S.s);
    };

    X11Platform::X11Platform() {
        m_display     = XOpenDisplay(nullptr);
        m_screen      = XDefaultScreenOfDisplay(m_display);
        m_root_window = XRootWindowOfScreen(m_screen);

        m_screen_resources = XRRGetScreenResourcesCurrent(m_display, m_root_window);

        // pretend to do some funky shit
        const auto pos = get_default_window_position({800, 600}, get_primary_monitor_index());
        std::cout << "Window @ (" << pos.x << ", " << pos.y << ") w/ size 800x600" << std::endl;

        m_atom_wm_protocols     = intern_atom("WM_PROTOCOLS");
        m_atom_wm_delete_window = intern_atom("WM_DELETE_WINDOW");

        {
            int i;
            XRRQueryExtension(m_display, &m_randr_event_base, &i);
        }

        XRRSelectInput(m_display, m_root_window, RRScreenChangeNotifyMask | RROutputPropertyNotifyMask | RROutputChangeNotifyMask | RRCrtcChangeNotifyMask);

        // Keycodes
        {
#if XkbKeyNameLength == 4
            // wild what you can do when the name length is 4 bytes
            // also for reference the names here are from
            std::unordered_map<uint32_t, KeyCode> name_mapping;
            name_mapping["TLDE"_kn] = KeyCode::Grave;
            name_mapping["AE01"_kn] = KeyCode::Num1;
            name_mapping["AE02"_kn] = KeyCode::Num2;
            name_mapping["AE03"_kn] = KeyCode::Num3;
            name_mapping["AE04"_kn] = KeyCode::Num4;
            name_mapping["AE05"_kn] = KeyCode::Num5;
            name_mapping["AE06"_kn] = KeyCode::Num6;
            name_mapping["AE07"_kn] = KeyCode::Num7;
            name_mapping["AE08"_kn] = KeyCode::Num8;
            name_mapping["AE09"_kn] = KeyCode::Num9;
            name_mapping["AE10"_kn] = KeyCode::Num0;
            name_mapping["AE11"_kn] = KeyCode::Hyphen;
            name_mapping["AE12"_kn] = KeyCode::Equals;

            name_mapping["AD01"_kn] = KeyCode::Q;
            name_mapping["AD02"_kn] = KeyCode::W;
            name_mapping["AD03"_kn] = KeyCode::E;
            name_mapping["AD04"_kn] = KeyCode::R;
            name_mapping["AD05"_kn] = KeyCode::T;
            name_mapping["AD06"_kn] = KeyCode::Y;
            name_mapping["AD07"_kn] = KeyCode::U;
            name_mapping["AD08"_kn] = KeyCode::I;
            name_mapping["AD09"_kn] = KeyCode::O;
            name_mapping["AD10"_kn] = KeyCode::P;
            name_mapping["AD11"_kn] = KeyCode::LeftBracket;
            name_mapping["AD12"_kn] = KeyCode::RightBracket;

            name_mapping["AC01"_kn] = KeyCode::A;
            name_mapping["AC02"_kn] = KeyCode::S;
            name_mapping["AC03"_kn] = KeyCode::D;
            name_mapping["AC04"_kn] = KeyCode::F;
            name_mapping["AC05"_kn] = KeyCode::G;
            name_mapping["AC06"_kn] = KeyCode::H;
            name_mapping["AC07"_kn] = KeyCode::J;
            name_mapping["AC08"_kn] = KeyCode::K;
            name_mapping["AC09"_kn] = KeyCode::L;
            name_mapping["AC10"_kn] = KeyCode::Semicolon;
            name_mapping["AC11"_kn] = KeyCode::Apostrophe;
            name_mapping["BKSL"_kn] = KeyCode::Backslash;

            name_mapping["AB01"_kn] = KeyCode::Z;
            name_mapping["AB02"_kn] = KeyCode::X;
            name_mapping["AB03"_kn] = KeyCode::C;
            name_mapping["AB04"_kn] = KeyCode::V;
            name_mapping["AB05"_kn] = KeyCode::B;
            name_mapping["AB06"_kn] = KeyCode::N;
            name_mapping["AB07"_kn] = KeyCode::M;
            name_mapping["AB08"_kn] = KeyCode::Comma;
            name_mapping["AB09"_kn] = KeyCode::Period;
            name_mapping["AB10"_kn] = KeyCode::Slash;

            name_mapping["LSGT"_kn] = KeyCode::World1;

            name_mapping["ESC"_kn]  = KeyCode::Escape;
            name_mapping["TAB"_kn]  = KeyCode::Tab;
            name_mapping["CAPS"_kn] = KeyCode::CapsLock;
            name_mapping["BKSP"_kn] = KeyCode::Backspace;
            name_mapping["RTRN"_kn] = KeyCode::Enter;
            name_mapping["SPCE"_kn] = KeyCode::Space;
            name_mapping["LFSH"_kn] = KeyCode::LeftShift;
            name_mapping["RTSH"_kn] = KeyCode::RightShift;
            name_mapping["LCTL"_kn] = KeyCode::LeftControl;
            name_mapping["RCTL"_kn] = KeyCode::RightControl;
            name_mapping["LALT"_kn] = KeyCode::LeftAlt;
            name_mapping["RALT"_kn] = KeyCode::RightAlt;
            name_mapping["LWIN"_kn] = KeyCode::LeftSuper;
            name_mapping["RWIN"_kn] = KeyCode::RightSuper;
            name_mapping["MENU"_kn] = KeyCode::Menu;
            name_mapping["PRSC"_kn] = KeyCode::PrintScreen;
            name_mapping["SCLK"_kn] = KeyCode::ScrollLock;
            name_mapping["PAUS"_kn] = KeyCode::Pause;
            name_mapping["INS"_kn]  = KeyCode::Insert;
            name_mapping["DELE"_kn] = KeyCode::Delete;
            name_mapping["HOME"_kn] = KeyCode::Home;
            name_mapping["END"_kn]  = KeyCode::End;
            name_mapping["PGUP"_kn] = KeyCode::PageUp;
            name_mapping["PGDN"_kn] = KeyCode::PageDown;
            name_mapping["UP"_kn]   = KeyCode::Up;
            name_mapping["DOWN"_kn] = KeyCode::Down;
            name_mapping["LEFT"_kn] = KeyCode::Left;
            name_mapping["RGHT"_kn] = KeyCode::Right;
            name_mapping["NMLK"_kn] = KeyCode::NumLock;

            name_mapping["FK01"_kn] = KeyCode::F1;
            name_mapping["FK02"_kn] = KeyCode::F2;
            name_mapping["FK03"_kn] = KeyCode::F3;
            name_mapping["FK04"_kn] = KeyCode::F4;
            name_mapping["FK05"_kn] = KeyCode::F5;
            name_mapping["FK06"_kn] = KeyCode::F6;
            name_mapping["FK07"_kn] = KeyCode::F7;
            name_mapping["FK08"_kn] = KeyCode::F8;
            name_mapping["FK09"_kn] = KeyCode::F9;
            name_mapping["FK10"_kn] = KeyCode::F10;
            name_mapping["FK11"_kn] = KeyCode::F11;
            name_mapping["FK12"_kn] = KeyCode::F12;

            name_mapping["KP7"_kn] = KeyCode::Numpad7;
            name_mapping["KP8"_kn] = KeyCode::Numpad8;
            name_mapping["KP9"_kn] = KeyCode::Numpad9;

            name_mapping["KP4"_kn] = KeyCode::Numpad4;
            name_mapping["KP5"_kn] = KeyCode::Numpad5;
            name_mapping["KP6"_kn] = KeyCode::Numpad6;

            name_mapping["KP1"_kn] = KeyCode::Numpad1;
            name_mapping["KP2"_kn] = KeyCode::Numpad2;
            name_mapping["KP3"_kn] = KeyCode::Numpad3;

            name_mapping["KPEN"_kn] = KeyCode::NumpadEnter;
            name_mapping["KPEQ"_kn] = KeyCode::NumpadEquals;
            name_mapping["KP0"_kn]  = KeyCode::Numpad0;
            name_mapping["KPDL"_kn] = KeyCode::NumpadPeriod;
            name_mapping["KPPT"_kn] = KeyCode::NumpadPeriod;

            name_mapping["KPDV"_kn] = KeyCode::NumpadDivide;
            name_mapping["KPMU"_kn] = KeyCode::NumpadMultiply;
            name_mapping["KPSU"_kn] = KeyCode::NumpadSubtract;
            name_mapping["KPAD"_kn] = KeyCode::NumpadAdd;

            name_mapping["LVL3"_kn] = KeyCode::RightAlt;
            name_mapping["MDSW"_kn] = KeyCode::RightAlt;

            XkbDescPtr desc = XkbGetMap(m_display, 0, XkbUseCoreKbd);
            XkbGetNames(m_display, XkbKeyNamesMask | XkbKeyAliasesMask, desc);

            const int minScancode = desc->min_key_code;
            const int maxScancode = desc->max_key_code;

            memset(m_keycodes, 0, sizeof(m_keycodes)); // m_keycodes is a sized array this should work

            for (int sc = minScancode; sc <= maxScancode; ++sc) {
                if (const auto it = name_mapping.find(_kn(desc->names->keys[sc].name)); it != name_mapping.end()) {
                    m_keycodes[sc] = it->second;
                    continue;
                }

                for (int i = 0; i < desc->names->num_key_aliases; ++i) {
                    if (_kn(desc->names->key_aliases[i].real) != _kn(desc->names->keys[sc].name))
                        continue;
                    if (const auto it = name_mapping.find(_kn(desc->names->key_aliases[i].alias)); it != name_mapping.end()) {
                        m_keycodes[sc] = it->second;
                        break;
                    }
                }
            }

            XkbFreeNames(desc, XkbKeyNamesMask, true);
            XkbFreeKeyboard(desc, 0, true);

#else
            // do it via strings
            throw std::runtime_error("Xkb Key Name Mapping Via Strings not yet implemented");
#endif
        }

        {
            Bool s;
            if (XkbSetDetectableAutoRepeat(m_display, true, &s)) if (s) m_xkb_auto_repeat_detectable = true;
        }
    }

    X11Platform::~X11Platform() {
        XRRFreeScreenResources(m_screen_resources);
        XCloseDisplay(m_display);
    }

    std::weak_ptr<Window> X11Platform::create_window(const WindowDescription &description) {
        const auto window = std::shared_ptr<X11Window>(new X11Window(description));
        register_new_window(window->x11_handle(), window);
        return window;
    }

    size_t X11Platform::get_window_count() const {
        return m_open_windows.size();
    }

    glm::ivec2 X11Platform::get_default_window_position(const glm::uvec2 &size, const uint32_t target_monitor) const {
        if (!m_screen_resources) {
            return {0, 0}; // screen resources weren't successfully queried.
        }

        if (target_monitor >= m_screen_resources->noutput) {
            if (m_screen_resources->noutput == 0) {
                // without any outputs, just return a default value (same as if screen resources weren't queried right)
                return {0, 0};
            }
            return _get_default_window_position(size, _get_primary_output());
        }

        return _get_default_window_position(size, m_screen_resources->outputs[target_monitor]);
    }

    size_t X11Platform::get_primary_monitor_index() const {
        const RROutput output = _get_primary_output();
        for (size_t i = 0; i < m_screen_resources->noutput; ++i) {
            if (m_screen_resources->outputs[i] == output) {
                return i;
            }
        }

        return SIZE_MAX;
    }

    void X11Platform::run_event_loop() {
        XEvent event;
        while (!m_open_windows.empty()) {
            XNextEvent(m_display, &event);
            _handle_event(event);
        }
    }

    void X11Platform::step_event_loop() {
        XEvent event;
        XPending(m_display);
        while (XQLength(m_display)) {
            XNextEvent(m_display, &event);
            _handle_event(event);
        }
        XFlush(m_display);
    }

    Atom X11Platform::intern_atom(const std::string &text, const bool create_not_exists) const {
        return XInternAtom(m_display, text.c_str(), create_not_exists);
    }

    size_t X11Platform::get_monitor_index_by_point(const glm::ivec2 &point) const {
        for (size_t i = 0; i < m_screen_resources->noutput; i++) {
            if (_output_contains_point(m_screen_resources->outputs[i], point)) {
                return i;
            }
        }

        // TODO: when we have logging, print out a warning here.
        return get_primary_monitor_index(); // default to primary
    }

    glm::ivec2 X11Platform::get_cursor_pos() const {
        [[maybe_unused]] ::Window     root_ret;
        [[maybe_unused]] ::Window     child_ret;
        int                           x, y;
        [[maybe_unused]] int          wx, wy;
        [[maybe_unused]] unsigned int mask_ret;
        XQueryPointer(m_display, m_root_window, &root_ret, &child_ret, &x, &y, &wx, &wy, &mask_ret);

        return {x, y};
    }

    void X11Platform::register_new_window(::Window window, const std::shared_ptr<X11Window> &p_window) {
        m_open_windows.insert(window);
        m_window_map.insert({window, p_window});
    }

    glm::ivec2 X11Platform::_get_default_window_position(const glm::uvec2 &size, const RROutput output) const {
        XRROutputInfo *output_info = XRRGetOutputInfo(m_display, m_screen_resources, output);
        XRRCrtcInfo   *crtc_info   = XRRGetCrtcInfo(m_display, m_screen_resources, output_info->crtc);

        int32_t x = crtc_info->x + static_cast<int32_t>(crtc_info->width - size.x) / 2;
        int32_t y = crtc_info->y + static_cast<int32_t>(crtc_info->height - size.y) / 2;

        XRRFreeOutputInfo(output_info);
        XRRFreeCrtcInfo(crtc_info);

        return {x, y};
    }

    RROutput X11Platform::_get_primary_output() const {
        return XRRGetOutputPrimary(m_display, m_root_window);
    }

    const XRRModeInfo &X11Platform::_get_mode_info(const RRMode mode) const {
        for (size_t i = 0; i < m_screen_resources->nmode; ++i) {
            if (m_screen_resources->modes[i].id == mode) {
                return m_screen_resources->modes[i];
            }
        }

        throw std::runtime_error("Mode not found");
    }

    bool X11Platform::_output_contains_point(const RROutput output, const glm::ivec2 &point) const {
        const XRROutputInfo *output_info = XRRGetOutputInfo(m_display, m_screen_resources, output);
        const XRRCrtcInfo   *crtc_info   = XRRGetCrtcInfo(m_display, m_screen_resources, output_info->crtc);
        return point.x >= crtc_info->x && point.y >= crtc_info->y && point.x <= crtc_info->x + crtc_info->width && point.y <= crtc_info->y + crtc_info->height;
    }

    void X11Platform::_handle_event(XEvent &event) {
        unsigned int scancode = 0xFFFFFFFF;
        if (event.type == KeyPress || event.type == KeyRelease)
            scancode = event.xkey.keycode;
        const bool filtered = XFilterEvent(&event, None);

        switch (event.type) {
        case KeyPress:
            _on_key_press(event.xkey, filtered, scancode);
            break;
        case KeyRelease:
            _on_key_release(event.xkey, filtered, scancode);
            break;
        case ButtonPress:
            _on_button_press(event.xbutton);
            break;
        case ButtonRelease:
            _on_button_release(event.xbutton);
            break;
        case MotionNotify:
            _on_motion_notify(event.xmotion);
            break;
        case EnterNotify:
            _on_enter_notify(event.xcrossing);
            break;
        case LeaveNotify:
            _on_leave_notify(event.xcrossing);
            break;
        case FocusIn:
            _on_focus_in(event.xfocus);
            break;
        case FocusOut:
            _on_focus_out(event.xfocus);
            break;
        case KeymapNotify:
            _on_keymap_notify(event.xkeymap);
            break;
        case Expose:
            _on_expose(event.xexpose);
            break;
        case GraphicsExpose:
            _on_graphics_expose(event.xgraphicsexpose);
            break;
        case NoExpose:
            _on_no_expose(event.xnoexpose);
            break;
        case VisibilityNotify:
            _on_visibility_notify(event.xvisibility);
            break;
        case CreateNotify:
            _on_create_notify(event.xcreatewindow);
            break;
        case DestroyNotify:
            _on_destroy_notify(event.xdestroywindow);
            break;
        case UnmapNotify:
            _on_unmap_notify(event.xunmap);
            break;
        case MapNotify:
            _on_map_notify(event.xmap);
            break;
        case MapRequest:
            _on_map_request(event.xmaprequest);
            break;
        case ReparentNotify:
            _on_reparent_notify(event.xreparent);
            break;
        case ConfigureNotify:
            _on_configure_notify(event.xconfigure);
            break;
        case ConfigureRequest:
            _on_configure_request(event.xconfigurerequest);
            break;
        case GravityNotify:
            _on_gravity_notify(event.xgravity);
            break;
        case CirculateNotify:
            _on_circulate_notify(event.xcirculate);
            break;
        case CirculateRequest:
            _on_circulate_request(event.xcirculaterequest);
            break;
        case PropertyNotify:
            _on_property_notify(event.xproperty);
            break;
        case SelectionClear:
            _on_selection_clear(event.xselectionclear);
            break;
        case SelectionRequest:
            _on_selection_request(event.xselectionrequest);
            break;
        case SelectionNotify:
            _on_selection_notify(event.xselection);
            break;
        case ColormapNotify:
            _on_colormap_notify(event.xcolormap);
            break;
        case ClientMessage:
            _on_client_message(event.xclient);
            break;
        case MappingNotify:
            _on_mapping_notify(event.xmapping);
            break;
        case GenericEvent:
            _on_generic_event(event.xgeneric);
            break;
        default:
            if (event.type == m_randr_event_base + RRNotify)
                _on_randr_notify(*reinterpret_cast<XRRNotifyEvent *>(&event));
            else if (event.type == m_randr_event_base + RRScreenChangeNotify)
                _on_randr_screen_change_notify(*reinterpret_cast<XRRScreenChangeNotifyEvent *>(&event));
            break;
        }
    }

    inline KeyCode _trsc(const unsigned int &sc, KeyCode const (&keycodes)[256]) {
        if (sc > 255)
            return KeyCode::Invalid;
        return keycodes[sc];
    }

    inline KeyMods _trmods(const unsigned int state) {
        return {.shift     = (state & ShiftMask) != 0,
                .control   = (state & ControlMask) != 0,
                .alt       = (state & Mod1Mask) != 0,
                .super     = (state & Mod4Mask) != 0,
                .caps_lock = (state & LockMask) != 0,
                .num_lock  = (state & Mod2Mask) != 0};
    }

    void X11Platform::_on_key_press(const XKeyPressedEvent &e, const bool filtered, const unsigned int scancode) {
        const KeyCode kc = _trsc(scancode, m_keycodes);
        if (const auto it = m_window_map.find(e.window); it != m_window_map.end()) {
            it->second->_on_key_press(e, filtered, scancode, kc);
        }
    }

    void X11Platform::_on_key_release(const XKeyReleasedEvent &e, const bool filtered, const unsigned int scancode) {
        const KeyCode kc = _trsc(scancode, m_keycodes);
        if (const auto it = m_window_map.find(e.window); it != m_window_map.end()) {
            it->second->_on_key_release(e, filtered, scancode, kc);
        }
    }

    void X11Platform::_on_button_press(const XButtonPressedEvent &e) {}

    void X11Platform::_on_button_release(const XButtonReleasedEvent &e) {}

    void X11Platform::_on_motion_notify(const XMotionEvent &e) {}

    void X11Platform::_on_enter_notify(const XEnterWindowEvent &e) {}

    void X11Platform::_on_leave_notify(const XLeaveWindowEvent &e) {}

    void X11Platform::_on_focus_in(const XFocusInEvent &e) {}

    void X11Platform::_on_focus_out(const XFocusOutEvent &e) {}

    void X11Platform::_on_keymap_notify(const XKeymapEvent &e) {}

    void X11Platform::_on_expose(const XExposeEvent &e) {}

    void X11Platform::_on_graphics_expose(const XGraphicsExposeEvent &e) {}

    void X11Platform::_on_no_expose(const XNoExposeEvent &e) {}

    void X11Platform::_on_visibility_notify(const XVisibilityEvent &e) {}

    void X11Platform::_on_create_notify(const XCreateWindowEvent &e) {}

    void X11Platform::_on_destroy_notify(const XDestroyWindowEvent &e) {}

    void X11Platform::_on_unmap_notify(const XUnmapEvent &e) {}

    void X11Platform::_on_map_notify(const XMapEvent &e) {}

    void X11Platform::_on_map_request(const XMapRequestEvent &e) {}

    void X11Platform::_on_reparent_notify(const XReparentEvent &e) {}

    void X11Platform::_on_configure_notify(const XConfigureEvent &e) {
        if (const auto it = m_window_map.find(e.window); it != m_window_map.end()) {
            const auto window = it->second;
            window->_try_on_resize({static_cast<uint32_t>(e.width), static_cast<uint32_t>(e.height)});
        }
    }

    void X11Platform::_on_configure_request(const XConfigureRequestEvent &e) {}

    void X11Platform::_on_gravity_notify(const XGravityEvent &e) {}

    void X11Platform::_on_circulate_notify(const XCirculateEvent &e) {}

    void X11Platform::_on_circulate_request(const XCirculateRequestEvent &e) {}

    void X11Platform::_on_property_notify(const XPropertyEvent &e) {}

    void X11Platform::_on_selection_clear(const XSelectionClearEvent &e) {}

    void X11Platform::_on_selection_request(const XSelectionRequestEvent &e) {}

    void X11Platform::_on_selection_notify(const XSelectionEvent &e) {}

    void X11Platform::_on_colormap_notify(const XColormapEvent &e) {}

    void X11Platform::_on_client_message(const XClientMessageEvent &e) {
        if (e.message_type == m_atom_wm_protocols && e.format == 32 && e.data.l[0] == m_atom_wm_delete_window) {
            if (const auto it = m_window_map.find(e.window); it != m_window_map.end()) {
                bool       close  = true;
                const auto window = it->second;
                window->call_close_request_callback(&close);

                if (close) {
                    m_open_windows.erase(e.window);
                    m_window_map.erase(e.window);
                    window->call_close_callback();
                }
            }
        }
    }

    void X11Platform::_on_mapping_notify(const XMappingEvent &e) {}

    void X11Platform::_on_generic_event(const XGenericEvent &e) {}

    void X11Platform::_on_randr_notify(XRRNotifyEvent &e) {
        XRRUpdateConfiguration(reinterpret_cast<XEvent *>(&e));

        switch (e.subtype) {
        case RRNotify_CrtcChange:
            _on_randr_crtc_change_notify(*reinterpret_cast<const XRRCrtcChangeNotifyEvent *>(&e));
            break;
        case RRNotify_OutputChange:
            _on_randr_output_change_notify(*reinterpret_cast<const XRROutputChangeNotifyEvent *>(&e));
            break;
        case RRNotify_OutputProperty:
            _on_randr_output_property_notify(*reinterpret_cast<const XRROutputPropertyNotifyEvent *>(&e));
            break;
        default:
            break;
        }
    }

    void X11Platform::_on_randr_screen_change_notify(XRRScreenChangeNotifyEvent &e) {
        XRRUpdateConfiguration(reinterpret_cast<XEvent *>(&e));
        _update_screen_resources();
    }

    void X11Platform::_on_randr_crtc_change_notify(const XRRCrtcChangeNotifyEvent &e) {
        _update_screen_resources();
    }

    void X11Platform::_on_randr_output_change_notify(const XRROutputChangeNotifyEvent &e) {
        _update_screen_resources();
    }

    void X11Platform::_on_randr_output_property_notify(const XRROutputPropertyNotifyEvent &e) {
        _update_screen_resources();
    }

    void X11Platform::_update_screen_resources() {
        if (m_screen_resources)
            XRRFreeScreenResources(m_screen_resources);
        m_screen_resources = XRRGetScreenResources(m_display, m_root_window);
    }

    X11Window::X11Window(const WindowDescription &description) {
        const auto *platform = _get_platform();

        glm::ivec2 pos;

        switch (static_cast<WindowPlacementMethod>(description.placement.index())) {
        case WindowPlacementMethod::USE_DEFAULT:
            pos = platform->get_default_window_position(description.size, platform->get_primary_monitor_index());
            break;
        case WindowPlacementMethod::FIXED_POSITION:
            pos = std::get<WindowFixedPosition>(description.placement).position;
            break;
        case WindowPlacementMethod::CENTER_MONITOR:
            pos = platform->get_default_window_position(description.size, std::get<WindowCenterMonitor>(description.placement).monitor_index);
            break;
        case WindowPlacementMethod::CENTER_HOVERED_MONITOR:
            pos = platform->get_default_window_position(description.size, platform->get_monitor_index_by_point(platform->get_cursor_pos()));
        }

        XSetWindowAttributes swa{};
        // just give me all my shit
        swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask | ButtonMotionMask | ButtonPressMask | ButtonReleaseMask | EnterWindowMask | LeaveWindowMask |
            StructureNotifyMask | SubstructureNotifyMask | SubstructureRedirectMask | VisibilityChangeMask | FocusChangeMask | PropertyChangeMask | ColormapChangeMask |
            OwnerGrabButtonMask;

        m_window = XCreateWindow(platform->display(), platform->root_window(), pos.x, pos.y, description.size.x, description.size.y, 0, XDefaultDepthOfScreen(platform->screen()),
                                 InputOutput, XDefaultVisualOfScreen(platform->screen()), CWEventMask, &swa);
        XMapWindow(platform->display(), m_window);
        XStoreName(platform->display(), m_window, description.title.c_str());

        Atom protocols[1] = {platform->m_atom_wm_delete_window};
        XSetWMProtocols(platform->display(), m_window, protocols, 1);

        XSizeHints *hints = XAllocSizeHints();
        hints->flags |= PSize | PPosition | PWinGravity;
        hints->width  = static_cast<int>(description.size.x);
        hints->height = static_cast<int>(description.size.y);

        hints->x = pos.x;
        hints->y = pos.y;

        hints->win_gravity = StaticGravity;

        if (!description.resizable) {
            hints->flags |= PMinSize | PMaxSize;
            hints->min_width = hints->max_width = static_cast<int>(description.size.x);
            hints->min_height = hints->max_height = static_cast<int>(description.size.y);
        }

        XSetWMNormalHints(platform->display(), m_window, hints);
        XFree(hints);

        memset(m_current_keystate, 0, sizeof(m_current_keystate));
        memset(m_key_timings, 0, sizeof(m_key_timings));
    }

    X11Window::~X11Window() {
        XDestroyWindow(_get_platform()->display(), m_window);
    }

    glm::uvec2 X11Window::get_inner_size() const {
        XWindowAttributes attribs;
        XGetWindowAttributes(_get_platform()->display(), m_window, &attribs);
        return {static_cast<uint32_t>(attribs.width), static_cast<uint32_t>(attribs.height)};
    }


    void X11Window::trigger_close() {
        XEvent e;
        e.xclient.type = ClientMessage;
        e.xclient.window = m_window;
        e.xclient.message_type = _get_platform()->m_atom_wm_protocols;
        e.xclient.format = 32;
        e.xclient.data.l[0] = _get_platform()->m_atom_wm_delete_window;
        e.xclient.data.l[1] = CurrentTime;
        XSendEvent(_get_platform()->display(), m_window, false, NoEventMask, &e);
    }

    void X11Window::_try_on_resize(const glm::uvec2 &new_size) {
        if (new_size.x != m_cached_size.x || new_size.y != m_cached_size.y) {
            call_resize_callback(new_size);
            m_cached_size = new_size;
        }
    }

    void X11Window::_on_key_press(const XKeyPressedEvent &e, const bool filtered, const unsigned int scancode, const KeyCode keycode) {
        const KeyMods mods = _trmods(e.state);

        if (const Time event_time_diff = e.time - m_key_timings[scancode];
            event_time_diff == e.time || 0 < event_time_diff && event_time_diff < 0x80000000) { // avoid duplicate events
            call_key_pressed_callback(keycode, mods, scancode, m_current_keystate[scancode]);
            m_current_keystate[scancode] = true;
        }

        if (!filtered) {
            // TODO: text input
        }
    }

    void X11Window::_on_key_release(const XKeyPressedEvent &e, const bool filtered, const unsigned int scancode, const KeyCode keycode) {
        const KeyMods mods = _trmods(e.state);

        // we don't like repeats
        if (!_get_platform()->m_xkb_auto_repeat_detectable) {
            if (XEventsQueued(_get_platform()->m_display, QueuedAfterReading)) {
                XEvent e2;
                XPeekEvent(_get_platform()->m_display, &e2);
                if (e2.type == KeyPress && e2.xkey.window == e.window && e2.xkey.keycode == scancode && e2.xkey.time - e.time < 20) return;
            }
        }

        m_current_keystate[scancode] = false;
        call_key_released_callback(keycode, mods, scancode);
    }
} // namespace neuron
#endif
