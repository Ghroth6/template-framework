#ifndef TFW_CLI_COMMON_H
#define TFW_CLI_COMMON_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "TFW_errorno.h"
#include "TFW_common_defines.h"
#include "TFW_sdk_log.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    char name[TFW_NORMAL_BUFFER_SIZE_MAX];
    int32_t (*func)(void);
} CliTestFunc;

typedef struct {
    const char* name;
    const char* description;
    CliTestFunc* functions;
    int32_t func_count;
} CliTestModule;

typedef CliTestModule* (*ModuleGetterFunc)(void);

// ============================================================================
// CLI utility functions
// ============================================================================

/**
 * @brief Read an integer value from standard input
 *
 * This inline function reads an integer from stdin and handles input errors
 *
 * @param value Pointer to store the read integer value
 * @return TFW_SUCCESS if input is valid, TFW_ERROR_INVALID_PARAM for invalid input
 *
 * @note This function will keep prompting until a valid integer is entered or EOF is reached
 *
 * Example usage:
 * @code
 * int32_t age;
 * printf("Enter your age: ");
 * int32_t result = TFW_CliReadInteger(&age);
 * if (result == TFW_SUCCESS) {
 *     printf("Your age is: %d\n", age);
 * } else {
 *     printf("Invalid input!\n");
 * }
 * @endcode
 */
static inline int32_t TFW_CliReadInteger(int32_t* value)
{
    char buffer[32];
    char* endptr;

    if (value == NULL) {
        return TFW_ERROR_INVALID_PARAM;
    }

    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return TFW_ERROR;
    }

    // Remove trailing newline if present
    char* newline = strchr(buffer, '\n');
    if (newline != NULL) {
        *newline = '\0';
    }

    // Convert string to integer
    int32_t result = (int32_t)strtol(buffer, &endptr, 10);

    // Check if conversion was successful
    if (*endptr != '\0' && *endptr != '\n' && *endptr != '\r') {
        return TFW_ERROR_INVALID_PARAM;
    }

    *value = result;
    return TFW_SUCCESS;
}

/**
 * @brief Read a string value from standard input
 *
 * This inline function reads a string from stdin and handles input errors
 *
 * @param buffer Pointer to the buffer to store the read string
 * @param buffer_size Size of the buffer
 * @return TFW_SUCCESS if input is valid, TFW_ERROR_INVALID_PARAM for invalid input
 *
 * @note This function will remove the trailing newline character if present
 *
 * Example usage:
 * @code
 * char name[50];
 * printf("Enter your name: ");
 * int32_t result = TFW_CliReadString(name, sizeof(name));
 * if (result == TFW_SUCCESS) {
 *     printf("Hello, %s!\n", name);
 * } else {
 *     printf("Failed to read name!\n");
 * }
 * @endcode
 */
static inline int32_t TFW_CliReadString(char* buffer, size_t buffer_size)
{
    if (buffer == NULL || buffer_size == 0) {
        return TFW_ERROR_INVALID_PARAM;
    }

    if (fgets(buffer, (int)buffer_size, stdin) == NULL) {
        return TFW_ERROR;
    }

    // Remove trailing newline if present
    char* newline = strchr(buffer, '\n');
    if (newline != NULL) {
        *newline = '\0';
    }

    return TFW_SUCCESS;
}

/**
 * @brief Read a float value from standard input
 *
 * This inline function reads a float from stdin and handles input errors
 *
 * @param value Pointer to store the read float value
 * @return TFW_SUCCESS if input is valid, TFW_ERROR_INVALID_PARAM for invalid input
 *
 * @note This function will keep prompting until a valid float is entered or EOF is reached
 *
 * Example usage:
 * @code
 * float price;
 * printf("Enter the price: ");
 * int32_t result = TFW_CliReadFloat(&price);
 * if (result == TFW_SUCCESS) {
 *     printf("The price is: %.2f\n", price);
 * } else {
 *     printf("Invalid input!\n");
 * }
 * @endcode
 */
static inline int32_t TFW_CliReadFloat(float* value)
{
    char buffer[32];
    char* endptr;

    if (value == NULL) {
        return TFW_ERROR_INVALID_PARAM;
    }

    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return TFW_ERROR;
    }

    // Remove trailing newline if present
    char* newline = strchr(buffer, '\n');
    if (newline != NULL) {
        *newline = '\0';
    }

    // Convert string to float
    float result = strtof(buffer, &endptr);

    // Check if conversion was successful
    if (*endptr != '\0' && *endptr != '\n' && *endptr != '\r') {
        return TFW_ERROR_INVALID_PARAM;
    }

    *value = result;
    return TFW_SUCCESS;
}

#ifdef __cplusplus
}
#endif

#endif // TFW_CLI_COMMON_H
