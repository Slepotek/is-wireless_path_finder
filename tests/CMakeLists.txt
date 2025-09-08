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
add_subdirectory(path_tests)
# add_subdirectory(integration)     # Future

# Find valgrind
find_program(VALGRIND_PROGRAM valgrind)

if(VALGRIND_PROGRAM)
    # Add memory check tests for each module
    add_test(
        NAME matrix_utils_memcheck
        COMMAND ${VALGRIND_PROGRAM}
            --leak-check=full
            --show-leak-kinds=all
            --track-origins=yes
            --error-exitcode=1
            $<TARGET_FILE:test_matrix_utils>
    )

    add_test(
        NAME path_memcheck
        COMMAND ${VALGRIND_PROGRAM}
            --leak-check=full
            --show-leak-kinds=all
            --track-origins=yes
            --error-exitcode=1
            $<TARGET_FILE:test_path>
    )

    # Make memcheck tests depend on regular tests
    set_tests_properties(matrix_utils_memcheck PROPERTIES DEPENDS MatrixUtilsTest)
    set_tests_properties(path_memcheck PROPERTIES DEPENDS PathTest)
endif()