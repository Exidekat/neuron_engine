cmake_minimum_required(VERSION 3.29)
project(neuron_synapse VERSION 0.2.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_library(neuron_synapse src/neuron/synapse/generic.cpp src/neuron/synapse/generic.hpp
        src/neuron/synapse/linux.cpp src/neuron/synapse/linux.hpp
        src/neuron/synapse/pre.hpp
        src/neuron/synapse/wayland.cpp src/neuron/synapse/wayland.hpp
        src/neuron/synapse/win32.cpp src/neuron/synapse/win32.hpp
        src/neuron/synapse/x11.cpp src/neuron/synapse/x11.hpp
        src/neuron/synapse/cocoas.cpp src/neuron/synapse/cocoas.hpp
        src/neuron/synapse/macos.cpp src/neuron/synapse/macos.hpp)

if (APPLE)
    target_sources(neuron_synapse src/neuron/synapse/cocoas.mm src/neuron/synapse/macos.mm)
endif()

target_include_directories(neuron_synapse PUBLIC src/)
target_link_libraries(neuron_synapse PUBLIC neuron::stem)
target_compile_definitions(neuron_synapse PUBLIC SYNAPSE_VERSION="${neuron_synapse_VERSION}" SYNAPSE_VERSION_MAJOR=${neuron_synapse_VERSION_MAJOR} SYNAPSE_VERSION_MINOR=${neuron_synapse_VERSION_MINOR} SYNAPSE_VERSION_PATCH=${neuron_synapse_VERSION_PATCH})

if (BUILD_SHARED_LIBS)
    target_compile_definitions(neuron_synapse PUBLIC SYNAPSE_SHARED)
    target_compile_definitions(neuron_synapse PRIVATE SYNAPSE_SHARED_EXPORTS)
endif ()

if (APPLE)
    target_link_libraries(neuron_synapse PUBLIC "-framework Cocoa" "-framework IOKit" "-framework CoreFoundation")
elseif (LINUX)
    target_link_libraries(neuron_synapse PUBLIC X11 Xrandr Xinerama Xi Xcursor)
elseif (WIN32)
    target_link_libraries(neuron_synapse PUBLIC Dwmapi)
endif ()



add_library(neuron::synapse ALIAS neuron_synapse)