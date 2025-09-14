/**
 * @file dfs_algorithm.cpp
 * @brief Implementation of DFS path finding algorithm
 * @author Slepotek
 * @date September 2025
 * @version 1.0
 */

#include "dfs_algorithm.hpp"
#include "path_finder_utils.hpp"
#include <stdexcept>
#include <array>

/**
 * @brief Finds a viable path using DFS with smart starting point selection
 * @param matrixWorld Reference to the matrix world to search in
 * @param pathLength Target path length wrapped in type-safe structure
 * @param maxStartingPoints Maximum starting points to try per batch
 * @return Path object containing found path (empty if no solution found)
 * @throws std::invalid_argument If pathLength is zero or exceeds matrix size
 * 
 * Implementation uses multi-call stateful integration with PathFinderUtils:
 * 1. Validates input parameters for correctness
 * 2. Iteratively requests starting point candidates until exhausted
 * 3. For each candidate, attempts DFS path finding with backtracking
 * 4. Returns first successful path or empty path if no solution exists
 * 
 * Complexity: O(4^L × S) where L is path length and S is starting points tried
 */
Path DFSAlgorithm::findViablePath(const MatrixWorld &matrixWorld,
                                  PathLength pathLength,
                                  MaxStartingPoints maxStartingPoints)
{
    if (pathLength.value == 0)
    {
        throw std::invalid_argument("Path length must be greater than zero");
    }

    if (pathLength.value > matrixWorld.getTotalCells())
    {
        throw std::invalid_argument("Path length exceeds matrix size");
    }

    PathFinderUtils pathFinder;
    while (!pathFinder.getIsExhausted())
    {
        auto startingPoints = pathFinder.findStartingPointCandidates(matrixWorld, maxStartingPoints.value);
        
        // Try each starting point
        for (const auto &start : startingPoints)
        {
            std::vector<std::vector<bool>> visited(matrixWorld.getColSize(),
                                                   std::vector<bool>(matrixWorld.getRowSize(), false));
            Path currentPath;

            // Mark starting point as visited and add to path
            visited[start.first][start.second] = true;
            currentPath.addCoordinate(start.first, start.second);

            // Attempt DFS from this starting point
            if (dfsRecursive(matrixWorld, currentPath, visited, pathLength.value))
            {
                return currentPath;
            }
        }
    }
    // Return empty path if no solution found
    return {};
}

/**
 * @brief Recursive DFS implementation with backtracking for path finding
 * @param matrixWorld Reference to the matrix world for bounds and cell checking
 * @param currentPath Reference to path being built (modified during recursion)
 * @param visited Reference to visited cells matrix (modified during recursion)
 * @param targetLength Target path length to achieve
 * @return true if target length reached, false if no valid path from current state
 * 
 * Core recursive algorithm implementing depth-first search with backtracking:
 * 1. Base case: Returns true when target length is reached
 * 2. Explores all 4 directions (up, down, left, right) from current position
 * 3. For each valid unvisited neighbor:
 *    - Marks as visited and adds to path
 *    - Recursively searches from new position
 *    - Backtracks if recursive call fails (removes from path, marks unvisited)
 * 4. Returns false if no valid path found from current position
 * 
 * Uses safe integer arithmetic with bounds checking to prevent overflow.
 * Maintains path contiguity through 4-directional movement only.
 */
bool DFSAlgorithm::dfsRecursive(const MatrixWorld &matrixWorld,
                                Path &currentPath,
                                std::vector<std::vector<bool>> &visited,
                                uint16_t targetLength)
{
    // Base case: reached target length
    if (currentPath.getLength() == targetLength)
    {
        return true;
    }

    // Get current position
    auto [currentRow, currentCol] = currentPath.getCurrentCoordinate();

    // 4-directional movement: up, down, left, right
    // Keeping standard ints for arithmetic
    std::array<std::array<int, 2>, 4> directions = {{{-1, 0}, {0, 1}, {1, 0}, {0, -1}}};

    for (auto &direction : directions)
    {
        int newRow = static_cast<int>(currentRow) + direction[0];
        int newCol = static_cast<int>(currentCol) + direction[1];

        // Bounds checking
        if (newRow >= 0 && newRow < static_cast<int>(matrixWorld.getColSize()) &&
            newCol >= 0 && newCol < static_cast<int>(matrixWorld.getRowSize()) &&
            !visited[newRow][newCol] && 
            matrixWorld.isUnblocked(static_cast<uint16_t>(newRow), static_cast<uint16_t>(newCol))) {

            // Mark as visited and add to path
            visited[newRow][newCol] = true;
            currentPath.addCoordinate(static_cast<uint16_t>(newRow), static_cast<uint16_t>(newCol));

            // Recursive call
            if (dfsRecursive(matrixWorld, currentPath, visited, targetLength))
            {
                return true;
            }

            // Backtrack
            (void)currentPath.getNextCoordinate(); // Remove last coordinate
            visited[newRow][newCol] = false;
        }
    }
    
    return false;
}
