/**
 * @file cli_utils.hpp
 * @brief CLI utilities for is-wireless application
 * @author Slepotek
 * @date September 2025
 * @version 1.0
 */

#ifndef CLI_UTILS_H
#define CLI_UTILS_H

#include "Ipath_algorithm.hpp"
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <utility>

/**
 * @struct CLIParameters
 * @brief Container for all parsed command line parameters
 * 
 * Holds validated parameters from CLI parsing including matrix dimensions,
 * path constraints, and blocked cell coordinates. Uses type-safe wrappers
 * for pathLength and maxStartingPoints to prevent parameter confusion.
 * 
 * @note All coordinates are 0-indexed matrix positions
 */
struct CLIParameters {
    uint16_t rows, cols;                                    ///< Matrix dimensions
    PathLength pathLength;                                  ///< Target path length
    MaxStartingPoints maxStartingPoints = {5};             ///< Max starting points to try
    std::vector<std::pair<uint16_t, uint16_t>> blockedCells; ///< Blocked cell coordinates
};

/**
 * @brief Parses command line arguments into CLIParameters structure
 * @param argc Number of command line arguments
 * @param argv Array of command line argument strings
 * @return Parsed parameters structure
 */
CLIParameters CLIParser(size_t argc, std::vector<std::string> argv);

/**
 * @brief Prints help information for the CLI
 */
void printHelp();

#endif
