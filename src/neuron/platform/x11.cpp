#ifdef __unix__
#include "x11.hpp"

#include <glm/gtx/string_cast.hpp>
#include <iostream>

namespace neuron {
    inline X11Platform *_get_platform() {
        return dynamic_cast<X11Platform *>(Platform::get().get());
    }

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
            switch (event.type) {
            case KeyPress:
                _on_key_press(event.xkey);
                break;
            case KeyRelease:
                _on_key_release(event.xkey);
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
            case ResizeRequest:
                _on_resize_request(event.xresizerequest);
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

    void X11Platform::_on_key_press(const XKeyPressedEvent &e) {}

    void X11Platform::_on_key_release(const XKeyReleasedEvent &e) {}

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

    void X11Platform::_on_configure_notify(const XConfigureEvent &e) {}

    void X11Platform::_on_configure_request(const XConfigureRequestEvent &e) {}

    void X11Platform::_on_gravity_notify(const XGravityEvent &e) {}

    void X11Platform::_on_resize_request(const XResizeRequestEvent &e) {}

    void X11Platform::_on_circulate_notify(const XCirculateEvent &e) {}

    void X11Platform::_on_circulate_request(const XCirculateRequestEvent &e) {}

    void X11Platform::_on_property_notify(const XPropertyEvent &e) {}

    void X11Platform::_on_selection_clear(const XSelectionClearEvent &e) {}

    void X11Platform::_on_selection_request(const XSelectionRequestEvent &e) {}

    void X11Platform::_on_selection_notify(const XSelectionEvent &e) {}

    void X11Platform::_on_colormap_notify(const XColormapEvent &e) {}

    void X11Platform::_on_client_message(const XClientMessageEvent &e) {
        if (e.message_type == m_atom_wm_protocols && e.format == 32 && e.data.l[0] == m_atom_wm_delete_window) {
            m_open_windows.erase(e.window);
            m_window_map[e.window]->close();
        }
    }

    void X11Platform::_on_mapping_notify(const XMappingEvent &e) {}

    void X11Platform::_on_generic_event(const XGenericEvent &e) {}

    void X11Platform::_on_randr_notify(const XRRNotifyEvent &e) {
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

    void X11Platform::_on_randr_screen_change_notify(const XRRScreenChangeNotifyEvent &e) {}

    void X11Platform::_on_randr_crtc_change_notify(const XRRCrtcChangeNotifyEvent &e) {}

    void X11Platform::_on_randr_output_change_notify(const XRROutputChangeNotifyEvent &e) {}

    void X11Platform::_on_randr_output_property_notify(const XRROutputPropertyNotifyEvent &e) {}

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
            StructureNotifyMask | SubstructureNotifyMask | SubstructureRedirectMask | VisibilityChangeMask | ResizeRedirectMask | FocusChangeMask | PropertyChangeMask |
            ColormapChangeMask | OwnerGrabButtonMask;

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
    }

    X11Window::~X11Window() {
        close();
    }

    void X11Window::close() {
        if (m_open) {
            XDestroyWindow(_get_platform()->display(), m_window);
            m_open = false;
        }
    }
} // namespace neuron
#endif
