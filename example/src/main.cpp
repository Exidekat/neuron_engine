

#include "neuron/neuron.hpp"
#include "neuron/platform/generic.hpp"

#include <iostream>
#include <ostream>

int main() {
    try {
        neuron::Platform::init();

        neuron::Platform::cleanup();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
