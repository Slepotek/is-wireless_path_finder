# Test executable
add_executable(test_program test_main.cpp)
target_link_libraries(test_program pathFinder_lib)

# Register CTest
add_test(NAME ProgramTest COMMAND test_program)

# Set properties
set_target_properties(test_program PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON)

# Add subdirectories for each test module
add_subdirectory(matrix_utils_tests)
# add_subdirectory(path_structure)  # Future
# add_subdirectory(integration)     # Future