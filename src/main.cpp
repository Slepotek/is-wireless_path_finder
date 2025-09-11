/**
 * @file main.cpp
 * @brief Main entry point for is-wireless path finding application
 * @author Slepotek
 * @date September 2025
 * @version 1.0
 * 
 * Provides CLI interface for adaptive path finding in NxM matrices.
 * Parses command line arguments, validates parameters, and executes
 * the selected path finding algorithm with visualization output.
 */

#include "cli_utils.hpp"
#include "dfs_algorithm.hpp"
#include "matrix_utils.hpp"
#include "path.hpp"
#include <cstddef>
#include <iostream>

/**
 * @brief Main entry point for is-wireless path finding application
 * @param argc Number of command line arguments
 * @param argv Array of command line argument strings
 * @return 0 on success, 1 on error (invalid parameters or path finding failure)
 * 
 * Application workflow:
 * 1. Converts C-style argv to std::vector<std::string> for type safety
 * 2. Parses command line arguments using CLIParser
 * 3. Creates MatrixWorld with specified dimensions
 * 4. Blocks specified cells in the matrix
 * 5. Executes DFS algorithm to find viable path
 * 6. Outputs path coordinates or reports failure
 * 
 * Error handling:
 * - Invalid CLI parameters: CLIParser throws exceptions (program terminates)
 * - Cell blocking failures: Returns error code 1
 * - Path finding failures: Reports empty path gracefully
 * 
 * @note Uses type-safe parameter structures (PathLength, MaxStartingPoints)
 * @note Provides detailed parameter output for verification and debugging
 */
int main(int argc, char *argv[])
{
    // Convert argv to vector<string> for type safety and easier handling
    auto argc_size = static_cast<size_t>(argc);
    std::vector<std::string> args;
    args.reserve(argc_size);
    for (size_t index = 0; index < argc_size; ++index)
    {
        args.emplace_back(argv[index]);
    }

    // Parse command line arguments (may throw exceptions for invalid input)
    CLIParameters params = CLIParser(argc_size, args);
    // Output parsed parameters for verification and debugging
    std::cout << "Rows: " << params.rows << std::endl;
    std::cout << "Cols: " << params.cols << std::endl;
    std::cout << "Path Length: " << params.pathLength.value << std::endl;
    std::cout << "Max Starting Points: " << params.maxStartingPoints.value << std::endl;
    std::cout << "Blocked Cells: ";
    for (auto iterator = params.blockedCells.begin();
         // limit output to first 100 blocked cells to avoid flooding console
         iterator != params.blockedCells.end() &&
         std::distance(params.blockedCells.begin(), iterator) < MAX_PATH_PRINT_LENGTH;
         ++iterator)
    {
        std::cout << "{" << iterator->first << "," << iterator->second << "} ";
    }
    std::cout << std::endl;

    // Create matrix world with specified dimensions
    MatrixWorld matrix(params.rows, params.cols);

    // Block specified cells (validate success)
    if (!matrix.matrixBlanking(params.blockedCells))
    {
        std::cerr << "Error: Failed to block specified cells. Check coordinates are within matrix bounds." << std::endl;
        return 1;
    }

    // Execute DFS path finding algorithm
    DFSAlgorithm dfs;
    Path path = dfs.findViablePath(matrix, params.pathLength, params.maxStartingPoints);

    // Output results
    if (path.isEmpty())
    {
        std::cout << "No viable path found with the specified parameters." << std::endl;
        std::cout << "Try reducing path length or increasing max starting points." << std::endl;
    }
    else
    {
        path.printPath();
    }

    return 0;
}