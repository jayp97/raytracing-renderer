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

# Set up separate Catch2 testing executables for each test file
add_executable(test_ray test/test_ray.cpp test/catch_amalgamated.cpp)
target_link_libraries(test_ray PRIVATE raytracer_lib)

#test executable for camera
add_executable(test_camera test/test_camera.cpp test/catch_amalgamated.cpp)
target_link_libraries(test_camera PRIVATE raytracer_lib)

#test executable for sphere
add_executable(test_sphere test/test_sphere.cpp test/catch_amalgamated.cpp)
target_link_libraries(test_sphere PRIVATE raytracer_lib)

#test executable for sphere
add_executable(test_triangle test/test_triangle.cpp test/catch_amalgamated.cpp)
target_link_libraries(test_triangle PRIVATE raytracer_lib)

# Add custom targets for each test executable
add_custom_target(run_test_ray COMMAND test_ray DEPENDS test_ray)
add_custom_target(run_test_camera COMMAND test_camera DEPENDS test_camera)
add_custom_target(run_test_sphere COMMAND test_sphere DEPENDS test_sphere)
add_custom_target(run_test_triangle COMMAND test_triangle DEPENDS test_triangle)
