#ifdef __unix__
#include "wayland.hpp"

namespace neuron::synapse {

    WaylandPlatform::WaylandPlatform() = default;

    std::weak_ptr<Window> WaylandPlatform::create_window(const WindowDescription &description) {
        return {};
    }

    size_t WaylandPlatform::get_window_count() const {
        return 0;
    }

    void WaylandPlatform::run_event_loop() {}

    void WaylandPlatform::step_event_loop() {}
} // namespace neuron
#endif
