/**
 * @file test_path.cpp
 * @brief Unit tests for Path class functionality
 * @author Slepotek
 * @date September 2025
 * @version 1.0
 * 
 * Comprehensive test suite validating Path class operations including:
 * - Path construction and basic operations
 * - Contiguity validation for DFS pathfinding
 * - Exception handling for invalid operations
 * - Iterator functionality for GUI rendering
 */

#include <cassert>
#include <cstdint>
#include <iostream>
#include <utility>
#include <vector>
#include "path.hpp"
#include "../test_main.hpp"

/**
 * @brief Tests basic path construction and property validation
 * 
 * Validates core path functionality:
 * - Path creation (empty constructor)
 * - Initial state verification (empty path, size 0)
 * - Basic coordinate addition and retrieval
 * - Size tracking accuracy
 * 
 * @note Tests the fundamental path state after construction
 */
void test_path_creation() {
    std::cout << "Running test_path_creation...\n";
    
    Path path;
    std::pair<uint16_t, uint16_t> ref = {5, 10};
    assert(path.isEmpty() == true);
    assert(path.getLength() == 0);
    
    // Add first coordinate
    path.addCoordinate(5, 10);
    assert(path.isEmpty() == false);
    assert(path.getLength() == 1);
    assert(path.getCurrentCoordinate() == ref);

    std::cout << "✓ test_path_creation passed\n";
}

/**
 * @brief Tests exception handling for invalid operations
 * 
 * Validates that Path methods properly throw exceptions for:
 * - Getting coordinates from empty path
 * - Verification that exception messages are meaningful
 * - Ensures proper error handling for edge cases
 * 
 * @note Uses specific exception type catching to verify correct error handling
 */
void test_path_exceptions() {
    std::cout << "Running test_path_exceptions...\n";

    Path path;
    
    // Test getCurrentCoordinate on empty path
    try {
        std::pair<uint16_t, uint16_t> coord = path.getCurrentCoordinate();
        UNUSED(coord);
        assert(false); // Should not reach here
    } catch (const std::out_of_range& e) {
        std::cout << "Caught expected exception: " << e.what() << "\n";
    }
    
    // Test getNextCoordinate on empty path
    try {
        std::pair<uint16_t, uint16_t> coord = path.getNextCoordinate();
        UNUSED(coord);
        assert(false); // Should not reach here
    } catch (const std::out_of_range& e) {
        std::cout << "Caught expected exception: " << e.what() << "\n";
    }

    std::cout << "✓ test_path_exceptions passed\n";
}

/**
 * @brief Tests path operations and stack-like behavior
 * 
 * Validates path manipulation functionality:
 * - Adding multiple coordinates
 * - Stack-like pop operations (getNextCoordinate)
 * - Current coordinate tracking
 * - Path clearing functionality
 * 
 * @note Tests the core DFS backtracking operations
 */
void test_path_operations() {
    std::cout << "Running test_path_operations...\n";
    
    Path path;
    
    // Build a simple path
    path.addCoordinate(0, 0);
    path.addCoordinate(0, 1);
    path.addCoordinate(1, 1);
    std::pair<uint16_t, uint16_t> ref0 = {1, 1};
    std::pair<uint16_t, uint16_t> ref1 = {0, 1};
    
    assert(path.getLength() == 3);
    assert(path.getCurrentCoordinate() == ref0);

    // Test stack-like pop operation
    auto coord = path.getNextCoordinate();
    assert(coord == ref0);
    assert(path.getLength() == 2);
    assert(path.getCurrentCoordinate() == ref1);

    // Clear path
    path.clear();
    assert(path.isEmpty() == true);
    assert(path.getLength() == 0);
    
    std::cout << "✓ test_path_operations passed\n";
}

/**
 * @brief Tests contiguity validation for pathfinding
 * 
 * Validates path contiguity checking:
 * - Valid contiguous paths (4-directional adjacency)
 * - Invalid non-contiguous paths
 * - Edge cases (empty path, single coordinate)
 * - Complex valid paths with multiple turns
 * 
 * @note Critical for DFS pathfinding algorithm validation
 */
void test_path_contiguity() {
    std::cout << "Running test_path_contiguity...\n";
    
    Path path;
    
    // Empty path should be contiguous
    assert(path.isContiguous() == true);
    
    // Single coordinate should be contiguous
    path.addCoordinate(5, 5);
    assert(path.isContiguous() == true);
    
    // Valid horizontal adjacency
    path.addCoordinate(5, 6);
    assert(path.isContiguous() == true);
    
    // Valid vertical adjacency
    path.addCoordinate(4, 6);
    assert(path.isContiguous() == true);
    
    // Test invalid non-adjacent path
    Path invalid_path;
    invalid_path.addCoordinate(0, 0);
    invalid_path.addCoordinate(2, 2);  // Not adjacent
    assert(invalid_path.isContiguous() == false);
    
    std::cout << "✓ test_path_contiguity passed\n";
}

/**
 * @brief Tests iterator functionality for GUI rendering
 * 
 * Validates iterator operations:
 * - Begin/end iterator access
 * - Range-based for loop compatibility
 * - Iterator consistency with path contents
 * 
 * @note Essential for GUI path visualization
 */
void test_path_iterators() {
    std::cout << "Running test_path_iterators...\n";
    
    Path path;
    path.addCoordinate(1, 2);
    path.addCoordinate(3, 4);
    path.addCoordinate(5, 6);
    std::pair<uint16_t, uint16_t> ref0 = {1, 2};
    std::pair<uint16_t, uint16_t> ref1 = {3, 4};
    std::pair<uint16_t, uint16_t> ref2 = {5, 6};
    
    // Test iterator access
    auto iter = path.begin();
    assert(*iter == ref0);
    ++iter;
    assert(*iter == ref1);
    ++iter;
    assert(*iter == ref2);
    ++iter;
    assert(iter == path.end());

    // Test range-based for loop
    std::vector<std::pair<uint16_t, uint16_t>> coords;
    for (const auto& coord : path) {
        coords.push_back(coord);
    }
    
    assert(coords.size() == 3);
    assert(coords[0] == ref0);
    assert(coords[1] == ref1);
    assert(coords[2] == ref2);

    std::cout << "✓ test_path_iterators passed\n";
}

/**
 * @brief Tests contiguity validation edge cases including potential underflow
 *
 * Validates edge cases that could cause integer underflow:
 * - Adjacent cells where first coordinate is larger than second
 * - Ensures proper handling of unsigned integer subtraction
 *
 * @note Critical test for preventing underflow bugs in contiguity checking
 */
void test_path_contiguity_edge_cases() {
    std::cout << "Running test_path_contiguity_edge_cases...\n";

    Path path;

    // Test case that would cause underflow with naive subtraction
    // col1 < col2, so col1 - col2 would underflow without proper handling
    path.addCoordinate(5, 10);  // First coordinate
    path.addCoordinate(5, 9);   // Adjacent left (col1=10 > col2=9)
    assert(path.isContiguous() == true);

    path.clear();

    // Test vertical case with potential underflow
    // row1 < row2, so row1 - row2 would underflow
    path.addCoordinate(8, 5);   // First coordinate
    path.addCoordinate(7, 5);   // Adjacent up (row1=8 > row2=7)
    assert(path.isContiguous() == true);

    std::cout << "✓ test_path_contiguity_edge_cases passed\n";
}

/**
 * @brief Main test runner for Path class
 * 
 * Executes all path-related tests in sequence:
 * - Basic construction and operations
 * - Exception handling validation
 * - Stack-like operations for DFS
 * - Contiguity validation for pathfinding
 * - Iterator functionality for GUI
 * 
 * @note Follows established testing pattern from matrix_utils
 */
int main() {
    std::cout << "\n=== Running Path Tests ===\n";
    
    test_path_creation();
    test_path_exceptions();
    test_path_operations();
    test_path_contiguity();
    test_path_iterators();
    test_path_contiguity_edge_cases();
    
    std::cout << "=== All Path Tests Passed ===\n\n";
    return 0;
}
