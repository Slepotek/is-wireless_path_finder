/**
 * @file test_dfs_algorithm.cpp
 * @brief Unit tests for DFS algorithm functionality
 * @author Slepotek
 * @date September 2025
 * @version 1.0
 */

#include "../test_main.hpp"
#include "dfs_algorithm.hpp"
#include <cassert>
#include <iostream>

/**
 * @brief Tests basic DFS path finding in simple unblocked matrix
 * 
 * Creates a 3x3 matrix with all cells unblocked and attempts to find
 * a path of length 4. Validates that the algorithm can find a valid
 * contiguous path in an optimal scenario with no obstacles.
 * 
 * Test conditions:
 * - 3x3 matrix (9 total cells)
 * - All cells unblocked
 * - Target path length: 4
 * - Uses default maxStartingPoints (5)
 * 
 * Expected results:
 * - Path found (not empty)
 * - Path length exactly 4
 * - Path is contiguous (4-directional adjacency)
 */
void testBasicPathFinding()
{
    std::cout << "Testing basic path finding..." << std::endl;

    MatrixWorld world(3, 3);
    DFSAlgorithm dfs;
    
    // Find path of length 4 with default maxStartingPoints
    Path result = dfs.findViablePath(world, {4}, {5}); // Fixed: Added missing MaxStartingPoints parameter
    
    assert(!result.isEmpty());
    assert(result.getLength() == 4);
    assert(result.isContiguous());
    
    std::cout << "✓ Basic path finding test passed" << std::endl;
}

/**
 * @brief Tests path finding with blocked cells creating constraints
 * 
 * Creates a 4x4 matrix with strategic blocked cells to test the algorithm's
 * ability to navigate around obstacles. Blocks cells (1,1) and (1,2) to
 * create a barrier that forces path finding around obstacles.
 * 
 * Test conditions:
 * - 4x4 matrix (16 total cells)
 * - 2 cells blocked: (1,1) and (1,2)
 * - Target path length: 6
 * - Uses default maxStartingPoints (5)
 * 
 * Expected results:
 * - Path found despite obstacles
 * - Path length exactly 6
 * - Path is contiguous and avoids blocked cells
 */
void testPathFindingWithBlockedCells()
{
    std::cout << "Testing path finding with blocked cells..." << std::endl;

    MatrixWorld world(4, 4);
    // Block some cells to create constraints
    world.setCell(1, 1, true);
    world.setCell(1, 2, true);
    
    DFSAlgorithm dfs;
    Path result = dfs.findViablePath(world, {6}, {5}); // Fixed: Added missing MaxStartingPoints parameter
    
    assert(!result.isEmpty());
    assert(result.getLength() == 6);
    assert(result.isContiguous());
    
    std::cout << "✓ Path finding with blocked cells test passed" << std::endl;
}

/**
 * @brief Tests impossible path scenarios with heavily constrained matrix
 * 
 * Creates a scenario where the requested path length is impossible to achieve
 * due to matrix constraints. Blocks all cells except the center cell (1,1)
 * in a 3x3 matrix, leaving only 1 unblocked cell but requesting path length 3.
 * 
 * Test conditions:
 * - 3x3 matrix (9 total cells)
 * - 8 cells blocked (all except center)
 * - Only cell (1,1) unblocked
 * - Target path length: 3 (impossible with only 1 unblocked cell)
 * 
 * Expected results:
 * - Empty path returned (algorithm recognizes impossibility)
 * - No exceptions thrown (graceful handling)
 */
void testImpossiblePath()
{
    std::cout << "Testing impossible path scenarios..." << std::endl;

    // Create heavily blocked matrix
    MatrixWorld world(3, 3);
    for (uint16_t i = 0; i < 3; i++) {
        for (uint16_t j = 0; j < 3; j++) {
            if (i != 1 || j != 1) { // Leave only center cell
                world.setCell(i, j, true);
            }
        }
    }
    
    DFSAlgorithm dfs;
    Path result = dfs.findViablePath(world, {3}, {5}); // Fixed: Added missing MaxStartingPoints parameter
    
    assert(result.isEmpty());
    
    std::cout << "✓ Impossible path test passed" << std::endl;
}

/**
 * @brief Tests exception handling for invalid input parameters
 * 
 * Validates that the DFS algorithm properly throws exceptions for invalid
 * input conditions. Tests both zero path length and path length exceeding
 * matrix capacity to ensure robust error handling.
 * 
 * Test cases:
 * 1. Zero path length - should throw std::invalid_argument
 * 2. Path length > matrix size - should throw std::invalid_argument
 * 
 * Expected results:
 * - Appropriate exceptions thrown for invalid inputs
 * - Exception messages are meaningful
 * - No crashes or undefined behavior
 */
void testExceptionHandling()
{
    std::cout << "Testing exception handling..." << std::endl;

    MatrixWorld world(3, 3);
    DFSAlgorithm dfs;
    
    // Test zero path length
    bool exceptionThrown = false;
    try {
        UNUSED(dfs.findViablePath(world, {0}, {5})); // Fixed: Added missing MaxStartingPoints parameter
    } catch (const std::invalid_argument &e) {
        exceptionThrown = true;
    }
    assert(exceptionThrown);
    
    // Test path length exceeding matrix size
    exceptionThrown = false;
    try {
        UNUSED(dfs.findViablePath(world, {20}, {5})); // Fixed: Added missing MaxStartingPoints parameter
    } catch (const std::invalid_argument &e) {
        exceptionThrown = true;
    }
    assert(exceptionThrown);
    
    std::cout << "✓ Exception handling test passed" << std::endl;
}

/**
 * @brief Main test runner for DFS algorithm test suite
 * 
 * Executes all DFS algorithm test functions in sequence with comprehensive
 * error handling. Tests cover basic functionality, obstacle navigation,
 * impossible scenarios, and exception handling. Uses try-catch blocks
 * to provide meaningful error reporting.
 * 
 * Test sequence:
 * 1. Basic path finding in unblocked matrix
 * 2. Path finding with blocked cells (obstacle avoidance)
 * 3. Impossible path scenarios (graceful failure)
 * 4. Exception handling (input validation)
 * 
 * @return 0 on success (all tests passed), 1 on failure
 */
int main()
{
    std::cout << "=== DFS Algorithm Test Suite ===" << std::endl;

    try {
        testBasicPathFinding();
        testPathFindingWithBlockedCells();
        testImpossiblePath();
        testExceptionHandling();

        std::cout << "\n✅ All DFS Algorithm tests passed successfully!" << std::endl;
        return 0;
    } catch (const std::exception &e) {
        std::cout << "\n❌ Test failed with exception: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cout << "\n❌ Test failed with unknown exception" << std::endl;
        return 1;
    }
}
