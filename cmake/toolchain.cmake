# Toolchain file for GCC
set(CMAKE_SYSTEM_NAME Linux)

# Specify the cross compiler
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

# Where is the target environment
set(CMAKE_FIND_ROOT_PATH /usr)

# Search for programs in the build host directories
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# For libraries and headers in the target directories
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Compiler flags
set(CMAKE_C_FLAGS_INIT "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_INIT "-Wall -Wextra")