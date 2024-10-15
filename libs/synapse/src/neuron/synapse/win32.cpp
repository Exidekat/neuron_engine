#include "win32.hpp"
#include <stdexcept>
#ifdef WIN32
namespace neuron::synapse {
    namespace detail {
        static auto CLASSNAME = "neuron_class_name";

        std::unique_ptr<Platform> create_win32_platform() {
            return std::make_unique<Win32Platform>();
        }
    } // namespace detail

    inline Win32Platform *_get_platform() {
        return dynamic_cast<Win32Platform *>(Platform::get().get());
    }

    Win32Platform::Win32Platform() {
        throw std::runtime_error("Win32 is not yet implemented.");
    }

    Win32Platform::~Win32Platform() {}

    std::weak_ptr<Window> Win32Platform::create_window(const WindowDescription &description) {
        return {};
    }

    size_t Win32Platform::get_window_count() const {
        return 0;
    }

    void Win32Platform::run_event_loop() {}

    void Win32Platform::step_event_loop() {}

    Win32Window::Win32Window(const WindowDescription &description) {
        DWORD ex_style = WS_EX_OVERLAPPEDWINDOW;
        DWORD style = WS_OVERLAPPEDWINDOW;

        int x, y;

        switch (static_cast<WindowPlacementMethod>(description.placement.index())) {
        case WindowPlacementMethod::USE_DEFAULT:
            x = y = CW_USEDEFAULT;
            break;
        case WindowPlacementMethod::FIXED_POSITION:
            { const auto& p = std::get<WindowFixedPosition>(description.placement).position; x = p.x; y = p.y; };
            break;
        case WindowPlacementMethod::CENTER_MONITOR:

            break;
        case WindowPlacementMethod::CENTER_HOVERED_MONITOR:
            break;
        }

        m_hwnd = CreateWindowExA(ex_style, detail::CLASSNAME, description.title.c_str(), style, x, y, description.size.x, description.size.y, nullptr, nullptr,
                                 _get_platform()->hinstance(), this);
    }

    glm::uvec2 Win32Window::get_inner_size() const {
        RECT r;
        GetClientRect(m_hwnd, &r);
        return {r.right - r.left, r.bottom - r.top};
    }

    void Win32Window::trigger_close() {
        SendMessageA(m_hwnd, WM_CLOSE, 0, 0);
    }
} // namespace neuron

#else
// define neuron::detail::create_win32_platform as a runtime error

#include "neuron/platform/generic.hpp"
#include <memory>
#include <stdexcept>

namespace neuron::synapse::detail {
    std::unique_ptr<Platform> create_win32_platform() {
        throw std::runtime_error("Win32 platform can only be created in Windows.");
    }
} // namespace neuron::detail


#endif
