#pragma once
#ifdef WIN32
#ifdef UNICODE
#undef UNICODE
#endif

#include "generic.hpp"
#include "neuron/synapse/pre.hpp"

#include <Windows.h>
#include <unordered_set>

namespace neuron::synapse {
    class Win32Window;

    class NEURON_API Win32Platform final : public Platform {
      public:
         Win32Platform();
        ~Win32Platform() override;

        std::weak_ptr<Window> create_window(const WindowDescription &description) override;
        [[nodiscard]] size_t  get_window_count() const override;
        void                  run_event_loop() override;
        void                  step_event_loop() override;

        [[nodiscard]] inline HINSTANCE hinstance() const { return m_hinstance; };

      private:
        friend class Win32Window;
        HINSTANCE m_hinstance;

        std::unordered_set<HWND>                               m_open_windows;
        std::unordered_map<HWND, std::shared_ptr<Win32Window>> m_window_map;
    };

    class NEURON_API Win32Window final : public Window {
        friend class Win32Platform;
        explicit Win32Window(const WindowDescription &description);

      public:
        ~Win32Window() override = default;

        glm::uvec2 get_inner_size() const override;
        void       trigger_close() override;

        [[nodiscard]] inline HWND win32_handle() const { return m_hwnd; };

      private:
        HWND m_hwnd;
    };
} // namespace neuron

#endif
