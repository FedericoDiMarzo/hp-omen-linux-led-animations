#pragma once

#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Max lenght of a string
 */
#define STR_MAX_LEN 1000

/**
 * @brief Calculates the size of an array
 */
#define ARRAY_SIZE_M(XS) sizeof(XS) / sizeof(*XS)

/**
 * @brief Apply the module operator
 *
 * @param X input
 * @param M modulo
 */
#define WRAP_M(X, M) ((X) % (M))

/**
 * @brief Max between two numbers
 *
 * @param X first number
 * @param Y second number
 */
#define MAX_M(X, Y) ((X) > (Y) ? (X) : (Y))

/**
 * @brief Min between two numbers
 *
 * @param X first number
 * @param Y second number
 */
#define MIN_M(X, Y) ((X) < (Y) ? (X) : (Y))

/**
 * @brief Safe string copy
 *
 * @param DEST name of the string to create
 * @param ORIG source string
 * @param N maximum lenght of the new string
 */
#define STRCPY_SAFE_M(DEST, ORIG, N)            \
    char DEST[N + 1];                           \
    memcpy(DEST, ORIG, MIN_M(strlen(ORIG), N)); \
    DEST[MIN_M(strlen(ORIG), N)] = '\0';

/**
 * @brief Tries to parse a string
 *
 * @param PARSER parser macro
 * @param S input string
 * @param RES result of the parsin
 * @param CODE_ERROR code to execute in case of errors
 */
#define STR_TRYPARSE_M(PARSER, S, RES, CODE_ERROR) \
    do {                                           \
        char* str_end[1];                          \
        RES = PARSER(S, str_end);                  \
        if (*str_end == S) {                       \
            CODE_ERROR;                            \
        }                                          \
    } while (0)

/**
 * @brief Parses a string to a float
 */
#define STR_TO_FLOAT_M(S, STR_END) strtof(S, STR_END)

/**
 * @brief Parses a string to a double
 */
#define STR_TO_DOUBLE_M(S, STR_END) strod(S, STR_END)

/**
 * @brief Parses a string to a 32 bit int
 */
#define STR_TO_INT_M(S, STR_END) ((int)strtol(S, STR_END, 10))

/**
 * @brief Python-like context manager to open a file
 *
 * @param F name of the file pointer to open
 * @param PATH path to the file to open
 * @param MODE mode parameter of fopen
 * @param CODE_SUCCESS code to execute in case open success
 * @param CODE_ERROR code to execute in case open error
 */
#define WITH_FILE_M(F, PATH, MODE, CODE_SUCCESS, CODE_ERROR) \
    do {                                                     \
        FILE* F = fopen(PATH, MODE);                         \
        if (F == NULL) {                                     \
            CODE_ERROR;                                      \
            break;                                           \
        }                                                    \
        CODE_SUCCESS;                                        \
        fclose(F);                                           \
    } while (0)

/**
 * @brief Gets the home directory of the user that launched the program
 */
#define GET_HOME_DIR_M() (getpwnam(getlogin())->pw_dir)
