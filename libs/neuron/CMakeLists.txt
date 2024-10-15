cmake_minimum_required(VERSION 3.29)
project(neuron VERSION 0.2.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_library(neuron src/neuron/neuron.cpp)
target_include_directories(neuron PUBLIC src/)
target_link_libraries(neuron PUBLIC neuron::occipital neuron::stem neuron::synapse)
target_compile_definitions(neuron PUBLIC NEURON_VERSION="${neuron_VERSION}" NEURON_VERSION_MAJOR=${neuron_VERSION_MAJOR} NEURON_VERSION_MINOR=${neuron_VERSION_MINOR} NEURON_VERSION_PATCH=${neuron_VERSION_PATCH})

if (BUILD_SHARED_LIBS)
    target_compile_definitions(neuron PUBLIC NEURON_SHARED)
    target_compile_definitions(neuron PRIVATE NEURON_SHARED_EXPORTS)
endif()

add_library(neuron::neuron ALIAS neuron)

