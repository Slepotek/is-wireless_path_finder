/**
 * @file performance_measure.hpp
 * @brief Header for the PerformanceMeasure class.
 * @author Slepotek
 * @date September 2025
 * @version 1.0
 */

#ifndef PERFORMANCE_MEASURE_HPP
#define PERFORMANCE_MEASURE_HPP

#include <chrono>
#include <cstdint>
#include <linux/perf_event.h>
#include <sys/syscall.h>
#include <unistd.h>

/**
 * @class PerformanceMeasure
 * @brief A utility class for measuring code execution time and CPU cycles.
 * * This class uses high-resolution clocks and the Linux perf_event_open
 * syscall to provide precise measurements of a code block's performance.
 * It is designed to be used with a PerformanceMeasureGuard for RAII-style
 * measurement.
 */
class PerformanceMeasure
{
private:
    /**
     * @brief The start time point for the measurement.
     */
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
    
    /**
     * @brief The end time point for the measurement.
     */
    std::chrono::time_point<std::chrono::high_resolution_clock> stopTime;
    
    /**
     * @brief The number of CPU cycles counted during the measurement.
     */
    uint64_t cycleCount;
    
    /**
     * @brief The configuration for the perf event cycle counter.
     */
    perf_event_attr cycleCounterS = {};
    
    /**
     * @brief The file descriptor for the perf event counter.
     */
    int32_t perfFileDesc;
    
    /**
     * @brief A helper function to open a perf event.
     * @param attr Pointer to a perf_event_attr structure.
     * @param pid Process ID to measure (-1 for current process).
     * @param cpu CPU to measure (-1 for all CPUs).
     * @param group_fd File descriptor of the perf event group leader.
     * @param flags Flags for the syscall.
     * @return A file descriptor for the event or -1 on error.
     *
     * This function wraps the perf_event_open syscall for easier use.
     */
    static int64_t perf_event_open(struct perf_event_attr *attr, pid_t pid, int cpu, int group_fd, unsigned long flags)
    {
        return syscall(__NR_perf_event_open, attr, pid, cpu, group_fd, flags);
    }

public:
    /**
     * @struct Measures
     * @brief A struct to hold the measured time and cycle counts.
     */
    struct Measures
    {
        /**
         * @brief The elapsed time in microseconds.
         */
        uint64_t timeCount;
        
        /**
         * @brief The number of CPU cycles.
         */
        uint64_t cycleCount;
    };
    
    /**
     * @brief Constructs a new PerformanceMeasure object.
     *
     * Initializes the perf event attributes and opens the perf event file
     * descriptor to prepare for cycle counting.
     */
    PerformanceMeasure();

    /**
     * @brief Starts the performance measurement.
     *
     * Resets the cycle counter and records the current time.
     */
    void measureStart();
    
    /**
     * @brief Stops the performance measurement.
     *
     * Records the stop time and reads the final cycle count from the
     * perf event counter.
     */
    void measureStop();
    
    /**
     * @brief Prints the measured time and cycle counts to the console.
     */
    void printMeasurements();
    
    /**
     * @brief Saves the measured time and cycle counts to a Measures struct.
     * @return A Measures struct containing the final time and cycle counts.
     */
    Measures saveMeasures();
};

#endif // PERFORMANCE_MEASURE_HPP