#include "wayland.hpp"

namespace neuron {

    WaylandPlatform::WaylandPlatform() : LinuxPlatform() {}

    std::shared_ptr<Window> WaylandPlatform::create_window(const WindowDescription &description) {
        return {};
    }
} // namespace neuron
