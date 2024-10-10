add_executable(example src/main.cpp)
target_include_directories(example PRIVATE src/)
target_link_libraries(example PRIVATE neuron::neuron)