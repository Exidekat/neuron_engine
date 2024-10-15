#ifdef __APPLE__
#include "macos.hpp"
#include "cocoas.hpp"

#include <stdexcept>
#include <memory>

namespace neuron {
    namespace detail {
        std::unique_ptr<Platform> create_macos_platform() {
            return std::make_unique<CocoaPlatform>();
        }
    }

    MacOSPlatform::MacOSPlatform() = default;

    MacOSPlatform::~MacOSPlatform() = default;
}
#else

namespace neuron::detail {
    std::unique_ptr<Platform> create_macos_platform() {
        throw std::runtime_error("macOS platform can only be created on macOS.");
    }
}
#endif