cmake_minimum_required(VERSION 3.10)
project(Raytracer)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)

# Add the source and include directories
include_directories(include)

# Create a library from the source files
file(GLOB SOURCES "src/*.cpp")
add_library(raytracer_lib ${SOURCES})

# Add the main raytracer executable, linking against the library
add_executable(raytracer ${SOURCES})
target_link_libraries(raytracer PRIVATE raytracer_lib)

# Set up the Catch2 testing executable
file(GLOB TEST_SOURCES "test/*.cpp")
add_executable(test_ray ${TEST_SOURCES} test/catch_amalgamated.cpp)
target_link_libraries(test_ray PRIVATE raytracer_lib)

# Add a custom target for "make test" to run test_ray
add_custom_target(test COMMAND test_ray DEPENDS test_ray)
