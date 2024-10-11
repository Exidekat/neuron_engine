#ifdef __unix__
#include "wayland.hpp"

namespace neuron {

    WaylandPlatform::WaylandPlatform() = default;

    std::weak_ptr<Window> WaylandPlatform::create_window(const WindowDescription &description) {
        return {};
    }

    void WaylandPlatform::run_event_loop() {}
} // namespace neuron
#endif
