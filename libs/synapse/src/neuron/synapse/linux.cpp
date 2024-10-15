#ifdef __unix__
#include "linux.hpp"
#include "x11.hpp"
#include "wayland.hpp"

#include <cstdlib>
#include <cstring>

namespace neuron {
    namespace detail {
        std::unique_ptr<Platform> create_linux_platform() {
            const char *session_type = getenv("XDG_SESSION_TYPE");
            if (strlen(session_type) == 0 || strcmp(session_type, "tty") == 0) {
                throw std::runtime_error("Headless mode is not yet implemented. If you are running X11 or Wayland, set the XDG_SESSION_TYPE environment variable accordingly.");
            }

            if (strcmp(session_type, "x11") == 0) {
                return std::make_unique<X11Platform>();
            }

            if (strcmp(session_type, "wayland") == 0) {
                return std::make_unique<WaylandPlatform>();
            }

            throw std::runtime_error("Headless mode is not yet implemented. If you are running X11 or Wayland, set the XDG_SESSION_TYPE environment variable accordingly. Any other session types will not be supported.");
        }
    } // namespace detail

    LinuxPlatform::LinuxPlatform() = default;

    LinuxPlatform::~LinuxPlatform() = default;
} // namespace neuron

#else

#include <memory>
#include "neuron/platform/generic.hpp"
#include <stdexcept>

namespace neuron::synapse::detail {
    std::unique_ptr<Platform> create_linux_platform() {
        throw std::runtime_error("Linux platform can only be created on Linux.");
    }
} // namespace neuron::detail
#endif