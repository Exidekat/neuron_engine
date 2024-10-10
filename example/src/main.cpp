

#include "neuron/neuron.hpp"
#include "neuron/platform/generic.hpp"

int main() {
    neuron::Platform::init();

    neuron::Platform::cleanup();

    return 0;
}
