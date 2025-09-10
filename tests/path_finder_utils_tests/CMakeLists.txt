# Test path_finder_utils
add_executable(test_path_finder_utils test_path_finder_utils.cpp)
target_link_libraries(test_path_finder_utils pathFinder_lib)

# Register with CTest
add_test(NAME PathFinderUtilsTest COMMAND test_path_finder_utils)

# Set properties
set_target_properties(test_path_finder_utils PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON)   