#include "generic.hpp"

namespace neuron {
    namespace detail {
        std::shared_ptr<Platform> platform;

        extern std::shared_ptr<Platform> create_win32_platform();
        extern std::shared_ptr<Platform> create_linux_platform();
    }

    std::shared_ptr<Platform> Platform::get() {
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

    void Platform::cleanup() {}
}