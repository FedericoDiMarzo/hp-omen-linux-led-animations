#pragma once

#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define STR_MAX_LEN 1000

#define ARRAY_SIZE_M(XS) sizeof(XS) / sizeof(*XS)

#define WRAP_M(X, M) ((X) % (M))
#define MAX_M(X, Y) ((X) > (Y) ? (X) : (Y))
#define MIN_M(X, Y) ((X) < (Y) ? (X) : (Y))

#define STR_MAKE_SAFE_M(DEST, ORIG, N)          \
    char DEST[N + 1];                           \
    memcpy(DEST, ORIG, MIN_M(strlen(ORIG), N)); \
    DEST[MIN_M(strlen(ORIG), N)] = '\0';

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

#define GET_HOME_DIR_M() (getpwnam(getlogin())->pw_dir)

#define SET_COLOR_M(COLOR, ZONE)                                                       \
    do {                                                                               \
        STR_MAKE_SAFE_M(safecolor, COLOR, HEXLEN);                                     \
        WITH_FILE_M(f, RGB_ZONE_PATH_##ZONE, "w", fprintf(f, "%s", safecolor), break); \
    } while (0);
