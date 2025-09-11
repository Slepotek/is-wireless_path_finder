/**
 * @file test_cli_utils.cpp
 * @brief Unit tests for CLI utilities
 * @author Slepotek
 * @date September 2025
 * @version 1.0
 */

#include "cli_utils.hpp"
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

/**
 * @brief Tests basic CLI parameter parsing functionality
 * 
 * Validates parsing of required parameters (rows, cols, pathLength) and
 * verifies that optional parameters use correct default values.
 * Tests the core functionality without optional parameters.
 * 
 * Test case: --rows 5 --cols 8 --pathLength 12
 * Expected: All required params parsed, defaults applied for optional params
 */
void testBasicParameterParsing()
{
    std::cout << "Testing basic parameter parsing..." << std::endl;

    const std::vector<std::string> args = {"pathFinder", "--rows", "5", "--cols", "8", "--pathLength", "12"};
    size_t argc = args.size(); // Fixed: CLIParser expects size_t, not int

    CLIParameters params = CLIParser(argc, args);

    assert(params.rows == 5);
    assert(params.cols == 8);
    assert(params.pathLength.value == 12);
    assert(params.maxStartingPoints.value == 5); // Default value
    assert(params.blockedCells.empty());

    std::cout << "✓ Basic parameter parsing test passed" << std::endl;
}

/**
 * @brief Tests maxStartingPoints optional parameter parsing
 * 
 * Validates that the optional maxStartingPoints parameter is correctly
 * parsed and overrides the default value of 5. Tests type-safe parameter
 * structure handling.
 * 
 * Test case: --maxStartingPoints 10 (with other required params)
 * Expected: maxStartingPoints.value == 10, other params parsed correctly
 */
void testMaxStartingPointsParsing()
{
    std::cout << "Testing maxStartingPoints parameter..." << std::endl;

    const std::vector<std::string> args =
        {"pathFinder", "--rows", "3", "--cols", "3", "--pathLength", "4", "--maxStartingPoints", "10"};
    size_t argc = args.size(); // Fixed: CLIParser expects size_t, not int

    CLIParameters params = CLIParser(argc, args);

    assert(params.rows == 3);
    assert(params.cols == 3);
    assert(params.pathLength.value == 4);
    assert(params.maxStartingPoints.value == 10);

    std::cout << "✓ MaxStartingPoints parameter test passed" << std::endl;
}

/**
 * @brief Tests blocked cells coordinate parsing functionality
 * 
 * Validates parsing of multiple blocked cell coordinates in {row,col} format.
 * Tests the regex parsing logic and coordinate extraction. Verifies that
 * coordinates are stored in correct order and with correct values.
 * 
 * Test case: --blockedCells {1,0} {2,1} {0,3}
 * Expected: 3 blocked cells parsed with correct coordinates in order
 */
void testBlockedCellsParsing()
{
    std::cout << "Testing blocked cells parsing..." << std::endl;

    const std::vector<std::string> args =
        {"pathFinder", "--rows", "4", "--cols", "4", "--pathLength", "6", "--blockedCells", "{1,0}", "{2,1}", "{0,3}"};
    size_t argc = args.size(); // Fixed: CLIParser expects size_t, not int

    CLIParameters params = CLIParser(argc, args);

    assert(params.rows == 4);
    assert(params.cols == 4);
    assert(params.pathLength.value == 6);
    assert(params.blockedCells.size() == 3);
    assert(params.blockedCells[0].first == 1 && params.blockedCells[0].second == 0);
    assert(params.blockedCells[1].first == 2 && params.blockedCells[1].second == 1);
    assert(params.blockedCells[2].first == 0 && params.blockedCells[2].second == 3);

    std::cout << "✓ Blocked cells parsing test passed" << std::endl;
}

/**
 * @brief Tests complete parameter set with all optional parameters
 * 
 * Comprehensive test that validates parsing when all parameters are provided,
 * including both optional parameters (maxStartingPoints and blockedCells).
 * Tests the full CLI parsing capability and parameter integration.
 * 
 * Test case: All parameters provided including multiple blocked cells
 * Expected: All parameters parsed correctly, no defaults used
 */
void testCompleteParameterSet()
{
    std::cout << "Testing complete parameter set..." << std::endl;

    const std::vector<std::string> args = {"pathFinder",
                                           "--rows",
                                           "6",
                                           "--cols",
                                           "6",
                                           "--pathLength",
                                           "15",
                                           "--maxStartingPoints",
                                           "8",
                                           "--blockedCells",
                                           "{0,0}",
                                           "{1,1}"};
    size_t argc = args.size(); // Fixed: CLIParser expects size_t, not int

    CLIParameters params = CLIParser(argc, args);

    assert(params.rows == 6);
    assert(params.cols == 6);
    assert(params.pathLength.value == 15);
    assert(params.maxStartingPoints.value == 8);
    assert(params.blockedCells.size() == 2);
    assert(params.blockedCells[0].first == 0 && params.blockedCells[0].second == 0);
    assert(params.blockedCells[1].first == 1 && params.blockedCells[1].second == 1);

    std::cout << "✓ Complete parameter set test passed" << std::endl;
}

/**
 * @brief Tests blocked cells file parameter parsing functionality
 * 
 * Validates parsing of blocked cell coordinates from file using --blockedCellsFile parameter.
 * Tests the file reading logic, comment handling, and coordinate extraction.
 * Uses a test file with known coordinates to verify correct parsing.
 * 
 * Test case: --blockedCellsFile test_blocked_cells.txt
 * Expected: 3 blocked cells parsed from file (0,1), (1,0), (2,2)
 */
void testBlockedCellsFileParsing()
{
    std::cout << "Testing blocked cells file parsing..." << std::endl;

    const std::vector<std::string> args = {"pathFinder", "--rows", "4", "--cols", "4", "--pathLength", "6", 
                                           "--blockedCellsFile", "/home/marcel/Code/is-wireless/tests/cli_utils_tests/test_blocked_cells.txt"};
    size_t argc = args.size();

    CLIParameters params = CLIParser(argc, args);

    assert(params.rows == 4);
    assert(params.cols == 4);
    assert(params.pathLength.value == 6);
    assert(params.blockedCells.size() == 3);
    assert(params.blockedCells[0].first == 0 && params.blockedCells[0].second == 1);
    assert(params.blockedCells[1].first == 1 && params.blockedCells[1].second == 0);
    assert(params.blockedCells[2].first == 2 && params.blockedCells[2].second == 2);

    std::cout << "✓ Blocked cells file parsing test passed" << std::endl;
}

/**
 * @brief Main test runner for CLI utilities test suite
 * 
 * Executes all CLI parsing test functions in sequence and reports results.
 * Tests cover basic parsing, optional parameters, blocked cells, file input,
 * and complete parameter sets. Uses assert() for validation - program terminates on failure.
 * 
 * @return 0 on success (all tests passed), program terminates on assertion failure
 */
int main()
{
    std::cout << "Running CLI Utils tests...\n" << std::endl;

    testBasicParameterParsing();
    testMaxStartingPointsParsing();
    testBlockedCellsParsing();
    testCompleteParameterSet();
    testBlockedCellsFileParsing();

    std::cout << "\n✓ All CLI Utils tests passed!" << std::endl;
    return 0;
}
