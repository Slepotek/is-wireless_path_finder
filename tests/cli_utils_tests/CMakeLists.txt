# CLI Utils Tests
add_executable(test_cli_utils test_cli_utils.cpp)
target_link_libraries(test_cli_utils pathFinder_lib)

# Register CTest
add_test(NAME CLIUtilsTests COMMAND test_cli_utils)

# Set properties
set_target_properties(test_cli_utils PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON)
