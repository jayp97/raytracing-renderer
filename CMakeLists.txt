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

#test executable for triangle
add_executable(test_triangle test/test_triangle.cpp test/catch_amalgamated.cpp)
target_link_libraries(test_triangle PRIVATE raytracer_lib)

#test executable for cylinder
add_executable(test_cylinder test/test_cylinder.cpp test/catch_amalgamated.cpp)
target_link_libraries(test_cylinder PRIVATE raytracer_lib)

#test executable for scene loader
add_executable(test_scene_loader test/test_scene_loader.cpp test/catch_amalgamated.cpp)
target_link_libraries(test_scene_loader PRIVATE raytracer_lib)

#test executable for raytracer
add_executable(test_raytracer test/test_raytracer.cpp test/catch_amalgamated.cpp)
target_link_libraries(test_raytracer PRIVATE raytracer_lib)

#test executable for binary rendering mode
add_executable(test_binary_rendering_mode test/test_binary_rendering_mode.cpp test/catch_amalgamated.cpp)
target_link_libraries(test_binary_rendering_mode PRIVATE test_binary_rendering_mode)

#test executable for shadow
add_executable(test_shadow test/test_shadow.cpp test/catch_amalgamated.cpp)
target_link_libraries(test_shadow PRIVATE test_shadow)

# Add custom targets for each test executable
add_custom_target(run_test_ray COMMAND test_ray DEPENDS test_ray)
add_custom_target(run_test_camera COMMAND test_camera DEPENDS test_camera)
add_custom_target(run_test_sphere COMMAND test_sphere DEPENDS test_sphere)
add_custom_target(run_test_triangle COMMAND test_triangle DEPENDS test_triangle)
add_custom_target(run_test_cylinder COMMAND test_cylinder DEPENDS test_cylinder)
add_custom_target(run_test_scene_loader COMMAND test_scene_loader DEPENDS test_scene_loader)
add_custom_target(run_test_raytracer COMMAND test_raytracer DEPENDS test_raytracer)
add_custom_target(run_test_binary_rendering_mode COMMAND test_binary_rendering_mode DEPENDS test_binary_rendering_mode)
add_custom_target(run_test_shadow COMMAND test_shadow DEPENDS test_shadow)
