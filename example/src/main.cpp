
#include "neuron/neuron.hpp"
#include "neuron/platform/generic.hpp"

#include <iostream>
#include <ostream>

void run() {
    auto window = neuron::Window::create(neuron::WindowDescription{
        .title     = "Hello!",
        .size      = {800, 600},
        .position  = std::nullopt,
        .resizable = false,
    });

    neuron::Platform::get()->run_event_loop();
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
