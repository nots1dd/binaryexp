cmake_minimum_required(VERSION 3.10)

project(DemoProjects)

# Set C and C++ compilers
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Set the output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/demos)

# Function to add a C executable
function(add_c_executable target source)
    add_executable(${target} ${source})
    set_target_properties(${target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/c/")
endfunction()

# Function to add a C++ executable
function(add_cpp_executable target source)
    add_executable(${target} ${source})
    set_target_properties(${target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cpp/")
endfunction()

# Add C executables
add_c_executable(segfault demos/c/demo1/segfault.c)
add_c_executable(system demos/c/demo2/system.c)
add_c_executable(victim demos/c/demo3/victim.c)
add_c_executable(exploit demos/c/demo3/exploit.c)

# Add C++ executable
add_cpp_executable(env_var demos/cpp/env_var.cpp)
