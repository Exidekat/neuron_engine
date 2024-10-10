#pragma once
#ifdef __unix__
#include "linux.hpp"

#include <unordered_set>
#include <vector>

#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>

namespace neuron {
    class X11Platform : public LinuxPlatform {
      public:
         X11Platform();
        ~X11Platform() override;

        std::shared_ptr<Window> create_window(const WindowDescription &description) override;

        glm::ivec2 get_default_window_position(const glm::uvec2 &size, uint32_t target_monitor) const;
        size_t     get_primary_monitor_index() const;

      private:
        Display *m_display;
        Screen  *m_screen;
        ::Window m_root_window;

        XRRScreenResources *m_screen_resources;

        std::unordered_set<::Window> m_open_windows;

        glm::ivec2 _get_default_window_position(const glm::uvec2 &size, RROutput output) const;

        RROutput           _get_primary_output() const;
        const XRRModeInfo &_get_mode_info(RRMode mode) const;
    };
} // namespace neuron
#endif
