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
 * @brief Tests basic DFS path finding in simple matrix
 */
void testBasicPathFinding()
{
    std::cout << "Testing basic path finding..." << std::endl;

    MatrixWorld world(3, 3);
    DFSAlgorithm dfs;
    
    // Find path of length 4
    Path result = dfs.findViablePath(world, {4});
    
    assert(!result.isEmpty());
    assert(result.getLength() == 4);
    assert(result.isContiguous());
    
    std::cout << "✓ Basic path finding test passed" << std::endl;
}

/**
 * @brief Tests path finding with blocked cells
 */
void testPathFindingWithBlockedCells()
{
    std::cout << "Testing path finding with blocked cells..." << std::endl;

    MatrixWorld world(4, 4);
    // Block some cells to create constraints
    world.setCell(1, 1, true);
    world.setCell(1, 2, true);
    
    DFSAlgorithm dfs;
    Path result = dfs.findViablePath(world, {6});
    
    assert(!result.isEmpty());
    assert(result.getLength() == 6);
    assert(result.isContiguous());
    
    std::cout << "✓ Path finding with blocked cells test passed" << std::endl;
}

/**
 * @brief Tests impossible path scenarios
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
    Path result = dfs.findViablePath(world, {3}); // Impossible
    
    assert(result.isEmpty());
    
    std::cout << "✓ Impossible path test passed" << std::endl;
}

/**
 * @brief Tests exception handling
 */
void testExceptionHandling()
{
    std::cout << "Testing exception handling..." << std::endl;

    MatrixWorld world(3, 3);
    DFSAlgorithm dfs;
    
    // Test zero path length
    bool exceptionThrown = false;
    try {
        UNUSED(dfs.findViablePath(world, {0}));
    } catch (const std::invalid_argument &e) {
        exceptionThrown = true;
    }
    assert(exceptionThrown);
    
    // Test path length exceeding matrix size
    exceptionThrown = false;
    try {
        UNUSED(dfs.findViablePath(world, {20}));
    } catch (const std::invalid_argument &e) {
        exceptionThrown = true;
    }
    assert(exceptionThrown);
    
    std::cout << "✓ Exception handling test passed" << std::endl;
}

/**
 * @brief Main test runner for DFS algorithm
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
