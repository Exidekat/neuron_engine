#include "win32.hpp"
#include <stdexcept>
#ifdef WIN32
namespace neuron {
    namespace detail {
        std::unique_ptr<Platform> create_win32_platform() {
            return std::make_unique<Win32Platform>();
        }
    } // namespace detail

    Win32Platform::Win32Platform() {
        throw std::runtime_error("Win32 is not yet implemented.");
    }

    Win32Platform::~Win32Platform() {}

    std::shared_ptr<Window> Win32Platform::create_window(const WindowDescription &description) {
        return {};
    }
} // namespace neuron

#else
// define neuron::detail::create_win32_platform as a runtime error

namespace neuron::detail {
    std::unique_ptr<Platform> create_win32_platform() {
        throw std::runtime_error("Win32 platform can only be created in Windows.");
    }
} // namespace neuron::detail


#endif
