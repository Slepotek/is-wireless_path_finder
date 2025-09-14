# Library sources
set(LIB_SOURCES
     src/matrix_utils.cpp
     src/path.cpp
     src/performance_measure.cpp
     src/path_finder_utils.cpp
     src/dfs_algorithm.cpp
     src/cli_utils.cpp
     src/performance_guard.cpp)

set(LIB_HEADERS
     include/matrix_utils.hpp
     include/path.hpp
     include/performance_measure.hpp
     include/path_finder_utils.hpp
     include/dfs_algorithm.hpp
     include/cli_utils.hpp
     include/Ipath_algorithm.hpp
     include/performance_guard.hpp)

# Create static library
add_library(pathFinder_lib STATIC ${LIB_SOURCES} ${LIB_HEADERS})
target_include_directories(pathFinder_lib PUBLIC include)

# Set library properties
set_target_properties(pathFinder_lib PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON)