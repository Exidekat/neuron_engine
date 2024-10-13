#include "neuron/neuron.hpp"
#include "neuron/platform/generic.hpp"

#include <iostream>
#include <ostream>

#include <chrono>

void run() {
    const auto window = neuron::Window::create(neuron::WindowDescription{
        .title     = "Hello!",
        .size      = {800, 600},
        .placement = neuron::WindowCenterMonitor{0},
        .resizable = false,
    });

    window.lock()->set_on_resize([](const glm::uvec2& new_size) {
        std::cout << "Size: " << new_size.x << ", " << new_size.y << std::endl;
    });

    window.lock()->set_on_close([] {
        std::cout << "Close" << std::endl;
    });

    window.lock()->set_on_key_pressed([](const neuron::KeyCode keycode, const neuron::KeyMods &mods, const unsigned int scancode, const bool is_repeat) {
        std::cout << std::hex << static_cast<uint16_t>(keycode) << std::dec << " -- " << is_repeat << std::endl;
    });

    window.lock()->set_on_key_released([window](const neuron::KeyCode keycode, const neuron::KeyMods& mods, const unsigned int scancode) {
        if (keycode == neuron::KeyCode::Escape && mods.control) {
            window.lock()->trigger_close();
        }
    });

    using clock = std::chrono::high_resolution_clock;
    using duration = std::chrono::duration<double>;
    using time_point = std::chrono::time_point<clock, duration>;

    time_point this_frame = clock::now();
    duration delta_time(1.0 / 60.0);
    time_point last_frame = this_frame - delta_time;

    while (neuron::Platform::get()->get_window_count()) {
        neuron::Platform::get()->step_event_loop();

        last_frame = this_frame;
        while ((clock::now() - last_frame).count() < 1.0 / 60.0) { (void)0; }
        this_frame = clock::now();
        delta_time = this_frame - last_frame;
    }
}

int main() {
    try {
        neuron::Platform::init();

        run();

        neuron::Platform::cleanup();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
