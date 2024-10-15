#pragma once
#ifdef __APPLE__
#include "macos.hpp"

#include <unordered_map>
#include <unordered_set>
#include <vector>

#import <Cocoa/Cocoa.h>

namespace neuron {
    class CocoaWindow;

    class NEURON_API CocoaPlatform final : public MacOSPlatform {
      public:
        CocoaPlatform();
        ~CocoaPlatform() override;

        std::weak_ptr<Window> create_window(const WindowDescription &description) override;
        [[nodiscard]] size_t get_window_count() const override;
        void run_event_loop() override;
        void step_event_loop() override;

        [[nodiscard]] inline NSApplication *app() const noexcept { return m_app; }

      private:
        NSApplication *m_app;
        std::unordered_set<NSWindow *> m_open_windows;
        std::unordered_map<NSWindow *, std::shared_ptr<CocoaWindow>> m_window_map;
    };

    class NEURON_API CocoaWindow final : public Window {
      public:
        explicit CocoaWindow(const WindowDescription &description);
        ~CocoaWindow() override;

        glm::uvec2 get_inner_size() const override;
        void trigger_close() override;

        [[nodiscard]] inline NSWindow *cocoa_handle() const { return m_window; }

      private:
        NSWindow *m_window;
        glm::uvec2 m_cached_size;
    };
}
#endif