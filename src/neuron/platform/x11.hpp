#pragma once
#ifdef __unix__
#include "linux.hpp"

#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>

namespace neuron {
    class X11Window;

    class NEURON_API X11Platform final : public LinuxPlatform {
      public:
         X11Platform();
        ~X11Platform() override;

        std::weak_ptr<Window> create_window(const WindowDescription &description) override;
        [[nodiscard]] size_t  get_window_count() const override;
        void                  run_event_loop() override;
        void                  step_event_loop() override;

        [[nodiscard]] inline Display *display() const noexcept { return m_display; }

        [[nodiscard]] inline Screen *screen() const noexcept { return m_screen; }

        [[nodiscard]] inline ::Window root_window() const noexcept { return m_root_window; }

        glm::ivec2 get_default_window_position(const glm::uvec2 &size, uint32_t target_monitor) const;
        size_t     get_primary_monitor_index() const;
        Atom       intern_atom(const std::string &text, bool create_not_exists = false) const;
        size_t     get_monitor_index_by_point(const glm::ivec2 &point) const;
        glm::ivec2 get_cursor_pos() const;

      protected:
        friend class X11Window;
        void register_new_window(::Window window, const std::shared_ptr<X11Window> &p_window);

        Atom m_atom_wm_delete_window;
        Atom m_atom_wm_protocols;

      private:
        Display            *m_display;
        Screen             *m_screen;
        ::Window            m_root_window;
        XRRScreenResources *m_screen_resources;

        int m_randr_event_base;

        std::unordered_set<::Window>                             m_open_windows;
        std::unordered_map<::Window, std::shared_ptr<X11Window>> m_window_map;

        glm::ivec2         _get_default_window_position(const glm::uvec2 &size, RROutput output) const;
        RROutput           _get_primary_output() const;
        const XRRModeInfo &_get_mode_info(RRMode mode) const;
        bool               _output_contains_point(RROutput output, const glm::ivec2 &point) const;
        void               _handle_event(const XEvent &event);

        // Event Loop Handlers
        void _on_key_press(const XKeyPressedEvent &e);
        void _on_key_release(const XKeyReleasedEvent &e);
        void _on_button_press(const XButtonPressedEvent &e);
        void _on_button_release(const XButtonReleasedEvent &e);
        void _on_motion_notify(const XMotionEvent &e);
        void _on_enter_notify(const XEnterWindowEvent &e);
        void _on_leave_notify(const XLeaveWindowEvent &e);
        void _on_focus_in(const XFocusInEvent &e);
        void _on_focus_out(const XFocusOutEvent &e);
        void _on_keymap_notify(const XKeymapEvent &e);
        void _on_expose(const XExposeEvent &e);
        void _on_graphics_expose(const XGraphicsExposeEvent &e);
        void _on_no_expose(const XNoExposeEvent &e);
        void _on_visibility_notify(const XVisibilityEvent &e);
        void _on_create_notify(const XCreateWindowEvent &e);
        void _on_destroy_notify(const XDestroyWindowEvent &e);
        void _on_unmap_notify(const XUnmapEvent &e);
        void _on_map_notify(const XMapEvent &e);
        void _on_map_request(const XMapRequestEvent &e);
        void _on_reparent_notify(const XReparentEvent &e);
        void _on_configure_notify(const XConfigureEvent &e);
        void _on_configure_request(const XConfigureRequestEvent &e);
        void _on_gravity_notify(const XGravityEvent &e);
        void _on_circulate_notify(const XCirculateEvent &e);
        void _on_circulate_request(const XCirculateRequestEvent &e);
        void _on_property_notify(const XPropertyEvent &e);
        void _on_selection_clear(const XSelectionClearEvent &e);
        void _on_selection_request(const XSelectionRequestEvent &e);
        void _on_selection_notify(const XSelectionEvent &e);
        void _on_colormap_notify(const XColormapEvent &e);
        void _on_client_message(const XClientMessageEvent &e);
        void _on_mapping_notify(const XMappingEvent &e);
        void _on_generic_event(const XGenericEvent &e);

        void _on_randr_notify(const XRRNotifyEvent &e);
        void _on_randr_screen_change_notify(const XRRScreenChangeNotifyEvent &e);
        void _on_randr_crtc_change_notify(const XRRCrtcChangeNotifyEvent &e);
        void _on_randr_output_change_notify(const XRROutputChangeNotifyEvent &e);
        void _on_randr_output_property_notify(const XRROutputPropertyNotifyEvent &e);

        void _update_screen_resources();
    };

    class NEURON_API X11Window final : public Window {
        friend class X11Platform;
        explicit X11Window(const WindowDescription &description);

      public:
        ~X11Window() override;

        glm::uvec2 get_inner_size() const override;

        [[nodiscard]] inline ::Window x11_handle() const { return m_window; }

      private:
        ::Window m_window;
        glm::uvec2 m_cached_size;

        void _try_on_resize(const glm::uvec2& new_size);
    };
} // namespace neuron
#endif
