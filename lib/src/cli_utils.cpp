/**
 * @file cli_utils.cpp
 * @brief Implementation of CLI utilities for is-wireless application
 * @author Slepotek
 * @date September 2025
 * @version 1.0
 */

#include "cli_utils.hpp"
#include "performance_guard.hpp"
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <regex>
#include <stdexcept>
#include <string> 
#include <fstream>
#include <sstream>

/**
 * @brief Prints comprehensive help information for the CLI application
 * 
 * Displays usage instructions, required and optional parameters, examples,
 * and important notes about the path finding algorithm. Called when user
 * specifies --help or -h flags.
 */
void printHelp() {
    std::cout << R"(pathFinder - Adaptive Path Finding in NxM Matrix

USAGE:
    pathFinder --rows R --cols C --pathLength N [OPTIONS]

REQUIRED:
    --rows R                Number of matrix rows (e.g., --rows 5)
    --cols C                Number of matrix columns (e.g., --cols 5)
    --pathLength N          Target path length (e.g., --pathLength 12)

OPTIONAL:
    --maxStartingPoints N   Maximum starting points to try (default: 5)
    --blockedCells COORDS   Blocked cell coordinates (e.g., --blockedCells {1,0} {2,1})
    --blockedCellsFile FILE Path to file containing blocked cell coordinates
    --enableMeasurement     Enable performance measurements (wall time and cycles) [*sudo required]
    --help, -h              Show this help message

EXAMPLES:
    pathFinder --rows 5 --cols 5 --pathLength 6
    pathFinder --rows 8 --cols 8 --pathLength 12 --blockedCells {1,0} {2,0} {1,1}
    pathFinder --rows 10 --cols 10 --pathLength 15 --maxStartingPoints 10
    sudo pathFinder --rows 10 --cols 10 --pathLength 15 --maxStartingPoints 10 --enableMeasurement
    pathFinder --rows 100 --cols 100 --pathLength 50 --blockedCellsFile blocked_cells.txt

BLOCKED CELLS FILE FORMAT:
    Each line should contain: row,col
    Lines starting with # are treated as comments
    Example file content:
        # Blocked cells for test matrix
        0,1
        1,0
        2,2

NOTES:
    - Matrix cells are 0-indexed
    - Path finds contiguous route through unblocked cells (value 0)
    - Blocked cells have value 1 and cannot be traversed
    - Algorithm uses DFS with smart starting point selection
    - Higher maxStartingPoints increases search thoroughness but takes longer
)" << std::endl;
}

/**
 * @brief Extracts blocked cell coordinates from command line arguments
 * @param index Reference to current argument index (modified during parsing)
 * @param argc Total number of command line arguments
 * @param argv Vector of command line argument strings
 * @param params Reference to CLIParameters structure to populate
 * 
 * Parses blocked cell coordinates in format {row,col} or row,col.
 * Handles shell brace expansion by automatically adding braces when missing.
 * Continues parsing until next flag (starting with '-') or end of arguments.
 * 
 * Supported formats:
 * - {0,1} - Standard format with braces
 * - 0,1   - Shell-expanded format (braces added automatically)
 */
static void extractBlockedCells(size_t &index, size_t argc, const std::vector<std::string> &argv, CLIParameters &params)
{
    while (index + 1 < argc && argv[index + 1][0] != '-')
    {
        std::string cellStr = argv[++index];
        // Handle shell brace expansion: "0,0" instead of "{0,0}"
        if (cellStr.find(',') != std::string::npos && cellStr[0] != '{')
        {
            cellStr.insert(cellStr.begin(), '{');
            cellStr += "}"; // Add missing braces
        }
        std::regex cellPattern(R"(\{(\d+),(\d+)\})");
        std::smatch match;
        if (std::regex_match(cellStr, match, cellPattern))
        {
            uint16_t row = static_cast<uint16_t>(std::stoi(match[1]));
            uint16_t col = static_cast<uint16_t>(std::stoi(match[2]));
            params.blockedCells.emplace_back(row, col);
        }
        else
        {
            std::cerr << "Invalid blocked cell format: " << cellStr << std::endl;
            std::cerr << "Expected format: {row,col}" << std::endl;
            break;
        }
    }
}

static void extractBlockedCellsFromFile(const std::string &filePath, CLIParameters &params)
{
    std::ifstream file(filePath);

    if (!file.is_open())
    {
        throw std::runtime_error("Can not open file: " + filePath);
        exit(1);
    }

    std::string line;
    while (std::getline(file, line))
    {
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#')
        {
            continue;
        }
        
        std::istringstream iss(line);
        std::string rowStr;
        std::string colStr;

        if (std::getline(iss, rowStr, ',') && std::getline(iss, colStr))
        {
            try
            {
                params.blockedCells.emplace_back(static_cast<uint16_t>(std::stoi(rowStr)),
                                                 static_cast<uint16_t>(std::stoi(colStr)));
            }
            catch (const std::exception &e)
            {
                std::cerr << "Invalid coordinate format: " << rowStr << "," << colStr << std::endl;
            }
        }
    }
}

/**
 * @brief Parses command line arguments into structured parameters
 * @param argc Number of command line arguments
 * @param argv Vector of command line argument strings
 * @return CLIParameters structure containing parsed values
 * 
 * Main CLI parsing function that processes all supported command line flags:
 * - --help, -h: Display help and exit
 * - --rows: Matrix row count (required)
 * - --cols: Matrix column count (required)  
 * - --pathLength: Target path length (required)
 * - --maxStartingPoints: Maximum starting points to try (optional, default: 5)
 * - --blockedCells: Blocked cell coordinates (optional)
 * 
 * Uses type-safe parameter structures (PathLength, MaxStartingPoints) to prevent
 * argument confusion. Delegates blocked cell parsing to extractBlockedCells().
 * 
 * @note Function exits with code 0 if --help flag is encountered
 */
CLIParameters CLIParser(size_t argc, std::vector<std::string> argv) {
    CLIParameters params;
    // no arguments provided, print help and exit
    if (argc == 1) {
        printHelp();
        exit(0);
    }

    for (size_t index = 0; index < argc; ++index) {
        if (argv[index] == std::string("--help") || argv[index] == std::string("-h")) {
            printHelp();
            exit(0);
        }
        else if (argv[index] == std::string("--rows") && index + 1 < argc) {
            params.rows = static_cast<uint16_t>(std::stoi(argv[++index]));
        }
        else if (argv[index] == std::string("--cols") && index + 1 < argc) {
            params.cols = static_cast<uint16_t>(std::stoi(argv[++index]));
        }
        else if (argv[index] == std::string("--pathLength") && index + 1 < argc) {
            params.pathLength.value = static_cast<uint16_t>(std::stoi(argv[++index]));
        }
        else if (argv[index] == std::string("--maxStartingPoints") && index + 1 < argc) {
            params.maxStartingPoints.value = static_cast<uint16_t>(std::stoi(argv[++index]));
        }
        else if (argv[index] == std::string("--blockedCells") && index + 1 < argc) {
            extractBlockedCells(index, argc, argv, params);
        }
        else if (argv[index] == std::string("--blockedCellsFile") && index + 1 < argc) {
            extractBlockedCellsFromFile(argv[++index], params);
        }
        else if (argv[index] == std::string("--enableMeasurement")) {
            PerformanceMeasureGuard::isMeasurementEnabled=true;
        }
    }
    return params;
}
