#include "linux.hpp"
#include "x11.hpp"
#include "wayland.hpp"

#include <cstdlib>
#include <cstring>

namespace neuron {
    namespace detail {
        std::shared_ptr<Platform> create_linux_platform() {
            const char *session_type = getenv("XDG_SESSION_TYPE");
            if (strlen(session_type) == 0 || strcmp(session_type, "tty") == 0) {
                throw std::runtime_error("Headless mode is not yet implemented. If you are running X11 or Wayland, set the XDG_SESSION_TYPE environment variable accordingly.");
            }

            if (strcmp(session_type, "x11") == 0) {
                return std::make_shared<X11Platform>();
            }

            if (strcmp(session_type, "wayland") == 0) {
                return std::make_shared<WaylandPlatform>();
            }
        }
    } // namespace detail

    LinuxPlatform::LinuxPlatform() {}

    LinuxPlatform::~LinuxPlatform() {}
} // namespace neuron
