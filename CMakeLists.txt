cmake_minimum_required(VERSION 3.29)
project(neuron_engine VERSION 0.2.0)

if (PROJECT_IS_TOP_LEVEL)
    option(NEURON_BUILD_EXAMPLES "Build the Neuron example project" ON)
else()
    option(NEURON_BUILD_EXAMPLES "Build the Neuron example project" OFF)
endif()

add_subdirectory(libs/stem)
add_subdirectory(libs/synapse)
add_subdirectory(libs/occipital)
add_subdirectory(libs/neuron)

if (NEURON_BUILD_EXAMPLES)
    add_subdirectory(example)
endif()

