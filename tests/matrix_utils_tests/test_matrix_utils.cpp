/**
 * @file test_matrix_utils.cpp
 * @brief Unit tests for MatrixWorld class functionality
 * @author Slepotek
 * @date September 2025
 * @version 1.0
 * 
 * Comprehensive test suite validating MatrixWorld class operations including:
 * - Matrix construction and initialization
 * - Exception handling for invalid inputs
 * - Cell state management and operations
 * - Neighbor counting algorithms
 */

#include <cassert>
#include <iostream>
#include "matrix_utils.hpp"

#define UNUSED(x) ((void)(x))

/**
 * @brief Tests basic matrix construction and property validation
 * 
 * Validates core matrix functionality:
 * - Matrix creation with valid dimensions (3x4)
 * - Correct size reporting (getRowSize returns cols, getColSize returns rows)
 * - Initial state verification (all cells unblocked, matrixIsEmpty returns true)
 * - Cell counter accuracy (12 unblocked, 0 blocked cells)
 * 
 * @note Tests the fundamental matrix state after construction
 */
void test_matrix_creation() {
    std::cout << "Running test_matrix_creation...\n";
    
    MatrixWorld matrix(3, 4);
    assert(matrix.getRowSize() == 4);  // cols = row width
    assert(matrix.getColSize() == 3);  // rows = col height
    assert(matrix.getNoOfUnblockedCells() == 12);
    assert(matrix.getNoOfBlockedCells() == 0);
    assert(matrix.matrixIsEmpty() == true);
    
    std::cout << "✓ test_matrix_creation passed\n";
}

/**
 * @brief Tests constructor exception handling for invalid inputs
 * 
 * Validates that MatrixWorld constructor properly throws exceptions for:
 * - Zero row dimension (should throw std::invalid_argument)
 * - Verification that exception message is meaningful
 * - Ensures no matrix object is created on invalid input
 * 
 * @note Uses specific exception type catching to verify correct error handling
 */
void test_matrix_exceptions() {
    std::cout << "Running test_matrix_exceptions...\n";

    try {
        MatrixWorld matrix(0, 5);  // Invalid: zero rows
        assert(false); // Should not reach here - constructor should throw
    } catch (const std::invalid_argument& e) {
        std::cout << "Caught expected exception: " << e.what() << "\n";
        // Expected behavior - constructor threw appropriate exception
    }

    std::cout << "✓ test_matrix_exceptions passed\n";
}

/**
 * @brief Tests individual cell operations and state management
 * 
 * Validates cell manipulation functionality:
 * - Setting individual cell states (blocking cells)
 * - Cell state querying (isUnblocked method)
 * - Counter accuracy after state changes
 * - Matrix empty status updates after blocking cells
 * - Proper counter management (blocked/unblocked cell counts)
 * 
 * @note Tests the core cell state management that path finding relies on
 */
void test_cell_operations() {
    std::cout << "Running test_cell_operations...\n";
    
    MatrixWorld matrix(3, 3);
    
    // Initial state verification
    assert(matrix.getNoOfUnblockedCells() == 9);
    assert(matrix.getNoOfBlockedCells() == 0);
    assert(matrix.matrixIsEmpty() == true);
    
    // Block center cell and verify counters update
    assert(matrix.setCell(1, 1, true) == true);   // Block center cell
    assert(matrix.isUnblocked(1, 1) == false);    // Verify it's blocked
    assert(matrix.getNoOfUnblockedCells() == 8);  // Should decrease by 1
    assert(matrix.getNoOfBlockedCells() == 1);    // Should increase by 1
    assert(matrix.matrixIsEmpty() == false);      // Matrix no longer empty
    
    // Unblock the cell and verify counters revert
    assert(matrix.setCell(1, 1, false) == true);  // Unblock center cell
    assert(matrix.isUnblocked(1, 1) == true);     // Verify it's unblocked
    assert(matrix.getNoOfUnblockedCells() == 9);  // Should return to 9
    assert(matrix.getNoOfBlockedCells() == 0);    // Should return to 0
    assert(matrix.matrixIsEmpty() == true);       // Matrix empty again
    
    std::cout << "✓ test_cell_operations passed\n";
}

/**
 * @brief Tests 4-directional neighbor counting algorithm
 * 
 * Validates neighbor analysis functionality critical for path finding:
 * - Center cell neighbor counting (should have 4 neighbors initially)
 * - Corner cell neighbor counting (should have 2 neighbors)
 * - Edge cell neighbor counting (should have 3 neighbors)
 * - Neighbor count updates after blocking adjacent cells
 * - Boundary condition handling (corner cells after blocking neighbors)
 * 
 * @note This algorithm is essential for starting point selection in path finding
 * @note Tests 4-directional movement only (up, down, left, right)
 */
void test_count_unblocked_neighbors() {
    std::cout << "Running test_count_unblocked_neighbors...\n";
    
    MatrixWorld matrix(3, 3);
    
    // Test center cell (1,1) - should have 4 unblocked neighbors initially
    assert(matrix.countUnblockedNeighbors(1, 1) == 4);
    
    // Test corner cell (0,0) - should have 2 unblocked neighbors
    assert(matrix.countUnblockedNeighbors(0, 0) == 2);
    
    // Test edge cell (0,1) - should have 3 unblocked neighbors
    assert(matrix.countUnblockedNeighbors(0, 1) == 3);
    
    // Block some neighbors and test again
    matrix.setCell(0, 1, true);  // Block top neighbor of (1,1)
    matrix.setCell(1, 0, true);  // Block left neighbor of (1,1)
    
    // Now center cell (1,1) should have only 2 unblocked neighbors
    assert(matrix.countUnblockedNeighbors(1, 1) == 2);
    
    // Corner (0,0) should now have 0 unblocked neighbors (both adjacent cells blocked)
    assert(matrix.countUnblockedNeighbors(0, 0) == 0);
    
    std::cout << "✓ test_count_unblocked_neighbors passed\n";
}

/**
 * @brief Tests comprehensive error handling and edge cases
 * 
 * Validates error handling for various failure scenarios:
 * - Constructor exceptions for invalid dimensions
 * - Out-of-bounds access attempts
 * - Operations on empty matrices
 * - Division by zero in ratio calculations
 * - Boundary condition testing
 * 
 * @note Tests both exception throwing and bool return error handling patterns
 */
void test_error_handling() {
    std::cout << "Running test_error_handling...\n";
    
    // Test constructor exceptions
    try {
        MatrixWorld matrix(0, 0);  // Both dimensions zero
        assert(false); // Should not reach here
    } catch (const std::invalid_argument& e) {
        std::cout << "Caught expected exception for (0,0): " << e.what() << "\n";
    }
    
    try {
        MatrixWorld matrix(5, 0);  // Zero columns
        assert(false); // Should not reach here
    } catch (const std::invalid_argument& e) {
        std::cout << "Caught expected exception for zero cols: " << e.what() << "\n";
    }
    
    // Test out-of-bounds operations
    MatrixWorld matrix(2, 2);
    
    // setCell out-of-bounds should return false
    assert(matrix.setCell(5, 5, true) == false);
    assert(matrix.setCell(2, 1, true) == false);  // Row out of bounds
    assert(matrix.setCell(1, 2, true) == false);  // Col out of bounds
    
    // isUnblocked out-of-bounds should throw
    try {
        UNUSED(matrix.isUnblocked(5, 5));
        assert(false); // Should throw
    } catch (const std::invalid_argument& e) {
        std::cout << "Caught expected out-of-bounds exception: " << e.what() << "\n";
    }
    
    // Test division by zero in ratio calculation
    try {
        double ratio = matrix.getBlockedToUnblockedRatio(); // 0 blocked cells
        UNUSED(ratio);
        assert(false); // Should throw
    } catch (const std::invalid_argument& e) {
        std::cout << "Caught expected division by zero: " << e.what() << "\n";
    }
    
    // Block all cells and test ratio with 0 unblocked
    matrix.setCell(0, 0, true);
    matrix.setCell(0, 1, true);
    matrix.setCell(1, 0, true);
    matrix.setCell(1, 1, true);
    
    try {
        double ratio = matrix.getBlockedToUnblockedRatio(); // 0 unblocked cells
        UNUSED(ratio);
        assert(false); // Should throw
    } catch (const std::invalid_argument& e) {
        std::cout << "Caught expected division by zero (no unblocked): " << e.what() << "\n";
    }
    
    // Test neighbor counting with invalid coordinates
    assert(matrix.countUnblockedNeighbors(10, 10) == 0); // Should return 0 for invalid coords
    
    std::cout << "✓ test_error_handling passed\n";
}

/**
 * @brief Main test runner - executes all MatrixWorld test cases
 * 
 * Orchestrates the complete test suite execution:
 * - Runs all individual test functions in logical order
 * - Provides clear test progress output
 * - Reports overall test suite success
 * 
 * @return 0 on success (all tests passed)
 * @note Program terminates with assertion failure if any test fails
 */
int main() {
    std::cout << "Starting MatrixUtils tests...\n";
    
    test_matrix_creation();
    test_matrix_exceptions();
    test_cell_operations();
    test_count_unblocked_neighbors();
    test_error_handling();
    
    std::cout << "All MatrixUtils tests passed!\n";
    return 0;
}
