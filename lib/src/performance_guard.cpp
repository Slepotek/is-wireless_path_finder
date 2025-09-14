/**
 * @file performance_guard.cpp
 * @brief Implementation file for the PerformanceMeasureGuard class.
 * @author Slepotek
 * @date September 2025
 * @version 1.0
 */

#include "performance_guard.hpp"

/**
 * @brief Definition and initialization of the static member variable.
 * * This line allocates memory for the static variable and initializes it.
 * This must be done in a single source file to avoid linker errors.
 */
bool PerformanceMeasureGuard::isMeasurementEnabled = false;
