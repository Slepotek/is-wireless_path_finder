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

Path DFSAlgorithm::findViablePath(MatrixWorld &matrixWorld, 
                                  PathLength pathLength, 
                                  MaxStartingPoints maxStartingPoints)
{
    if (pathLength.value == 0) {
        throw std::invalid_argument("Path length must be greater than zero");
    }

    if (pathLength.value > matrixWorld.getTotalCells()) {
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

bool DFSAlgorithm::dfsRecursive(MatrixWorld &matrixWorld,
                                Path &currentPath,
                                std::vector<std::vector<bool>> &visited,
                                uint16_t targetLength)
{
    // Base case: reached target length
    if (currentPath.getLength() == targetLength) {
        return true;
    }
    
    // Get current position
    auto [currentRow, currentCol] = currentPath.getCurrentCoordinate();
    
    // 4-directional movement: up, down, left, right
    // Keeping standard ints for arithmetic
    std::array<std::array<int, 2>, 4> directions = {{{-1, 0}, {0, 1}, {1, 0}, {0, -1}}};

    for (auto & direction : directions) {
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
            if (dfsRecursive(matrixWorld, currentPath, visited, targetLength)) {
                return true;
            }
            
            // Backtrack
            (void)currentPath.getNextCoordinate(); // Remove last coordinate
            visited[newRow][newCol] = false;
        }
    }
    
    return false;
}
