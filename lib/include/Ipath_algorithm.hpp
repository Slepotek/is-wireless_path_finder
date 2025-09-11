#ifndef PATH_ALGORITHM_HPP
#define PATH_ALGORITHM_HPP

#include "matrix_utils.hpp"
#include "path.hpp"
#include <string>

// Type-safe structures for clarity and error avoidance

/**
 * @brief Type-safe wrapper for path length values
 * 
 * Prevents accidental parameter swapping and improves code readability.
 * Contains a single uint16_t value representing the desired path length.
 */
struct PathLength
{
    uint16_t value;
};

/**
 * @brief Type-safe wrapper for maximum starting points
 * 
 * Prevents accidental parameter swapping and improves code readability.
 * Contains a uint16_t value with default of 5 starting points to try.
 */
struct MaxStartingPoints
{
    uint16_t value = 5; // Default value
};

/**
 * @class PathAlgorithm
 * @brief Abstract base class for path finding algorithms
 * 
 * Defines the interface for all path finding algorithms in the system.
 * Implementations must provide path finding logic and algorithm identification.
 * Uses type-safe parameter wrappers to prevent argument confusion.
 * 
 * @note This is a pure virtual interface - cannot be instantiated directly
 */
class PathAlgorithm
{
public:
    virtual ~PathAlgorithm() = default;
    
    /**
     * @brief Finds a viable path in the given matrix world
     * @param world Matrix world to search in
     * @param pathLength Desired path length (type-safe wrapper)
     * @param maxStartingPoints Maximum starting points to try (default: 5)
     * @return Path object containing found path (empty if none found)
     */
    virtual Path findViablePath(const MatrixWorld &world,
                                PathLength pathLength,
                                MaxStartingPoints maxStartingPoints) = 0;
    
    /**
     * @brief Gets the human-readable name of the algorithm
     * @return Algorithm name string for display/logging purposes
     */
    [[nodiscard]] virtual std::string getAlgorithmName() const = 0;
};
#endif // PATH_ALGORITHM_HPP