#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define HEXLEN 6
#define RGB_ZONE_COUNT 4
#define RGB_ZONE_PATH_0 "/sys/devices/platform/hp-wmi/rgb_zones/zone00"
#define RGB_ZONE_PATH_1 "/sys/devices/platform/hp-wmi/rgb_zones/zone01"
#define RGB_ZONE_PATH_2 "/sys/devices/platform/hp-wmi/rgb_zones/zone02"
#define RGB_ZONE_PATH_3 "/sys/devices/platform/hp-wmi/rgb_zones/zone03"

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

/**
 * @brief Loads the saved RGB configuration
 *
 * @param f file where the configuration is saved
 * @param colors output
 */
void load_colors(FILE *f, char colors[4][7]);

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