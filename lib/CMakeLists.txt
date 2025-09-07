# Library sources
set(LIB_SOURCES
     src/matrix_utils.cpp
    #  src/performance_tracer.cpp
     src/path_finder.cpp)

set(LIB_HEADERS
     include/matrix_utils.hpp
    #  include/performance_tracer.hpp
    #  include/path.hpp
     include/path_finder.hpp)

# Create static library
add_library(pathFinder_lib STATIC ${LIB_SOURCES} ${LIB_HEADERS})
target_include_directories(pathFinder_lib PUBLIC include)

# Set library properties
set_target_properties(pathFinder_lib PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON)