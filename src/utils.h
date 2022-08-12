#pragma once

#include <stdio.h>
#include <stdlib.h>

#define ARRAY_SIZE_M(XS) sizeof(XS) / sizeof(*XS)

#define WRAP_M(X, M) ((X) % (M))

#define STR_MAKE_SAFE_M(DEST, ORIG, N) \
    char DEST[N + 1];                  \
    memcpy(DEST, ORIG, N);             \
    DEST[N + 1] = '\0';

#define OPEN_FILE_M(F, PATH, MODE, CODE)                 \
    do {                                                 \
        FILE *F = fopen(PATH, MODE);                     \
        if (F == NULL) {                                 \
            printf("error opening the file %s\n", PATH); \
            break;                                       \
        }                                                \
        CODE;                                            \
        fclose(F);                                       \
    } while (0)

#define SET_COLOR_M(COLOR, ZONE)                                                \
    do {                                                                        \
        STR_MAKE_SAFE_M(safecolor, COLOR, HEXLEN);                              \
        OPEN_FILE_M(f, RGB_ZONE_PATH_##ZONE, "w", fprintf(f, "%s", safecolor)); \
    } while (0);
