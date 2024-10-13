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

    const auto window2 = neuron::Window::create(neuron::WindowDescription{
        .title     = "Hello 2!",
        .size      = {800, 600},
        .placement = neuron::WindowCenterMonitor{1},
        .resizable = true,
    });

    window.lock()->set_on_resize([](const glm::uvec2& new_size) {
        std::cout << "W1 Size: " << new_size.x << ", " << new_size.y << std::endl;
    });

    window.lock()->set_on_close([] {
        std::cout << "Close W1" << std::endl;
    });

    window2.lock()->set_on_resize([](const glm::uvec2& new_size) {
        std::cout << "W2 Size: " << new_size.x << ", " << new_size.y << std::endl;
    });

    window2.lock()->set_on_close([] {
        std::cout << "Close W2" << std::endl;
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
