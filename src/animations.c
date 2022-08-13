#include "animations.h"

#include "utils.h"

// void set_color(char *color, int zone) {
//     if (zone == 0) SET_COLOR_M(color, 0);
//     if (zone == 1) SET_COLOR_M(color, 1);
//     if (zone == 2) SET_COLOR_M(color, 2);
//     if (zone == 3) SET_COLOR_M(color, 3);
// }

/**
 * @brief Loads the saved RGB configuration
 *
 * @param f file where the configuration is saved
 * @param colors output
 */
void load_colors(FILE* f, char colors[4][7]) {
    for (int i = 0; i < RGB_ZONE_COUNT; i++) {
        fgets(colors[i], HEXLEN + 2, f);
    }
}

/**
 * @brief Steps RGB animation
 *
 * @param colors array of colors
 * @param frequency frequency of color change
 * @return int error message
 */
int steps(char colors[4][7], float frequency) {
    int counter = 0;
    int zone = 0;
    while (1) {
        SET_COLOR_M(colors[WRAP_M(counter + 0, 4)], 0);
        SET_COLOR_M(colors[WRAP_M(counter + 1, 4)], 1);
        SET_COLOR_M(colors[WRAP_M(counter + 2, 4)], 2);
        SET_COLOR_M(colors[WRAP_M(counter + 3, 4)], 3);

        counter = (counter + 1);
        if (counter > 1000) counter = 0;

        sleep(1 / frequency);
    }

    // if the while breaks
    return EXIT_FAILURE;
}

/**
 * @brief Strobo RGB animation
 *
 * @param colors array of colors
 * @param frequency frequency of color change
 * @return int error message
 */
int strobo(char colors[4][7], float frequency) {
    int counter = 0;
    int zone = 0;
    char* black = "000000";
    while (1) {
        // RGB on
        SET_COLOR_M(colors[0], 0);
        SET_COLOR_M(colors[1], 1);
        SET_COLOR_M(colors[2], 2);
        SET_COLOR_M(colors[3], 3);
        sleep(1 / frequency);

        // RGB off
        SET_COLOR_M(black, 0);
        SET_COLOR_M(black, 1);
        SET_COLOR_M(black, 2);
        SET_COLOR_M(black, 3);
        sleep(1 / frequency);
    }

    // if the while breaks
    return EXIT_FAILURE;
}