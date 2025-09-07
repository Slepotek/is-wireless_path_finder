# Matrix utils test executable
add_executable(test_matrix_utils test_matrix_utils.cpp)
target_link_libraries(test_matrix_utils pathFinder_lib)

# Register with CTest
add_test(NAME MatrixUtilsTest COMMAND test_matrix_utils)

# Set properties
set_target_properties(test_matrix_utils PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON)