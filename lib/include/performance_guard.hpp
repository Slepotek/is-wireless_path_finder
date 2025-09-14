/**
 * @file performance_guard.hpp
 * @brief Header for the PerformanceMeasureGuard class.
 * @author Slepotek
 * @date September 2025
 * @version 1.0
 */

#ifndef PERFORMANCE_GUARD_HPP
#define PERFORMANCE_GUARD_HPP

#include "performance_measure.hpp"

/**
 * @class PerformanceMeasureGuard
 * @brief A RAII guard for performance measurement.
 * * This class automatically starts a performance measurement on construction
 * and stops it on destruction. It ensures that measurements are consistently
 * taken and logged, even in the presence of exceptions. The measurement is
 * controlled by a global flag, allowing for easy enabling and disabling
 * of performance monitoring.
 */
class PerformanceMeasureGuard
{
private:
    /**
     * @brief Pointer to the PerformanceMeasure instance.
     * * This class holds a non-owning pointer to an external
     * PerformanceMeasure instance to perform its operations on.
     */
    PerformanceMeasure *performanceMeasurementInstance; 

public:
    /**
     * @brief A global flag to enable or disable performance measurement.
     * * Measurement is only performed if this flag is set to true.
     */
    static bool isMeasurementEnabled;

    /**
     * @brief Constructs a new PerformanceMeasureGuard object.
     * @param pmInstance A pointer to the PerformanceMeasure instance to use.
     * * The constructor starts the measurement only if the global
     * isMeasurementEnabled flag is set.
     */
    explicit PerformanceMeasureGuard(PerformanceMeasure *pmInstance)
        : performanceMeasurementInstance(pmInstance)
    {
        if (isMeasurementEnabled)
        {
            performanceMeasurementInstance->measureStart();
        }
    }

    /**
     * @brief Destructor for the PerformanceMeasureGuard.
     * * The destructor stops the measurement and prints the results
     * if the global flag is enabled. This is guaranteed to be called
     * when the object goes out of scope, providing RAII.
     */
    ~PerformanceMeasureGuard()
    {
        if (isMeasurementEnabled)
        {
            performanceMeasurementInstance->measureStop();
            performanceMeasurementInstance->printMeasurements();
        }
    }

    /**
     * @brief Deleted copy constructor.
     * * This is a RAII class and should not be copied.
     */
    PerformanceMeasureGuard(const PerformanceMeasureGuard &) = delete;

    /**
     * @brief Deleted assignment operator.
     * * This is a RAII class and should not be assigned.
     * @return A reference to the PerformanceMeasureGuard.
     */
    PerformanceMeasureGuard &operator=(const PerformanceMeasureGuard &) = delete;
};

#endif // PERFORMANCE_GUARD_HPP