cmake_minimum_required(VERSION 3.12)
project(is-wireless_path_finding VERSION 1.0.0 LANGUAGES CXX)

# C++ standard in this project
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler flags
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# TODO: Change it to default to the Release
# Default to Debug build
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Libraries
add_subdirectory(lib)

# Main executable
add_executable(pathFinder src/main.cpp)
target_link_libraries(pathFinder pathFinder_lib)

# Optional: GUI support (uncomment if using a GUI library)
# find_package(Qt6 COMPONENTS Core Widgets QUIET)
# if(Qt6_FOUND)
#     target_link_libraries(pathfinder Qt6::Core Qt6::Widgets)
#     target_compile_definitions(pathfinder PRIVATE GUI_ENABLED)
# endif()

# Testing
option(BUILD_TESTS "Build test suites" ON)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Install target
install(TARGETS pathFinder DESTINATION bin)

# Print build info
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")