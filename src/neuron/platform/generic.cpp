#include "generic.hpp"

namespace neuron {
    namespace detail {
        std::unique_ptr<Platform> platform;

        extern std::unique_ptr<Platform> create_win32_platform();
        extern std::unique_ptr<Platform> create_linux_platform();
    }

    const std::unique_ptr<Platform> &Platform::get() {
        return detail::platform;
    }

    void Platform::init() {
#if defined(__unix__)
        detail::platform = detail::create_linux_platform();
#elif defined(WIN32)
        detail::platform = detail::create_win32_platform();
#else
#error "Unsupported System"
#endif
    }

    void Platform::cleanup() {
        detail::platform.reset();
    }

    Window::Window() = default;

    Window::~Window() = default;


    std::weak_ptr<Window> Window::create(const WindowDescription &description) {
        return Platform::get()->create_window(description);
    }
} // namespace neuron
