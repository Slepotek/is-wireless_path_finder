/**
 * @file matrix_utils.hpp
 * @brief Matrix utilities for path finding algorithm
 * @author Slepotek
 * @date September 2025
 * @version 1.0
 */

#ifndef MATRIX_UTILS_H
#define MATRIX_UTILS_H

#include <vector>
#include <cstdint>

/**
 * @class MatrixWorld
 * @brief Represents a 2D matrix world for path finding algorithms
 * 
 * This class manages a virtual world represented as a 2D matrix where cells can be
 * either blocked (impassable) or unblocked (passable). It provides functionality
 * for matrix manipulation, cell state management, and neighbor analysis required
 * for path finding algorithms.
 * 
 * Cell State Convention:
 * - false = unblocked/passable cell
 * - true = blocked/impassable cell
 * 
 * @note This class uses 4-directional neighbor analysis (up, down, left, right)
 */
class MatrixWorld
{
private:
    std::vector<bool> worldMatrix;    ///< Internal matrix storage (false=unblocked, true=blocked)
    uint16_t rows;                    ///< Number of rows in the matrix
    uint16_t cols;                    ///< Number of columns in the matrix
    uint32_t noOfUnblockedCells;      ///< Counter for unblocked (passable) cells
    uint32_t noOfBlockedCells;        ///< Counter for blocked (impassable) cells

    /**
     * @brief Converts 2D coordinates to 1D array index
     * @param row Row coordinate (0-based)
     * @param col Column coordinate (0-based)
     * @return Linear index in the internal vector
     * @throws std::length_error If matrix is empty
     * @throws std::invalid_argument If coordinates are out of bounds
     */
    [[nodiscard]] size_t getIndex(uint16_t row, uint16_t col) const;

    /**
     * @brief Internal matrix initialization helper
     * @param rows Number of rows for the matrix
     * @param cols Number of columns for the matrix
     * @throws std::invalid_argument If rows or cols is zero
     * @throws std::length_error If matrix size exceeds memory limits
     */
    void matrixInitialize(uint16_t rows, uint16_t cols);

public:
    /**
     * @brief Constructs a new MatrixWorld with specified dimensions
     * @param rows Number of rows (default: 2)
     * @param cols Number of columns (default: 2)
     * @throws std::invalid_argument If rows or cols is zero
     * @throws std::length_error If matrix size exceeds memory limits
     * 
     * Creates a matrix where all cells are initially unblocked (passable).
     */
    MatrixWorld(uint16_t rows = 2, uint16_t cols = 2);

    /**
     * @brief Resizes the matrix to new dimensions
     * @param rows New number of rows
     * @param cols New number of columns
     * @return true on success, false on failure
     * 
     * All existing data is lost and the matrix is reset to all unblocked cells.
     */
    bool matrixResize(uint16_t rows, uint16_t cols);

    /**
     * @brief Blocks multiple cells in the matrix
     * @param coordinates Vector of (row, col) pairs to block
     * @return true if all cells were successfully blocked, false otherwise
     * 
     * Only updates counters for cells that were previously unblocked.
     * Skips cells that are already blocked without error.
     */
    bool matrixBlanking(std::vector<std::pair<uint16_t, uint16_t>> coordinates);

    /**
     * @brief Checks if the matrix contains only unblocked cells
     * @return true if no cells are blocked, false otherwise
     */
    [[nodiscard]] bool matrixIsEmpty() const;

    /**
     * @brief Sets the state of a specific cell
     * @param row Row coordinate (0-based)
     * @param col Column coordinate (0-based)
     * @param state Cell state (true=blocked, false=unblocked)
     * @return true on success, false if coordinates are invalid
     * 
     * @note This method automatically updates cell counters when state changes.
     * Only updates counters if the cell state actually changes.
     */
    bool setCell(uint16_t row, uint16_t col, bool state);

    /**
     * @brief Resets all cells to unblocked state
     * @return true on success, false if matrix is empty
     */
    bool clearMatrix();

    /**
     * @brief Gets the number of columns (width of each row)
     * @return Number of columns in the matrix
     */
    [[nodiscard]] uint16_t getRowSize() const;

    /**
     * @brief Gets the number of rows (height of each column)
     * @return Number of rows in the matrix
     */
    [[nodiscard]] uint16_t getColSize() const;

    /**
     * @brief Counts unblocked neighbors in 4 directions
     * @param row Row coordinate of the center cell
     * @param col Column coordinate of the center cell
     * @return Number of unblocked neighbors (0-4)
     * 
     * Counts neighbors in up, down, left, right directions only.
     * Returns 0 if the center coordinates are invalid.
     */
    [[nodiscard]] uint16_t countUnblockedNeighbors(uint16_t row, uint16_t col) const;

    /**
     * @brief Checks if a cell is unblocked (passable)
     * @param row Row coordinate (0-based)
     * @param col Column coordinate (0-based)
     * @return true if cell is unblocked, false if blocked
     * @throws std::length_error If matrix is empty
     * @throws std::invalid_argument If coordinates are out of bounds
     */
    [[nodiscard]] bool isUnblocked(uint16_t row, uint16_t col) const;

    /**
     * @brief Gets the total number of unblocked cells
     * @return Count of unblocked (passable) cells
     */
    [[nodiscard]] uint16_t getNoOfUnblockedCells() const;

    /**
     * @brief Gets the total number of blocked cells
     * @return Count of blocked (impassable) cells
     */
    [[nodiscard]] uint16_t getNoOfBlockedCells() const;

    /**
     * @brief Calculates the ratio of blocked to unblocked cells
     * @return Ratio as blocked_cells / unblocked_cells
     * @throws std::invalid_argument If either count is zero (division by zero)
     */
    [[nodiscard]] double getBlockedToUnblockedRatio() const;

    /**
     * @brief Gets the total number of cells in the matrix
     * @return Total cell count (rows × columns)
     * 
     * This method provides the overall matrix size, useful for bounds checking
     * and validation in path finding algorithms. Equivalent to rows × cols.
     */
    [[nodiscard]] size_t getTotalCells() const;
};
#endif