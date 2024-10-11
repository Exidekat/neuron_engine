#ifdef __unix__
#include "x11.hpp"

#include <iostream>

namespace neuron {
    X11Platform::X11Platform() : LinuxPlatform() {
        m_display     = XOpenDisplay(nullptr);
        m_screen      = XDefaultScreenOfDisplay(m_display);
        m_root_window = XRootWindowOfScreen(m_screen);

        m_screen_resources = XRRGetScreenResourcesCurrent(m_display, m_root_window);

        // pretend to do some funky shit
        const auto pos = get_default_window_position({800, 600}, get_primary_monitor_index());
        std::cout << "Window @ (" << pos.x << ", " << pos.y << ") w/ size 800x600" << std::endl;
    }

    X11Platform::~X11Platform() {
        XRRFreeScreenResources(m_screen_resources);
        XCloseDisplay(m_display);
    }

    std::shared_ptr<Window> X11Platform::create_window(const WindowDescription &description) {
        return {};
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
        for (size_t i = 0 ; i < m_screen_resources->noutput ; ++i) {
            if (m_screen_resources->outputs[i] == output) {
                return i;
            }
        }

        return SIZE_MAX;
    }

    glm::ivec2 X11Platform::_get_default_window_position(const glm::uvec2 &size, RROutput output) const {
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

    const XRRModeInfo &X11Platform::_get_mode_info(RRMode mode) const {
        for (size_t i = 0; i < m_screen_resources->nmode; ++i) {
            if (m_screen_resources->modes[i].id == mode) {
                return m_screen_resources->modes[i];
            }
        }

        throw std::runtime_error("Mode not found");
    }
} // namespace neuron
#endif