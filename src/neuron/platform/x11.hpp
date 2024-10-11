#pragma once
#ifdef __unix__
#include "linux.hpp"

#include <unordered_set>
#include <unordered_map>
#include <vector>

#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>

namespace neuron {
    class X11Window;

    class NEURON_API X11Platform final : public LinuxPlatform {
      public:
         X11Platform();
        ~X11Platform() override;

        std::shared_ptr<Window> create_window(const WindowDescription &description) override;

        glm::ivec2 get_default_window_position(const glm::uvec2 &size, uint32_t target_monitor) const;
        size_t     get_primary_monitor_index() const;

        [[nodiscard]] inline Display *display() const noexcept { return m_display; }

        [[nodiscard]] inline Screen *screen() const noexcept { return m_screen; }

        [[nodiscard]] inline ::Window root_window() const noexcept { return m_root_window; }

        void run_event_loop() override;

        Atom intern_atom(const std::string &text, bool create_not_exists = false) const;

      protected:
        friend class X11Window;
        void register_new_window(::Window window, X11Window* pwindow);

        Atom m_atom_wm_delete_window;
        Atom m_atom_wm_protocols;

      private:
        Display *m_display;
        Screen  *m_screen;
        ::Window m_root_window;

        XRRScreenResources *m_screen_resources;

        std::unordered_set<::Window> m_open_windows;
        std::unordered_map<::Window, X11Window*> m_window_map;

        glm::ivec2 _get_default_window_position(const glm::uvec2 &size, RROutput output) const;

        RROutput           _get_primary_output() const;
        const XRRModeInfo &_get_mode_info(RRMode mode) const;
    };

    class NEURON_API X11Window final : public Window {
        friend class X11Platform;
        explicit X11Window(const WindowDescription &description);

      public:
        ~X11Window() override;

        void close();
      private:
        ::Window m_window;
        bool m_open = true;
    };
} // namespace neuron
#endif
