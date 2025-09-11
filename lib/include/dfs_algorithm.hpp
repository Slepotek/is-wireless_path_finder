/**
 * @file dfs_algorithm.hpp
 * @brief DFS path finding algorithm implementation
 * @author Slepotek
 * @date September 2025
 * @version 1.0
 */

#ifndef DFS_ALGORITHM_H
#define DFS_ALGORITHM_H

#include "Ipath_algorithm.hpp"
#include "matrix_utils.hpp"
#include "path.hpp"
#include <cstdint>
#include <vector>

/**
 * @class DFSAlgorithm
 * @brief Depth-First Search algorithm for finding contiguous paths in matrix
 * 
 * Implements DFS with backtracking to find paths of specified length in a matrix.
 * Uses smart starting point selection and integrates with existing components.
 */
class DFSAlgorithm : public PathAlgorithm
{
public:
    /**
     * @brief Finds a viable path of specified length using DFS
     * @param matrixWorld Reference to the matrix world
     * @param pathLength Target path length wrapped in PathLength struct
     * @param maxStartingPoints Maximum starting points to try (default: {5})
     * @return Path object containing the found path (empty if none found)
     * @throws std::invalid_argument If pathLength.value is zero or exceeds matrix size
     * 
     * Uses type-safe parameter wrappers to prevent accidental argument swapping.
     * The maxStartingPoints parameter defaults to {5} when not specified.
     * 
     * Example usage:
     * @code
     * DFSAlgorithm dfs;
     * Path result = dfs.findViablePath(world, {12}, {3}); // path length 12, max 3 starting points
     * Path result2 = dfs.findViablePath(world, {8});      // path length 8, default 5 starting points
     * @endcode
     */
    [[nodiscard]] Path findViablePath(const MatrixWorld &matrixWorld,
                                      PathLength pathLength,
                                      MaxStartingPoints maxStartingPoints = {}) override;

    /** @brief Returns the name of the algorithm */
    [[nodiscard]] std::string getAlgorithmName() const override
    {
        return "Depth-First Search (DFS) Algorithm";
    }

private:
    /**
     * @brief Recursive DFS implementation with backtracking
     * @param matrixWorld Reference to the matrix world
     * @param currentPath Current path being built
     * @param visited Visited cells tracking
     * @param targetLength Target path length
     * @return true if target length reached, false otherwise
     */
    bool dfsRecursive(const MatrixWorld &matrixWorld,
                      Path &currentPath,
                      std::vector<std::vector<bool>> &visited,
                      uint16_t targetLength);
};

#endif
