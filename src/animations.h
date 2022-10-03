#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define HEXLEN 6
#define RGB_ZONE_COUNT 4
#define RGB_ZONE_PATH_0 "/sys/devices/platform/hp-wmi/rgb_zones/zone00"
#define RGB_ZONE_PATH_1 "/sys/devices/platform/hp-wmi/rgb_zones/zone01"
#define RGB_ZONE_PATH_2 "/sys/devices/platform/hp-wmi/rgb_zones/zone02"
#define RGB_ZONE_PATH_3 "/sys/devices/platform/hp-wmi/rgb_zones/zone03"

typedef struct RGB {
    int r, g, b;
} RGB_t;

/**
 * @brief Sets the color to a keyboard zone
 *
 * @param COLOR hex color string
 * @param ZONE zone of the keyboard
 */
#define SET_COLOR_M(COLOR, ZONE)                                                       \
    do {                                                                               \
        STRCPY_SAFE_M(safecolor, COLOR, HEXLEN);                                       \
        WITH_FILE_M(f, RGB_ZONE_PATH_##ZONE, "w", break, fprintf(f, "%s", safecolor)); \
    } while (0);

/**
 * @brief Converts a color from HEX to RGB_t
 *
 * @param HEX hexadecimal string
 * @param RGB rgb struct
 */
#define HEX_TO_RGB(HEX, RGB)                                     \
    do {                                                         \
        STRCPY_SAFE_M(r_str, HEX, 2);                            \
        STRCPY_SAFE_M(g_str, HEX + 2, 2);                        \
        STRCPY_SAFE_M(b_str, HEX + 4, 2);                        \
        STR_TRYPARSE_M(STR16_TO_INT_M, r_str, RGB.r, RGB.r = 0); \
        STR_TRYPARSE_M(STR16_TO_INT_M, g_str, RGB.g, RGB.g = 0); \
        STR_TRYPARSE_M(STR16_TO_INT_M, b_str, RGB.b, RGB.b = 0); \
    } while (0)

/**
 * @brief Converts a color from HEX to RGB_t
 *
 * @param HEX hexadecimal string
 * @param RGB rgb struct
 */
#define RGB_TO_HEX(RGB, HEX) \
    char HEX[HEXLEN + 1];    \
    snprintf(HEX, HEXLEN + 1, "%x%x%x", RGB.r, RGB.g, RGB.b);

/**
 * @brief Loads the saved RGB configuration
 *
 * @param f file where the configuration is saved
 * @param colors output
 */
void load_colors(FILE* f, char colors[4][7]);

/**
 * @brief Steps RGB animation
 *
 * @param colors array of colors
 * @param frequency frequency of color change
 * @return int error message
 */
int steps(char colors[4][7], float frequency);

/**
 * @brief Strobo RGB animation
 *
 * @param colors array of colors
 * @param frequency frequency of color change
 * @return int error message
 */
int strobo(char colors[4][7], float frequency);

/**
 * @brief Fade RGB animation
 *
 * @param colors array of colors
 * @param frequency frequency of color change
 * @return int error message
 */
int breath(char colors[4][7], float frequency);

/**
 * @brief Waves RGB animation
 *
 * @param colors array of colors
 * @param frequency frequency of color change
 * @return int error message
 */
int wave_uniform(char colors[4][7], float frequency);