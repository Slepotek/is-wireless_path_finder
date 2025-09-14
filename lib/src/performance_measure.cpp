/**
 * @file performance_measure.cpp
 * @brief Implementation of the PerformanceMeasure class.
 * @author Slepotek
 * @date September 2025
 * @version 1.0
 */

#include "performance_measure.hpp"
#include <cerrno>
#include <chrono>
#include <cstring>
#include <iostream>
#include <linux/perf_event.h>
#include <sys/ioctl.h>
#include <cstdlib> // For exit()

/**
 * @brief Constructs a new PerformanceMeasure object.
 * * This constructor initializes the perf_event_attr struct to monitor
 * CPU cycles, preparing the object for performance measurement.
 */
PerformanceMeasure::PerformanceMeasure()
{
    cycleCounterS.type = PERF_TYPE_HARDWARE;
    cycleCounterS.size = sizeof(perf_event_attr);
    cycleCounterS.config = PERF_COUNT_HW_CPU_CYCLES;
}

/**
 * @brief Starts the performance measurement.
 * * This function opens the perf event file descriptor for CPU cycles.
 * It resets and enables the counter, then records the start time.
 * @note This function will exit the program if it fails to open the
 * perf event file descriptor, as performance measurement is a critical
 * part of the application's functionality.
 */
void PerformanceMeasure::measureStart()
{
    perfFileDesc = static_cast<int32_t>(perf_event_open(&cycleCounterS, 0, -1, -1, 0));

    if (perfFileDesc == -1)
    {
        std::cerr << "Error opening the perf_event_open file descriptor." << "\n";
        std::cerr << "The performance measurement can not operate" << "\n";
        std::cerr << "Reason: " << strerror(errno) << " (errno " << errno << ")" << "\n";
        exit(1);
    }

    ioctl(perfFileDesc, PERF_EVENT_IOC_RESET, 0);
    ioctl(perfFileDesc, PERF_EVENT_IOC_ENABLE, 0);

    startTime = std::chrono::high_resolution_clock::now();

    std::cout << "Starting measurement" << "\n";
}

/**
 * @brief Stops the performance measurement.
 * * This function disables the perf event counter, reads the cycle count,
 * and closes the file descriptor. It also records the stop time.
 */
void PerformanceMeasure::measureStop()
{
    ioctl(perfFileDesc, PERF_EVENT_IOC_DISABLE);
    read(perfFileDesc, &cycleCount, sizeof(uint64_t));
    close(perfFileDesc);

    stopTime = std::chrono::high_resolution_clock::now();

    std::cout << "Stopping measurement" << "\n";
}

/**
 * @brief Prints the measured time and cycle counts.
 * * This function calculates the elapsed time in microseconds and prints
 * a formatted summary to the standard output.
 */
void PerformanceMeasure::printMeasurements()
{
    Measures printS = {};
    printS.timeCount = std::chrono::duration_cast<std::chrono::microseconds>(stopTime - startTime).count();
    printS.cycleCount = this->cycleCount;

    std::cout << "=== MEASURE SUMMARY ===" << "\n";
    std::cout << "Time taken(uS): " << printS.timeCount << "\n";
    std::cout << "Cycles: " << printS.cycleCount << "\n";
    std::cout << "=======================" << "\n";
}

/**
 * @brief Saves the measured time and cycle counts.
 * @return A Measures struct containing the final time (in milliseconds) and cycle counts.
 */
PerformanceMeasure::Measures PerformanceMeasure::saveMeasures()
{
    Measures saveS = {};
    saveS.timeCount = std::chrono::duration_cast<std::chrono::milliseconds>(stopTime - startTime).count();
    saveS.cycleCount = this->cycleCount;
    return saveS;
}