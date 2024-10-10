cmake_minimum_required(VERSION 3.26)
project(neuron VERSION 0.1.0 LANGUAGES C CXX)

set(NEURON_GLSLC_PATH "" CACHE STRING "Custom path to the glslc executable")
# allow user to specify path to vulkan sdk

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SRCS CONFIGURE_DEPENDS src/*.c src/*.cpp src/*.cxx src/*.s src/*.cc)

file(GLOB_RECURSE CFG_FILE_INPUTS CONFIGURE_DEPENDS cfi/*.in)

foreach(CFG_FILE_INPUT ${CFG_FILE_INPUTS})
    cmake_path(REMOVE_EXTENSION CFG_FILE_INPUT LAST_ONLY OUTPUT_VARIABLE CFG_FILE_OUTPUT)
    cmake_path(RELATIVE_PATH CFG_FILE_OUTPUT BASE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    message(STATUS "${CFG_FILE_INPUT} -> ${CFG_FILE_OUTPUT}")
    configure_file("${CFG_FILE_INPUT}" "${CFG_FILE_OUTPUT}")
endforeach()

add_library(neuron ${SRCS})
target_include_directories(neuron PUBLIC src/) # TODO: consider using an include/ folder which contains the interface and changing the src folder to PRIVATE
target_include_directories(neuron PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/cfi)

if (UNIX)
    target_link_libraries(neuron PUBLIC X11 Xrandr Xinerama Xi Xcursor)
elseif(WIN32)
    target_link_libraries(neuron PUBLIC Dwmapi)
endif()

find_package(Vulkan REQUIRED) 

target_link_libraries(neuron PUBLIC Vulkan::Vulkan)

if (NOT (NEURON_GLSLC_PATH STREQUAL ""))
    set(GLSLC_FOUND 1)
    set(GLSLC ${NEURON_GLSLC_PATH})
else()
    set(GLSLC_FOUND 0)

    if (Vulkan_glslc_FOUND)
        set(GLSLC Vulkan::glslc)
        set(GLSLC_FOUND)
    else()
        find_program(GLSLC glslc NAMES glslc.exe HINTS ENV VULKAN_SDK PATH_SUFFIXES Bin bin)
        if (GLSLC STREQUAL "GLSLC-NOTFOUND")
            message(WARNING "Failed to find GLSLC. Shaders must be compiled manually or a path to GLSLC should be specified via the NEURON_GLSLC_PATH variable")
        endif()
    endif()
endif()

if (Vulkan_shaderc_combined_FOUND)
    target_link_libraries(neuron PUBLIC Vulkan::shaderc_combined)
else()
    if (BUILD_SHARED_LIBS)
        find_library(SHADERC_SHARED_PATH shaderc_shared HINTS ENV VULKAN_SDK PATH_SUFFIXES Lib lib REQUIRED)
        target_link_libraries(neuron PUBLIC ${SHADERC_SHARED_PATH})
    else()
        find_library(SHADERC_COMBINED_PATH shaderc_combined HINTS ENV VULKAN_SDK PATH_SUFFIXES Lib lib REQUIRED)
        target_link_libraries(neuron PUBLIC ${SHADERC_COMBINED_PATH})
    endif()

    find_path(SHADERC_INCLUDE_DIR shaderc/shaderc.hpp HINTS ENV VULKAN_SDK PATH_SUFFIXES Include include REQUIRED)
    target_include_directories(neuron PUBLIC ${SHADERC_INCLUDE_DIR})
endif()

if (BUILD_SHARED_LIBS)
    target_compile_definitions(neuron PUBLIC -DNEURON_SHARED)
    target_compile_definitions(neuron PRIVATE -DNEURON_SHARED_EXPORTS)
endif()

add_library(neuron::neuron ALIAS neuron)

add_subdirectory(example)
