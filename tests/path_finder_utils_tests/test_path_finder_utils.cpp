/**
 * @file test_path_finder_utils.cpp
 * @brief Unit tests for PathFinderUtils class functionality
 * @author Slepotek
 * @date September 2025
 * @version 1.0
 * 
 * Comprehensive test suite validating PathFinderUtils class operations including:
 * - Starting point candidate selection and scoring
 * - Priority queue state management and exhaustion handling
 * - Exception handling for invalid inputs and edge cases
 * - Multi-call scenarios with stateful priority queue
 */

#include "../test_main.hpp"
#include "path_finder_utils.hpp"
#include <cassert>
#include <iostream>
#include <vector>

/**
 * @brief Tests basic starting point candidate selection
 * 
 * Validates core functionality:
 * - Candidate selection from a simple 3x3 matrix
 * - Proper scoring based on unblocked neighbor counts
 * - Correct return of requested number of candidates
 * - Priority ordering (best candidates first)
 * 
 * @note Uses a 3x3 matrix with center cell having maximum neighbors
 */
void testBasicCandidateSelection()
{
    std::cout << "Testing basic candidate selection..." << std::endl;

    // Create 3x3 matrix (all unblocked initially)
    MatrixWorld world(3, 3);
    PathFinderUtils pathFinder;

    // Request top 3 candidates
    auto candidates = pathFinder.findStartingPointCandidates(world, 3);

    // Verify we got exactly 3 candidates
    assert(candidates.size() == 3);

    // Center cell (1,1) should be first (has 4 neighbors)
    assert(candidates[0].first == 1 && candidates[0].second == 1);

    std::cout << "✓ Basic candidate selection test passed" << std::endl;
}

/**
 * @brief Tests candidate selection with blocked cells
 * 
 * Validates behavior with mixed cell states:
 * - Only unblocked cells are considered as candidates
 * - Scoring reflects actual unblocked neighbor counts
 * - Blocked cells are properly excluded from results
 * 
 * @note Creates a pattern where some cells have reduced connectivity
 */
void testCandidateSelectionWithBlockedCells()
{
    std::cout << "Testing candidate selection with blocked cells..." << std::endl;

    // Create 3x3 matrix and block some cells
    MatrixWorld world(3, 3);
    world.setCell(0, 0, true); // Block corner
    world.setCell(2, 2, true); // Block opposite corner

    PathFinderUtils pathFinder;
    auto candidates = pathFinder.findStartingPointCandidates(world, 7);

    // Should get 7 candidates (9 total - 2 blocked)
    assert(candidates.size() == 7);

    // Verify blocked cells are not in results
    for (const auto &candidate : candidates)
    {
        assert(candidate.first != 0 || candidate.second != 0);
        assert(candidate.first != 2 || candidate.second != 2);
    }

    std::cout << "✓ Candidate selection with blocked cells test passed" << std::endl;
}

/**
 * @brief Tests multi-call stateful behavior
 * 
 * Validates priority queue state management:
 * - First call populates the queue
 * - Subsequent calls consume from existing queue
 * - Candidates are returned in priority order across calls
 * - No duplicate candidates across multiple calls
 * 
 * @note Tests the core stateful design of the class
 */
void testMultiCallStatefulBehavior()
{
    std::cout << "Testing multi-call stateful behavior..." << std::endl;

    MatrixWorld world(3, 3);
    PathFinderUtils pathFinder;

    // First call - get top 2 candidates
    auto firstBatch = pathFinder.findStartingPointCandidates(world, 2);
    assert(firstBatch.size() == 2);

    // Second call - get next 3 candidates
    auto secondBatch = pathFinder.findStartingPointCandidates(world, 3);
    assert(secondBatch.size() == 3);

    // Verify no duplicates between batches
    for (const auto &first : firstBatch)
    {
        for (const auto &second : secondBatch)
        {
            assert(first.first != second.first || first.second != second.second);
        }
    }

    std::cout << "✓ Multi-call stateful behavior test passed" << std::endl;
}

/**
 * @brief Tests queue exhaustion handling
 * 
 * Validates exhaustion scenarios:
 * - Requesting more candidates than available returns all remaining
 * - Queue exhaustion is properly tracked
 * - Subsequent calls after exhaustion throw appropriate exception
 * 
 * @note Uses small matrix to easily trigger exhaustion
 */
void testQueueExhaustion()
{
    std::cout << "Testing queue exhaustion..." << std::endl;

    // Create small 2x2 matrix (4 total cells)
    MatrixWorld world(2, 2);
    PathFinderUtils pathFinder;

    // Get first 2 candidates
    auto firstBatch = pathFinder.findStartingPointCandidates(world, 2);
    assert(firstBatch.size() == 2);

    // Request more than remaining (should get all remaining)
    auto secondBatch = pathFinder.findStartingPointCandidates(world, 4);
    assert(secondBatch.size() == 2); // Only 2 remaining

    // Next call should throw exhaustion exception
    bool exceptionThrown = false;
    try
    {
        UNUSED(pathFinder.findStartingPointCandidates(world, 1));
    }
    catch (const std::runtime_error &e)
    {
        exceptionThrown = true;
    }
    assert(exceptionThrown);

    std::cout << "✓ Queue exhaustion test passed" << std::endl;
}

/**
 * @brief Tests exception handling for invalid inputs
 * 
 * Validates error conditions:
 * - Zero candidates requested throws invalid_argument
 * - Empty matrix throws invalid_argument
 * - Candidates exceeding matrix size throws length_error
 * 
 * @note Ensures robust error handling for edge cases
 */
void testExceptionHandling()
{
    std::cout << "Testing exception handling..." << std::endl;

    MatrixWorld world(3, 3);
    PathFinderUtils pathFinder;

    // Test zero candidates
    bool exceptionThrown = false;
    try
    {
        UNUSED(pathFinder.findStartingPointCandidates(world, 0));
    }
    catch (const std::invalid_argument &e)
    {
        exceptionThrown = true;
    }
    assert(exceptionThrown);

    // Test empty matrix
    MatrixWorld emptyWorld(2, 2);
    emptyWorld.setCell(0, 0, true);
    emptyWorld.setCell(0, 1, true);
    emptyWorld.setCell(1, 0, true);
    emptyWorld.setCell(1, 1, true);

    PathFinderUtils pathFinder2;
    exceptionThrown = false;
    try
    {
        UNUSED(pathFinder2.findStartingPointCandidates(emptyWorld, 1));
    }
    catch (const std::invalid_argument &e)
    {
        exceptionThrown = true;
    }
    assert(exceptionThrown);

    // Test candidates exceeding matrix size
    exceptionThrown = false;
    try
    {
        UNUSED(pathFinder.findStartingPointCandidates(world, 255)); // Exceeds getTotalCells()
    }
    catch (const std::length_error &e)
    {
        exceptionThrown = true;
    }
    assert(exceptionThrown);

    std::cout << "✓ Exception handling test passed" << std::endl;
}

/**
 * @brief Tests scoring algorithm accuracy
 * 
 * Validates neighbor counting and scoring:
 * - Corner cells have fewer neighbors than center cells
 * - Edge cells have intermediate neighbor counts
 * - Blocked neighbors are not counted in scores
 * 
 * @note Creates specific patterns to verify scoring logic
 */
void testScoringAlgorithm()
{
    std::cout << "Testing scoring algorithm..." << std::endl;

    // Create 4x4 matrix with specific pattern
    MatrixWorld world(4, 4);

    // Block some cells to create known neighbor patterns
    world.setCell(1, 0, true); // Block to affect (1,1) score
    world.setCell(0, 1, true); // Block to affect (1,1) score

    PathFinderUtils pathFinder;
    auto candidates = pathFinder.findStartingPointCandidates(world, 16);

    // Cell (1,1) should not be first anymore due to blocked neighbors
    // Cell (2,2) should have higher score (4 unblocked neighbors)
    bool found_2_2_before_1_1 = false;
    for (size_t i = 0; i < candidates.size(); i++)
    {
        if (candidates[i].first == 2 && candidates[i].second == 2)
        {
            // Found (2,2), now check if (1,1) comes later
            for (size_t j = i + 1; j < candidates.size(); j++)
            {
                if (candidates[j].first == 1 && candidates[j].second == 1)
                {
                    found_2_2_before_1_1 = true;
                    break;
                }
            }
            break;
        }
    }
    assert(found_2_2_before_1_1);

    std::cout << "✓ Scoring algorithm test passed" << std::endl;
}

/**
 * @brief Main test runner for PathFinderUtils
 * 
 * Executes all test functions and reports results.
 * Tests are run in logical order from basic to complex scenarios.
 */
int main()
{
    std::cout << "=== PathFinderUtils Test Suite ===" << std::endl;

    try
    {
        testBasicCandidateSelection();
        testCandidateSelectionWithBlockedCells();
        testMultiCallStatefulBehavior();
        testQueueExhaustion();
        testExceptionHandling();
        testScoringAlgorithm();

        std::cout << "\n✅ All PathFinderUtils tests passed successfully!" << std::endl;
        return 0;
    }
    catch (const std::exception &e)
    {
        std::cout << "\n❌ Test failed with exception: " << e.what() << std::endl;
        return 1;
    }
    catch (...)
    {
        std::cout << "\n❌ Test failed with unknown exception" << std::endl;
        return 1;
    }
}
