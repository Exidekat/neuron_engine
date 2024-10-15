#ifdef __APPLE__
#include "macos.hpp"

#else
namespace neuron::detail {
    std::unique_ptr<Platform> create_macos_platform() {
        throw std::runtime_error("Macos platform can only be created on Macos.");
    }
} // namespace neuron::detail
#endif