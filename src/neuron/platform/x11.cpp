#ifdef __unix__
#include "x11.hpp"

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

        m_atom_wm_protocols = intern_atom("WM_PROTOCOLS");
        m_atom_wm_delete_window = intern_atom("WM_DELETE_WINDOW");
    }

    X11Platform::~X11Platform() {
        XRRFreeScreenResources(m_screen_resources);
        XCloseDisplay(m_display);
    }

    std::shared_ptr<Window> X11Platform::create_window(const WindowDescription &description) {
        return std::shared_ptr<Window>(new X11Window(description));
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
        auto platform = _get_platform();
        XEvent event;
        while (!m_open_windows.empty()) {
            XNextEvent(m_display, &event);
            switch (event.type) {
            case ClientMessage: {
                if (const auto &cm = event.xclient; cm.message_type == platform->m_atom_wm_protocols && cm.format == 32 && cm.data.l[0] == platform->m_atom_wm_delete_window) {
                    m_open_windows.erase(cm.window);
                    m_window_map[cm.window]->close();
                }
            }
            default:
                break;
            }
        }
    }

    Atom X11Platform::intern_atom(const std::string &text, const bool create_not_exists) const {
        return XInternAtom(m_display, text.c_str(), create_not_exists);
    }

    void X11Platform::register_new_window(::Window window, X11Window* pwindow) {
        m_open_windows.insert(window);
        m_window_map.insert({window, pwindow});
    }

    glm::ivec2 X11Platform::_get_default_window_position(const glm::uvec2 &size, const RROutput output) const {
        XRROutputInfo     *output_info = XRRGetOutputInfo(m_display, m_screen_resources, output);
        const RRMode       mode        = output_info->modes[output_info->npreferred];
        const XRRModeInfo &mode_info   = _get_mode_info(mode);
        XRRCrtcInfo       *crtc_info   = XRRGetCrtcInfo(m_display, m_screen_resources, output_info->crtc);

        int32_t x = crtc_info->x + static_cast<int32_t>(mode_info.width - size.x) / 2;
        int32_t y = crtc_info->y + static_cast<int32_t>(mode_info.height - size.y) / 2;

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

    X11Window::X11Window(const WindowDescription &description) {
        auto *platform = _get_platform();

        glm::ivec2 pos;
        if (description.position.has_value()) {
            pos = description.position.value();
        } else {
            pos = platform->get_default_window_position(description.size, platform->get_primary_monitor_index());
            //pos = platform->get_default_window_position(description.size,0);
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

        Atom protocols[1] = { platform->m_atom_wm_delete_window };
        XSetWMProtocols(platform->display(), m_window, protocols, 1);

        if (!description.resizable) {
            XSizeHints* hints = XAllocSizeHints();
            long supplied;
            XGetWMNormalHints(platform->display(), m_window, hints, &supplied);

            hints->flags |= PMinSize | PMaxSize | PSize;
            hints->width = hints->min_width = hints->max_width = static_cast<int>(description.size.x);
            hints->height = hints->min_height = hints->max_height = static_cast<int>(description.size.y);

            XSetWMNormalHints(platform->display(), m_window, hints);
            XFree(hints);
        }

        platform->register_new_window(m_window, this);
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
