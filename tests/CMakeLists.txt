# CMakeLists.txt for tests directory
# Add subdirectories for each test module
add_subdirectory(matrix_utils_tests)
add_subdirectory(path_tests)
add_subdirectory(path_finder_utils_tests)
add_subdirectory(dfs_algorithm_tests)
add_subdirectory(cli_utils_tests)

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

    add_test(
        NAME path_finder_utils_memcheck
        COMMAND ${VALGRIND_PROGRAM}
            --leak-check=full
            --show-leak-kinds=all
            --track-origins=yes
            --error-exitcode=1
            $<TARGET_FILE:test_path_finder_utils>
    )

    add_test(
        NAME dfs_algorithm_memcheck
        COMMAND ${VALGRIND_PROGRAM}
            --leak-check=full
            --show-leak-kinds=all
            --track-origins=yes
            --error-exitcode=1
            $<TARGET_FILE:test_dfs_algorithm>
    )

    add_test(
        NAME cli_utils_memcheck
        COMMAND ${VALGRIND_PROGRAM}
            --leak-check=full
            --show-leak-kinds=all
            --track-origins=yes
            --error-exitcode=1
            $<TARGET_FILE:test_cli_utils>
    )

    # Make memcheck tests depend on regular tests
    set_tests_properties(matrix_utils_memcheck PROPERTIES DEPENDS MatrixUtilsTest)
    set_tests_properties(path_memcheck PROPERTIES DEPENDS PathTest)
    set_tests_properties(path_finder_utils_memcheck PROPERTIES DEPENDS PathFinderUtilsTests)
    set_tests_properties(dfs_algorithm_memcheck PROPERTIES DEPENDS DFSAlgorithmTests)
    set_tests_properties(cli_utils_memcheck PROPERTIES DEPENDS CLIUtilsTests)
endif()