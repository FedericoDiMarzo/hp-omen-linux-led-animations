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

// /**
//  * @brief Iterates through an array
//  *
//  * @param X iteration element
//  * @param I iteration index
//  * @param XS array to iterate
//  * @param TYPE type of the element
//  * @param CODE code to execute at each iteration
//  */
// #define FOREACH_M(X, I, XS, TYPE, CODE)              \
//     do {                                             \
//         for (int I = 0; I < ARRAY_SIZE_M(XS); I++) { \
//             TYPE X = XS[I];                          \
//             CODE;                                    \
//         }                                            \
//     } while (0)

/**
 * @brief Iterates through an array
 *
 * @param X iteration element
 * @param I iteration index
 * @param XS array to iterate
 * @param TYPE type of the element
 */
#define FOREACH_M(X, I, XS, TYPE)                                                   \
    for (int I = 0, _FOREACH_M##X##keep = 1, _FOREACH_M##X##len = ARRAY_SIZE_M(XS); \
         _FOREACH_M##X##keep == 1 && I < _FOREACH_M##X##len;                        \
         _FOREACH_M##X##keep = 1, I++)                                              \
        for (TYPE X = *((XS) + I); _FOREACH_M##X##keep == 1; _FOREACH_M##X##keep = 0)

/**
 * @brief Maps a function to an array to generate a new array
 *
 * @param XS original array
 * @param YS new array
 * @param OLD_TYPE type of XS
 * @param NEW_TYPE type of YS
 * @param F function to apply to the elements of XS
 */
#define ARRAY_MAP_M(XS, YS, OLD_TYPE, NEW_TIPE, F) \
    NEW_TYPE YS[ARRAY_SIZE_M(XS)];                 \
    FOREACH_M(x, i, XS, OLD_TYPE) {                \
        YS[i] = F(x);                              \
    }

/**
 * @brief Maps a function to an array
 *
 * @param XS target array
 * @param TYPE type of the array
 * @param F function to map
 */
#define ARRAY_MAP_SAME_M(XS, TYPE, F) \
    FOREACH_M(x, i, XS, TYPE) {       \
        XS[i] = F(x);                 \
    }

/**
 * @brief Apply the module operator
 *
 * @param X input
 * @param M modulo
 */
#define WRAP_M(X, M) ((X) % (M))

#define LIN_INTERPOL_M(X, Y, I) ((Y) * (I) - (X) * (1 - (I)))

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

#define CLIP_M(X, MIN, MAX) (MAX((MIN_M((X), MAX)), MIN))

/**
 * @brief Safe string copy
 *
 * @param DEST name of the string to create
 * @param ORIG source string
 * @param N maximum lenght of the new string
 */
#define STRCPY_SAFE_M(DEST, ORIG, N)            \
    char DEST[(N) + 1];                         \
    memcpy(DEST, ORIG, MIN_M(strlen(ORIG), N)); \
    DEST[MIN_M(strlen(ORIG), N)] = '\0';

/**
 * @brief Tries to parse a string
 *
 * @param PARSER parser macro
 * @param S input string
 * @param RES result of the parsin
 * @param __VA_ARGS__ code to execute in case of errors
 */
#define STR_TRYPARSE_M(PARSER, S, RES, ...) \
    do {                                    \
        char* str_end[1];                   \
        RES = PARSER(S, str_end);           \
        if (*str_end == S) {                \
            __VA_ARGS__;                    \
        }                                   \
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
 * @brief Parses a string to an int
 */
#define STR_TO_INT_M(S, STR_END) ((int)strtol(S, STR_END, 10))

/**
 * @brief Parses a string in base 16 to an int
 */
#define STR16_TO_INT_M(S, STR_END) ((int)strtol(S, STR_END, 16))

/**
 * @brief Python-like context manager to open a file
 *
 * @param F name of the file pointer to open
 * @param PATH path to the file to open
 * @param MODE mode parameter of fopen
 * @param CODE_ERROR code to execute in case open error
 * @param __VA_ARGS__ code to execute in case open success
 */
#define WITH_FILE_M(F, PATH, MODE, CODE_ERROR, ...) \
    do {                                            \
        FILE* F = fopen(PATH, MODE);                \
        if (F == NULL) {                            \
            CODE_ERROR;                             \
            break;                                  \
        }                                           \
        __VA_ARGS__;                                \
        fclose(F);                                  \
    } while (0)

/**
 * @brief Gets the home directory of the user that launched the program
 */
#define GET_HOME_DIR_M() (getpwnam(getlogin())->pw_dir)

#define GET_LOOKUP_TABLE_M(F, N, DEST, START, STOP)                                     \
    float DEST[N];                                                                      \
    if (START >= STOP)                                                                  \
        memset(DEST, 0, (N) * sizeof(*DEST));                                           \
    else {                                                                              \
        FOREACH_M(_, i, DEST, float) {                                                  \
            float x = LIN_INTERPOL_M((float)START, (float)STOP, (float)i / (float)(N)); \
            DEST[i] = F(x);                                                             \
        }                                                                               \
    }
