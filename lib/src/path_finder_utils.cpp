/**
 * @file path_finder_utils.cpp
 * @brief Implementation of path finding utilities and algorithms
 * @author Slepotek
 * @date September 2025
 * @version 1.0
 */

#include "path_finder_utils.hpp"
#include <stdexcept>
#include <vector>

/**
 * @brief Default constructor initializes empty priority queue and exhaustion state
 * 
 * Initializes PathFinderUtils with empty priority queue and sets isExhausted to false.
 * Priority queue will be lazily populated on first call to findStartingPointCandidates().
 * This approach avoids unnecessary computation if the object is created but never used.
 */
// Priority queue is automatically initialized as empty by default constructor
// isExhausted flag is initialized to false by member initializer
PathFinderUtils::PathFinderUtils() = default;

/**
 * @brief Finds and returns prioritized starting point candidates for path finding
 * @param matrixWorld Reference to the matrix world to analyze
 * @param numberOfCandidates Number of candidates to return (1-255)
 * @return Vector of coordinate pairs representing best starting points
 * @throws std::invalid_argument If numberOfCandidates is zero or matrix is fully blocked
 * @throws std::length_error If numberOfCandidates exceeds total matrix cells
 * @throws std::runtime_error If all candidates have been exhausted from previous calls
 * 
 * Multi-call stateful algorithm that maintains internal priority queue:
 * 1. **Lazy Initialization**: Populates priority queue on first call only
 * 2. **Scoring System**: Ranks cells by unblocked neighbor count (0-4 neighbors)
 * 3. **Batch Processing**: Returns requested number of highest-scored candidates
 * 4. **Exhaustion Tracking**: Marks when all candidates have been consumed
 * 
 * **Stateful Behavior:**
 * - First call: Scans entire matrix and builds priority queue
 * - Subsequent calls: Returns next batch from existing queue
 * - Final call: Returns remaining candidates and sets exhaustion flag
 * 
 * **Integration Pattern:**
 * Designed for multi-call usage with DFS algorithm - call repeatedly until
 * getIsExhausted() returns true to try all possible starting points.
 * 
 * **Performance:** O(N×M) for first call, O(k) for subsequent calls where k is numberOfCandidates
 */
// Default constructor - initializes an empty priority queue and sets isExhausted to false
std::vector<std::pair<std::uint16_t, uint16_t>> PathFinderUtils::findStartingPointCandidates(
    const MatrixWorld &matrixWorld,
    uint8_t numberOfCandidates)
{
    // Input validation - ensure numberOfCandidates is valid
    if (numberOfCandidates == 0)
    {
        throw std::invalid_argument("Number of candidates must be greater than zero.");
    }

    // Validate matrix state - cannot find candidates in empty matrix
    if (matrixWorld.getTotalCells() == matrixWorld.getNoOfBlockedCells())
    {
        throw std::invalid_argument("Matrix is empty. Cannot find starting point candidates.");
    }

    // Bounds checking - cannot request more candidates than total cells
    if (numberOfCandidates > matrixWorld.getTotalCells())
    {
        throw std::length_error("Number of candidates exceeds total number of cells in the matrix.");
    }

    // Check exhaustion state - prevent operations on exhausted queue
    if (isExhausted)
    {
        throw std::runtime_error("All candidates have been exhausted.");
    }

    // Lazy initialization: populate priority queue on first call
    // This approach avoids unnecessary computation if the object is created but never used
    if (priorityQueue.empty())
    {
        // Iterate through all matrix positions to find and score unblocked cells
        for (uint16_t colIndex = 0; colIndex < matrixWorld.getRowSize(); colIndex++)
        {
            for (uint16_t rowIndex = 0; rowIndex < matrixWorld.getColSize(); rowIndex++)
            {
                // Only consider unblocked (passable) cells as potential starting points
                if (matrixWorld.isUnblocked(rowIndex, colIndex))
                {
                    // Score each cell by counting unblocked neighbors (0-4)
                    // Higher scores indicate better connectivity for path finding
                    uint32_t score = matrixWorld.countUnblockedNeighbors(rowIndex, colIndex);
                    priorityQueue.emplace(score, std::make_pair(rowIndex, colIndex));
                }
            }
        }
    }

    std::vector<std::pair<std::uint16_t, uint16_t>> candidates;

    // Branch based on availability: full request vs. partial fulfillment
    if (priorityQueue.size() > numberOfCandidates)
    {
        // Standard case: extract exactly the requested number of candidates
        for (uint8_t i = 0; i < numberOfCandidates && !priorityQueue.empty(); ++i)
        {
            candidates.push_back(priorityQueue.top().second);
            priorityQueue.pop();
        }

        // Check if queue became empty after extraction (edge case)
        if (priorityQueue.empty())
        {
            isExhausted = true;
        }
    }
    else
    {
        // Exhaustion case: return all remaining candidates
        // This handles scenarios where fewer candidates exist than requested
        while (!priorityQueue.empty())
        {
            candidates.push_back(priorityQueue.top().second);
            priorityQueue.pop();
        }

        // Mark as exhausted since all candidates have been consumed
        isExhausted = true;
    }

    return candidates;
}