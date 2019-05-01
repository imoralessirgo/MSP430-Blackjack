/*
 * test_runner.h
 *
 *  Created on: Apr 10, 2018
 *      Author: deemer
 */

#ifndef TEST_RUNNER_H_
#define TEST_RUNNER_H_

#include <stdint.h>
#include <assert.h>
#include <stdio.h>

#include <intrinsics.h>

/*
 * Break to debugger
 * Since the debugger does not appear to halt until
 * one or more instructions after the opcode is given,
 * we add a few nop's here.
 */
#define DEBUG_BREAK() do { \
    _op_code(0x4343); \
    _no_operation(); \
    _no_operation(); \
} while (0)

//#define VERBOSE_ASSERT_MESSAGES

#ifdef VERBOSE_ASSERT_MESSAGES
#define ASSERT_FAIL(cond) do { \
    fputs("Assertion failed, (" _STR(cond) "), file " __FILE__ \
          ", line " _STR(__LINE__) "\n", stderr); \
    fflush(stderr); \
} while(0)
#else
#define ASSERT_FAIL(cond) do { \
    fputs("Assertion failed:  if you can't see enough info, " \
          "set a breakpoint here and restart to see the program state.\n", stderr); \
    fflush(stderr); \
} while(0)
#endif


// TEST_ASSERT2(actual, expected)
// Break to debugger if (actual != expected)
// In the debugger, the variables window will show variables
// _actual and _expected, indicating the values of
// each side of the expression tested.
#ifdef VERBOSE_ASSERT_MESSAGES
#define TEST_ASSERT2(actual, expected) do { \
    volatile typeof(actual) _actual = (actual); \
    volatile typeof(expected) _expected = (expected); \
    if((_actual) != (_expected)) { \
        snprintf(__actual_str,   DEBUG_STR_SIZE, "%d", _actual); \
        snprintf(__expected_str, DEBUG_STR_SIZE, "%d", _expected); \
        fputs("Assertion failed:  Received:  ", stderr); \
        fputs(__actual_str, stderr); \
        fputs(", Expected:  ", stderr); \
        fputs(__expected_str, stderr); \
        fputs("\n", stderr); \
        fflush(stderr); \
        DEBUG_BREAK(); \
    } \
} while(0)
#else
#define TEST_ASSERT2(actual, expected) do { \
    volatile typeof(actual) _actual = (actual); \
    volatile typeof(expected) _expected = (expected); \
    if((_actual) != (_expected)) { \
        fputs("Assertion failed:  See Variables.  If you can't see enough info, " \
              "set a breakpoint here and restart to see the program state.\n", stderr); \
        fflush(stderr); \
        DEBUG_BREAK(); \
    } \
} while(0)
#endif


/*
 * TEST_ASSERT(cond)
 * Break to debugger is (cond) is false
 */
#define TEST_ASSERT(cond) do { \
    if(!(cond)) { \
        ASSERT_FAIL((cond)); \
        DEBUG_BREAK(); \
    } \
} while (0)


#endif /* TEST_RUNNER_H_ */
