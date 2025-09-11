/**
 * @file path.hpp
 * @brief Path utilities for adaptive path finding algorithm
 * @author Slepotek
 * @date September 2025
 * @version 1.0
 */

#ifndef PATH_H
#define PATH_H

#include <cstdint>
#include <deque>
#include <utility>

/**
 * @class Path
 * @brief Path representation for adaptive path finding algorithm
 * 
 * This class manages a sequence of coordinates representing a path through a matrix.
 * It provides stack-like operations optimized for DFS backtracking while maintaining
 * full path iteration capabilities for validation and visualization.
 * 
 * Uses std::deque as the underlying container for optimal DFS performance:
 * - O(1) push_back()/pop_back() operations for DFS backtracking
 * - O(1) random access for contiguity validation
 * - Full iteration support for GUI path visualization
 * - O(1) size() query for length tracking
 * 
 * Path Validation:
 * - Supports 4-directional adjacency checking (up, down, left, right)
 * - Prevents integer underflow in contiguity calculations
 * - Validates complete path integrity
 * 
 * @note Perfect balance of stack-like operations with full container flexibility
 */
class Path {
private:
    std::deque<std::pair<uint16_t, uint16_t>> path;    ///< Internal path storage as coordinate sequence

public:
    /**
     * @brief Constructs an empty path
     * 
     * Creates a path with no coordinates. The path is initially empty and contiguous.
     */
    Path();

    /**
     * @brief Adds a coordinate to the end of the path
     * @param row Row coordinate (0-based matrix index)
     * @param col Column coordinate (0-based matrix index)
     * 
     * Appends the coordinate to the path using efficient deque push_back operation.
     * Does not validate contiguity - use isContiguous() for validation.
     */
    void addCoordinate(uint16_t row, uint16_t col);

    /**
     * @brief Gets the last coordinate and removes it from path (DFS backtracking)
     * @return The last coordinate pair before removal
     * @throws std::out_of_range if path is empty
     * 
     * Implements stack-like pop operation for DFS algorithm backtracking.
     * Returns the coordinate before removing it from the path.
     */
    [[nodiscard]] std::pair<uint16_t, uint16_t> getNextCoordinate();

    /**
     * @brief Gets the current (last) coordinate without removing it
     * @return The last coordinate pair in the path
     * @throws std::out_of_range if path is empty
     * 
     * Provides access to the current position for DFS decision making
     * without modifying the path state.
     */
    [[nodiscard]] std::pair<uint16_t, uint16_t> getCurrentCoordinate() const;

    /**
     * @brief Validates path contiguity using 4-directional adjacency
     * @return true if path is contiguous, false otherwise
     * 
     * Checks that each consecutive pair of coordinates in the path are
     * adjacent (Manhattan distance = 1). Uses underflow-safe arithmetic
     * for unsigned integer coordinate comparisons.
     */
    [[nodiscard]] bool isContiguous() const;

    /**
     * @brief Checks if the path contains no coordinates
     * @return true if path is empty, false otherwise
     */
    [[nodiscard]] bool isEmpty() const;

    /**
     * @brief Gets the number of coordinates in the path
     * @return Number of coordinates in the path
     */
    [[nodiscard]] size_t getLength() const;

    /**
     * @brief Removes all coordinates from the path
     * 
     * Resets the path to empty state. After clearing, isEmpty() returns true
     * and getLength() returns 0.
     */
    void clear();

    /**
     * @brief Prints the path coordinates to standard output
     * 
     * Outputs the path in a readable format for debugging and visualization.
     * Each coordinate is printed as (row, col).
     */
    void printPath() const;

    /**
     * @brief Gets iterator to the beginning of the path
     * @return Iterator pointing to the first coordinate
     * 
     * Enables range-based for loops and STL algorithm usage for path traversal.
     */
    [[nodiscard]] auto begin() const { return path.begin(); }

    /**
     * @brief Gets iterator to the end of the path
     * @return Iterator pointing past the last coordinate
     * 
     * Enables range-based for loops and STL algorithm usage for path traversal.
     */
    [[nodiscard]] auto end() const { return path.end(); }
};
#endif