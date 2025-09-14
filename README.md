# is-wireless - Adaptive Path Finding in NxM Matrix

**Version:** 1.0  
**Author:** Slepotek  
**Date:** September 2025

## 📋 Overview

**is-wireless** is a high-performance path finding application that discovers contiguous paths in NxM matrices using advanced algorithms. The system finds optimal routes through unblocked cells while avoiding obstacles, with applications in robotics, game development, and network routing.

### Key Features
- **Smart DFS Algorithm** with priority-based starting point selection
- **Polymorphic Architecture** - easily extensible for new algorithms (BFS, A*, etc.)
- **Type-Safe CLI Interface** with comprehensive parameter validation
- **Professional Testing Suite** with 100% coverage and memory leak detection
- **Robust Error Handling** with meaningful user feedback
- **Performance measurement** using wall time and cycle count from perf_event

## 🎯 What It Does

The application solves the **Adaptive Path Finding Problem**:
- Finds contiguous paths of specified length in NxM matrices
- Navigates around blocked cells (obstacles)
- Uses 4-directional movement (up, down, left, right)
- Optimizes starting point selection based on neighbor connectivity
- Maintains **O((N×M)³)** complexity constraint

## 🏗️ Architecture

### Core Components
- **MatrixWorld** - 2D matrix representation with efficient cell operations
- **Path** - Contiguous coordinate sequence with stack-like operations for DFS
- **PathFinderUtils** - Smart starting point selection with priority queue
- **DFSAlgorithm** - Depth-first search with backtracking implementation
- **CLI Interface** - Professional command-line argument parsing

### Design Patterns
- **Polymorphic Algorithms** - `PathAlgorithm` base class for extensibility
- **Type-Safe Parameters** - `PathLength` and `MaxStartingPoints` structs
- **RAII Memory Management** - Automatic cleanup, no manual memory handling
- **Exception-Driven Error Handling** - Clear error propagation

## 🚀 Quick Start

### Prerequisites
- **Compatible compiler** (GCC 11+, Clang 12+)
- **CMake 3.10+**
- **Make** build system
- **Valgrind** (optional, for memory leak detection)

### Build Commands

```bash
# Clean previous builds
make clean

# Build debug version (with symbols and compile_commands.json)
make build_debug

# Build optimized release version
make build_release

# Run the main application
make run_proj

# Run complete test suite
make test_proj
```

### Basic Usage

```bash
# Simple 5x5 matrix with path length 6
./pathFinder --rows 5 --cols 5 --pathLength 6

# With blocked cells and custom starting points
./pathFinder --rows 8 --cols 8 --pathLength 12 --maxStartingPoints 10 --blockedCells "{1,0}" "{2,1}" "{3,2}"

# With blocked cells, custom starting points and performance measurement
sudo ./pathFinder --rows 8 --cols 8 --pathLength 12 --maxStartingPoints 10 --blockedCells "{1,0}" "{2,1}" "{3,2}" --enableMeasurement

# Show help
./pathFinder --help
```

## 📖 Command Line Interface

### Required Parameters
- `--rows R` - Number of matrix rows (e.g., `--rows 5`)
- `--cols C` - Number of matrix columns (e.g., `--cols 5`) 
- `--pathLength N` - Target path length (e.g., `--pathLength 12`)

### Optional Parameters
- `--maxStartingPoints N` - Maximum starting points to try (default: 5)
- `--blockedCells COORDS` - Blocked cell coordinates (e.g., `--blockedCells "{1,0}" "{2,1}"`)
- `--help, -h` - Show detailed help message

## 🧪 Testing

### Test Suite Overview
- **comprehensive tests** covering all components
- **Memory leak detection** using Valgrind integration
- **Exception handling validation** for all error conditions
- **Edge case coverage** including underflow prevention
- **Performance validation** for O(1) operations

### Running Tests

```bash
# Run all tests
make test_proj

# Individual test components
cd build
./test_matrix_utils      # Matrix operations
./test_path             # Path contiguity and operations  
./test_path_finder_utils # Starting point selection
./test_dfs_algorithm    # DFS pathfinding algorithm
./test_cli_utils        # Command line parsing
```

```bash
# Run black box tests
cd tools
. pythonTests.sh

# The python virtual environment will be scaffolded
# User will be prompted for the scope of the tests
# (small (100x100 matrix), medium (200x200 matrix), large (500x500 matrix))
```

## 📁 Project Structure

```
is-wireless/
├── lib/                    # Core library
│   ├── include/           # Public headers
│   │   ├── matrix_utils.hpp
│   │   ├── path.hpp
│   │   ├── path_finder_utils.hpp
│   │   ├── dfs_algorithm.hpp
│   │   ├── cli_utils.hpp
│   │   ├── Ipath_algorithm.hpp
│   │   ├── performance_guard.hpp
│   │   └── performance_measure.hpp
|   |
│   └── src/              # Implementation files
│       ├── matrix_utils.cpp
│       ├── path.cpp
│       ├── path_finder_utils.cpp
│       ├── dfs_algorithm.cpp
│       ├── performance_guard.cpp
│       ├── performance_measure.cpp
│       └── cli_utils.cpp
├── tests/                 # Comprehensive test suite
│   ├── matrix_utils_tests/
│   ├── path_tests/
│   ├── path_finder_utils_tests/
│   ├── dfs_algorithm_tests/
│   ├── cli_utils_tests/
│   └── test_main.hpp     # Shared test utilities
├── src/                  # Main application
│   └── main.cpp
├── CMakeLists.txt        # Root build configuration
├── Makefile             # Convenience build targets
└── README.md            # This file
```

## 🔧 Development

### Build System
- **CMake** - Cross-platform build configuration
- **CTest** - Automated test discovery and execution
- **Valgrind Integration** - Memory leak detection per module
- **clangd Support** - LSP integration with compile_commands.json

### Code Quality
- **C++20 Standard** - Modern C++ features and safety
- **Doxygen Documentation** - Complete API documentation
- **Exception Safety** - RAII patterns and proper error handling
- **Memory Safety** - Zero memory leaks detected
- **Type Safety** - Strong typing prevents parameter confusion

### Adding New Algorithms
The polymorphic design makes it easy to add new pathfinding algorithms:

```cpp
class BFSAlgorithm : public PathAlgorithm {
public:
    Path findViablePath(const MatrixWorld& world, 
                       PathLength pathLength, 
                       MaxStartingPoints maxStartingPoints) override {
        // Implement BFS logic here
    }
};
```

## 📊 Performance

- **Time Complexity:** O(4^L × S) where L is path length, S is starting points
- **Space Complexity:** O(N×M) for matrix representation
- **Memory Efficient:** Uses `std::vector<bool>` for matrix storage
- **Optimized Operations:** O(1) path operations using `std::deque`

## 🎯 Future Enhancements

- **Additional Algorithms** - A* implementations
- **Multithreading** - Algorithm parallelization for performance gains
- **Advanced Optimization** - Hierarchical pathfinding for large matrices

---
