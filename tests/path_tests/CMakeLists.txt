# Path test executable
add_executable(test_path test_path.cpp)
target_link_libraries(test_path pathFinder_lib)

# Register with CTest
add_test(NAME PathTest COMMAND test_path)

# Set properties
set_target_properties(test_path PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON)