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
        FILE *F = fopen(PATH, MODE);                         \
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

/**
 * @brief Sets the color to a keyboard zone
 * 
 * @param COLOR hex color string
 * @param ZONE zone of the keyboard
 */
#define SET_COLOR_M(COLOR, ZONE)                                                       \
    do {                                                                               \
        STRCPY_SAFE_M(safecolor, COLOR, HEXLEN);                                       \
        WITH_FILE_M(f, RGB_ZONE_PATH_##ZONE, "w", fprintf(f, "%s", safecolor), break); \
    } while (0);
