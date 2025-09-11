/**
 * @file path.cpp
 * @brief Implementation of Path class for adaptive path finding algorithms
 * @author Slepotek
 * @date September 2025
 * @version 1.0
 */

#include "path.hpp"
#include <iostream>
#include <stdexcept>

/**
 * @brief Constructor implementation - creates an empty path
 * 
 * Uses default initialization for the deque container.
 * The path starts empty and is considered trivially contiguous.
 */
Path::Path() = default;

/**
 * @brief Adds coordinate to path using efficient deque operation
 * 
 * Appends coordinate to the end of the path using emplace_back for
 * optimal performance. Does not validate contiguity during insertion
 * for maximum DFS performance - validation is done separately.
 * 
 * @param row Row coordinate (0-based matrix index)
 * @param col Column coordinate (0-based matrix index)
 */
void Path::addCoordinate(uint16_t row, uint16_t col)
{
    path.emplace_back(row, col);
}

/**
 * @brief Implements stack-like pop operation for DFS backtracking
 * 
 * Returns the last coordinate and removes it from the path in one operation.
 * This is the core operation for DFS algorithm backtracking when exploring
 * dead ends in the search space.
 * 
 * @return The coordinate pair that was removed from the path
 * @throws std::out_of_range If path is empty
 */
std::pair<uint16_t, uint16_t> Path::getNextCoordinate()
{
    if (path.empty())
    {
        throw std::out_of_range("Path is empty, cannot get next coordinate.");
    }
    auto coord = path.back();
    path.pop_back();
    return coord;
}

/**
 * @brief Gets current position without modifying path state
 * 
 * Provides access to the current (last) coordinate for DFS decision making
 * without removing it from the path. Used for neighbor analysis and
 * path extension decisions.
 * 
 * @return The last coordinate pair in the path
 * @throws std::out_of_range If path is empty
 */
std::pair<uint16_t, uint16_t> Path::getCurrentCoordinate() const
{
    if (path.empty())
    {
        throw std::out_of_range("Path is empty, cannot get current coordinate.");
    }
    return path.back();
}

/**
 * @brief Validates path contiguity using underflow-safe arithmetic
 * 
 * Checks that each consecutive coordinate pair in the path are adjacent
 * using 4-directional connectivity (Manhattan distance = 1). Uses conditional
 * expressions to prevent unsigned integer underflow when calculating distances.
 * 
 * Algorithm:
 * - Empty or single-coordinate paths are trivially contiguous
 * - For each consecutive pair, calculate Manhattan distance safely
 * - Adjacent cells have Manhattan distance of exactly 1 (one step away)
 * 
 * @return true if entire path is contiguous, false if any gap exists
 */
bool Path::isContiguous() const
{
    if (path.size() < 2)
    {
        return true; // A path with 0 or 1 coordinate is trivially contiguous
    }
    for (size_t i = 1; i < path.size(); ++i)
    {
        auto [row1, col1] = path[i - 1];
        auto [row2, col2] = path[i];
        
        // Calculate Manhattan distance using underflow-safe arithmetic
        uint16_t row_dist = (row1 > row2) ? (row1 - row2) : (row2 - row1);
        uint16_t col_dist = (col1 > col2) ? (col1 - col2) : (col2 - col1);
        
        // Adjacent cells have Manhattan distance of exactly 1
        if ((row_dist + col_dist) != 1)
        {
            return false; // Not contiguous if not adjacent
        }
    }
    return true;
}

/**
 * @brief Checks if path contains no coordinates
 * 
 * Delegates to deque's empty() method for optimal performance.
 * 
 * @return true if path contains no coordinates, false otherwise
 */
bool Path::isEmpty() const
{
    return path.empty();
}

/**
 * @brief Gets the number of coordinates in the path
 * 
 * Delegates to deque's size() method which provides O(1) performance.
 * 
 * @return Number of coordinates currently in the path
 */
size_t Path::getLength() const
{
    return path.size();
}

/**
 * @brief Removes all coordinates from the path
 * 
 * Resets the path to empty state using deque's clear() method.
 * After clearing, isEmpty() returns true and getLength() returns 0.
 */
void Path::clear()
{
    path.clear();
}

/**
 * @brief Prints the path coordinates to standard output
 * 
 * Outputs the path in a readable format for debugging and visualization.
 * Each coordinate is printed as (row, col).
 */
void Path::printPath() const
{
    std::cout << "Path coordinates: ";
    for (const auto &coord : path)
    {
        std::cout << "(" << coord.first << ", " << coord.second << ") ";
    }
    std::cout << std::endl;
}
