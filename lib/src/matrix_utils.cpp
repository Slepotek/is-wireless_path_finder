/**
 * @file matrix_utils.cpp
 * @brief Implementation of MatrixWorld class for path finding algorithms
 * @author Slepotek
 * @date September 2025
 * @version 1.0
 */

#include "matrix_utils.hpp"
#include <algorithm>
#include <cstddef>
#include <stdexcept>
#include <array>

/**
 * @brief Constructor implementation - initializes matrix with given dimensions
 * 
 * Delegates to matrixInitialize() for actual initialization work.
 * Allows exceptions to bubble up naturally for proper error handling.
 */
MatrixWorld::MatrixWorld(uint16_t rows, uint16_t cols)
{
    matrixInitialize(rows, cols); // Exceptions bubble up
}

/**
 * @brief Converts 2D matrix coordinates to 1D vector index
 * 
 * Performs bounds checking and calculates linear index using row-major order.
 * Formula: index = row * cols + col
 * 
 * @param row Row coordinate (0-based)
 * @param col Column coordinate (0-based) 
 * @return Linear index for internal vector access
 * @throws std::length_error If matrix is empty
 * @throws std::invalid_argument If coordinates exceed matrix bounds
 */
size_t MatrixWorld::getIndex(uint16_t row, uint16_t col) const
{
    if (worldMatrix.empty())
    {
        throw std::length_error("The matrix is empty, index lookup is impossible!");
    }

    if (row >= rows || col >= cols)
    {
        throw std::invalid_argument("The given parameters are out of bounds of the matrix");
    }

    return (static_cast<size_t>(row) * cols) + col;
}

/**
 * @brief Resizes matrix to new dimensions with error handling
 * 
 * Wraps matrixInitialize() call in try-catch to provide bool return semantics.
 * All existing matrix data is lost during resize operation.
 * 
 * @param rows New number of rows
 * @param cols New number of columns
 * @return true if resize successful, false if initialization failed
 */
bool MatrixWorld::matrixResize(uint16_t rows, uint16_t cols)
{
    try
    {
        matrixInitialize(rows, cols);
        return true;
    }
    catch (const std::exception &)
    {
        return false;
    }
}

/**
 * @brief Blocks multiple cells while maintaining accurate counters
 * 
 * Iterates through coordinate pairs and blocks only previously unblocked cells.
 * This prevents double-counting and maintains counter accuracy.
 * Uses setCell() for bounds checking on each coordinate.
 * 
 * @param coordinates Vector of (row, col) pairs to block
 * @return true if all valid coordinates processed, false on any setCell failure
 */
bool MatrixWorld::matrixBlanking(std::vector<std::pair<uint16_t, uint16_t>> coordinates)
{
    bool result =
        std::all_of(coordinates.begin(), coordinates.end(), [this](const std::pair<uint16_t, uint16_t> &coordinate) {
            if (isUnblocked(coordinate.first, coordinate.second))
            {
                if (!setCell(coordinate.first, coordinate.second, true))
                {
                    return false; // setCell already handles bounds checking
                }
            }
            return true;
        });
    return result;
}

/**
 * @brief Core matrix initialization implementation
 * 
 * Validates dimensions, calculates memory requirements, and initializes
 * internal data structures. All cells start as unblocked (false).
 * Updates dimension variables and cell counters.
 * 
 * @param rows Number of matrix rows
 * @param cols Number of matrix columns
 * @throws std::invalid_argument If either dimension is zero
 * @throws std::length_error If total size exceeds vector capacity
 */
void MatrixWorld::matrixInitialize(uint16_t rows, uint16_t cols)
{
    if (rows == 0 || cols == 0)
    {
        throw std::invalid_argument("Matrix cannot be empty");
    }

    size_t matrixSize = static_cast<size_t>(rows) * cols;
    if (matrixSize > worldMatrix.max_size())
    {
        throw std::length_error("Matrix is too large for memory");
    }

    this->rows = rows;
    this->cols = cols;
    worldMatrix.resize(matrixSize, false);
    noOfUnblockedCells = static_cast<uint32_t>(matrixSize);
    noOfBlockedCells = 0;
}

/**
 * @brief Checks if matrix contains only unblocked cells
 * 
 * Uses blocked cell counter for O(1) performance instead of
 * iterating through entire matrix.
 * 
 * @return true if no cells are blocked (empty of obstacles)
 */
bool MatrixWorld::matrixIsEmpty() const
{
    return (noOfBlockedCells == 0);
}

/**
 * @brief Sets individual cell state with bounds checking
 * 
 * Uses getIndex() for coordinate validation and bounds checking.
 * Does not update cell counters - caller responsible for counter management.
 * Catches exceptions from getIndex() and converts to bool return.
 * 
 * @param row Row coordinate (0-based)
 * @param col Column coordinate (0-based)
 * @param state New cell state (true=blocked, false=unblocked)
 * @return true on success, false if coordinates invalid or matrix empty
 */
bool MatrixWorld::setCell(uint16_t row, uint16_t col, bool state)
{
    try
    {
        size_t indexToChange = getIndex(row, col);
        if (worldMatrix[indexToChange] != state)
        {
            worldMatrix[indexToChange] = state;
            // State change successful, update counters
            if (state)
            {
                // Cell set to blocked
                if (noOfUnblockedCells > 0) // Prevent underflow
                {
                    noOfUnblockedCells--;
                    noOfBlockedCells++;
                }
            }
            else
            {
                // Cell set to unblocked
                if (noOfBlockedCells > 0) // Prevent underflow
                {
                    noOfBlockedCells--;
                    noOfUnblockedCells++;
                }
            }
        }
        else
        {
            return false;
        }
        return true;
    }
    catch (const std::invalid_argument &)
    {
        return false; // Out of bounds
    }
    catch (const std::length_error &)
    {
        return false; // Empty matrix
    }
}

/**
 * @brief Resets all cells to unblocked state
 * 
 * Uses vector::assign() for efficient bulk operation.
 * Resets cell counters to reflect all-unblocked state.
 * Checks for empty matrix to avoid unnecessary operations.
 * 
 * @return true on successful clear, false if matrix was empty
 */
bool MatrixWorld::clearMatrix()
{
    if (worldMatrix.empty())
    {
        return false;
    }

    worldMatrix.assign(worldMatrix.size(), false);
    noOfUnblockedCells = static_cast<uint32_t>(worldMatrix.size());
    noOfBlockedCells = 0;
    return true;
}

/**
 * @brief Returns number of columns (width of each row)
 * 
 * Note: Method name might be confusing - returns column count,
 * which represents the size/width of each row in the matrix.
 * 
 * @return Number of columns in the matrix
 */
uint16_t MatrixWorld::getRowSize() const
{
    // Returning cols variable as the number of columns is the actual size
    // of one row, not the number of rows in the Matrix
    return cols;
}

/**
 * @brief Returns number of rows (height of each column)
 * 
 * Note: Method name might be confusing - returns row count,
 * which represents the size/height of each column in the matrix.
 * 
 * @return Number of rows in the matrix
 */
uint16_t MatrixWorld::getColSize() const
{
    // Returning row number as it is the actual size of one column, not the
    // number of columns in the matrix
    return rows;
}

/**
 * @brief Counts unblocked neighbors in 4 cardinal directions
 * 
 * Implements 4-directional neighbor analysis (up, down, left, right).
 * Validates center coordinates first, then checks each neighbor position
 * for bounds and blocked state. Uses signed arithmetic to handle edge cases.
 * 
 * @param row Row coordinate of center cell
 * @param col Column coordinate of center cell
 * @return Count of unblocked neighbors (0-4), or 0 if center coordinates invalid
 */
uint16_t MatrixWorld::countUnblockedNeighbors(uint16_t row, uint16_t col) const
{
    // First check if the given position is valid
    if (row >= rows || col >= cols) {
        return 0;  // Invalid position has no neighbors
    }
    
    uint16_t count = 0;
    
    // 4-directional offsets: up, right, down, left
    std::array<std::array<int, 2>, 4> directions = {{{-1, 0}, {0, 1}, {1, 0}, {0, -1}}};

    for (auto & direction : directions) {
        int newRow = static_cast<int>(row) + direction[0];
        int newCol = static_cast<int>(col) + direction[1];
        
        // Check bounds for neighbor position
        if (newRow >= 0 && newRow < static_cast<int>(rows) && 
            newCol >= 0 && newCol < static_cast<int>(cols)) {
            
            if (isUnblocked(static_cast<uint16_t>(newRow), static_cast<uint16_t>(newCol))) {
                count++;
            }
        }
    }
    
    return count;
}

/**
 * @brief Checks if specified cell is unblocked (passable)
 * 
 * Uses getIndex() for coordinate validation and bounds checking.
 * Inverts the stored boolean value since internal representation uses
 * false=unblocked, true=blocked convention.
 * 
 * @param row Row coordinate (0-based)
 * @param col Column coordinate (0-based)
 * @return true if cell is unblocked/passable
 * @throws std::length_error If matrix is empty
 * @throws std::invalid_argument If coordinates are out of bounds
 */
bool MatrixWorld::isUnblocked(uint16_t row, uint16_t col) const
{
    size_t indexToCheck = getIndex(row, col);
    return !worldMatrix[indexToCheck];  // Invert: false=unblocked, true=blocked
}

/**
 * @brief Returns cached count of unblocked cells
 * 
 * O(1) operation using maintained counter instead of matrix traversal.
 * Counter is updated by matrixBlanking() and other state-changing operations.
 * 
 * @return Current number of unblocked (passable) cells
 */
uint16_t MatrixWorld::getNoOfUnblockedCells() const
{
    return noOfUnblockedCells;
}

/**
 * @brief Returns cached count of blocked cells
 * 
 * O(1) operation using maintained counter instead of matrix traversal.
 * Counter is updated by matrixBlanking() and other state-changing operations.
 * 
 * @return Current number of blocked (impassable) cells
 */
uint16_t MatrixWorld::getNoOfBlockedCells() const
{
    return noOfBlockedCells;
}

/**
 * @brief Calculates ratio of blocked to unblocked cells
 * 
 * Performs division with explicit type casting for precision.
 * Validates both denominators to prevent division by zero.
 * Useful for analyzing matrix density and path-finding difficulty.
 * 
 * @return Ratio as double (blocked_count / unblocked_count)
 * @throws std::invalid_argument If either cell count is zero
 */
double MatrixWorld::getBlockedToUnblockedRatio() const
{
    if (noOfBlockedCells == 0 || noOfUnblockedCells == 0)
    {
        throw std::invalid_argument("Division by zero");
    }
    return static_cast<double>(noOfBlockedCells) / noOfUnblockedCells;
}

size_t MatrixWorld::getTotalCells() const
{
    return worldMatrix.size();
}
