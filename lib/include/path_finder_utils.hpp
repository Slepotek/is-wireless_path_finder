/**
 * @file path_finder_utils.hpp
 * @brief Path finding utilities and algorithms
 * @author Slepotek
 * @date September 2025
 * @version 1.0
 */

#ifndef PATH_FINDER_UTILS_H
#define PATH_FINDER_UTILS_H

#include "matrix_utils.hpp"
#include <cstdint>
#include <queue>
#include <utility>
#include <vector>

/**
 * @class PathFinderUtils
 * @brief Utilities for path finding algorithms with smart starting point selection
 * 
 * This class provides functionality for selecting optimal starting points for DFS
 * path finding algorithms. It uses a stateful priority queue approach to efficiently
 * identify and return the best candidates based on unblocked neighbor counts.
 * 
 * The class maintains an internal priority queue that is populated once and then
 * consumed in batches, allowing for efficient multi-call scenarios where different
 * sets of starting points need to be tried sequentially.
 * 
 * @note The priority queue is automatically populated on first call and tracks
 * exhaustion state to prevent unnecessary operations.
 */
class PathFinderUtils
{
private:
    std::priority_queue<std::pair<uint32_t, std::pair<std::uint16_t, uint16_t>>>
        priorityQueue;        ///< Priority queue storing (score, coordinates) pairs
    bool isExhausted = false; ///< Flag indicating if all candidates have been consumed

public:
    /**
     * @brief Constructs a new PathFinderUtils instance
     * 
     * Initializes an empty priority queue and sets exhaustion flag to false.
     * The priority queue will be populated on the first call to findStartingPointCandidates.
     */
    PathFinderUtils();

    /**
     * @brief Finds the best starting point candidates for path finding
     * @param matrixWorld Reference to the MatrixWorld to analyze
     * @param numberOfCandidates Number of candidates to return (1-255)
     * @return Vector of (row, col) coordinates sorted by score (best first)
     * @throws std::invalid_argument If numberOfCandidates is zero or matrix is empty
     * @throws std::length_error If numberOfCandidates exceeds matrixWorld.getTotalCells()
     * @throws std::runtime_error If all candidates have been exhausted
     * 
     * On first call, populates the internal priority queue by scoring all unblocked
     * cells based on their unblocked neighbor count. Subsequent calls consume from
     * the existing queue until exhausted.
     * 
     * If fewer candidates are available than requested, returns all remaining
     * candidates and marks the queue as exhausted.
     * 
     * Scoring algorithm:
     * - Each unblocked cell is scored by counting its unblocked neighbors (0-4)
     * - Higher scores indicate better starting points for path finding
     * - Only 4-directional neighbors are considered (up, down, left, right)
     */
    [[nodiscard]] std::vector<std::pair<std::uint16_t, uint16_t>> findStartingPointCandidates(
        const MatrixWorld &matrixWorld,
        uint8_t numberOfCandidates);

    /**
     * @brief Checks if all starting point candidates have been exhausted
     * @return true if no more candidates are available, false otherwise
     * 
     * This method allows callers to determine when the priority queue has been
     * completely consumed. Useful for implementing retry loops that continue
     * until all possible starting points have been attempted.
     * 
     * @note Returns false initially and after queue population, true only after
     * all candidates have been retrieved via findStartingPointCandidates()
     */
    [[nodiscard]] bool getIsExhausted() const
    {
        return isExhausted;
    }
};
#endif