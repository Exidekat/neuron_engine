cmake_minimum_required(VERSION 3.29)
project(neuron_occipital VERSION 0.2.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_library(neuron_occipital src/neuron/occipital/occipital.cpp)
target_include_directories(neuron_occipital PUBLIC src/)
target_link_libraries(neuron_occipital PUBLIC neuron::stem)
target_compile_definitions(neuron_occipital PUBLIC OCCIPITAL_VERSION="${neuron_occipital_VERSION}" OCCIPITAL_VERSION_MAJOR=${neuron_occipital_VERSION_MAJOR} OCCIPITAL_VERSION_MINOR=${neuron_occipital_VERSION_MINOR} OCCIPITAL_VERSION_PATCH=${neuron_occipital_VERSION_PATCH})

if (BUILD_SHARED_LIBS)
    target_compile_definitions(neuron_occipital PUBLIC OCCIPITAL_SHARED)
    target_compile_definitions(neuron_occipital PRIVATE OCCIPITAL_SHARED_EXPORTS)
endif()

add_library(neuron::occipital ALIAS neuron_occipital)

