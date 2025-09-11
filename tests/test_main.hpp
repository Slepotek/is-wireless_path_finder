/**
 * @file test_main.hpp
 * @brief Common test utilities and macros for the test suite
 * @author Slepotek
 * @date September 2025
 * @version 1.0
 * 
 * Provides shared utilities and macros used across all test files in the
 * is-wireless project. Contains compiler-specific workarounds and common
 * testing patterns to ensure consistent behavior across the test suite.
 */

#ifndef TEST_MAIN_HPP
#define TEST_MAIN_HPP

/**
 * @brief Macro to suppress unused variable warnings
 * @param x Variable or expression to mark as intentionally unused
 * 
 * Used to suppress compiler warnings for variables that are intentionally
 * unused, such as in exception handling tests where we catch exceptions
 * but don't use the exception object, or when testing functions that
 * return values we don't need to examine.
 * 
 * Example usage:
 * @code
 * try {
 *     auto result = someFunction();
 *     UNUSED(result); // We don't need to examine the result
 * } catch (const std::exception& e) {
 *     UNUSED(e); // We caught it but don't need the details
 * }
 * @endcode
 * 
 * @note This is a portable solution that works across different compilers
 * @note The cast to void is a standard idiom for marking variables as used
 */
#define UNUSED(x) ((void)(x))

#endif // TEST_MAIN_HPP
