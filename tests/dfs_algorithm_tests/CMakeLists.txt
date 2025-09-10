# DFS Algorithm Tests
add_executable(test_dfs_algorithm test_dfs_algorithm.cpp)
target_link_libraries(test_dfs_algorithm pathFinder_lib)

# Add test to CTest
add_test(NAME DFSAlgorithmTests COMMAND test_dfs_algorithm)

# Memory leak detection with Valgrind
find_program(VALGRIND_PROGRAM valgrind)
if(VALGRIND_PROGRAM)
    add_test(NAME DFSAlgorithmMemoryCheck 
             COMMAND ${VALGRIND_PROGRAM} --leak-check=full --error-exitcode=1 
             $<TARGET_FILE:test_dfs_algorithm>)
endif()
