#include "animations.h"

#include "utils.h"

#define UPDATE_RATE M_2_PI
#define LOOKUP_TABLE_LEN 512

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
        colors[i][6] = '\0';
    }
}

float positive_sin(float x) {
    return (x + 1);
}

float scale_sin(float x) {
    return x / 2;
}

float clip_sin(float x) {
    return (float)fmax(0.36, x);
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

/**
 * @brief Breath RGB animation
 *
 * @param colors array of colors
 * @param frequency frequency of color change
 * @return int error message
 */
int breath(char colors[4][7], float frequency) {
    // defining sin lookup table
    GET_LOOKUP_TABLE_M(sinf, LOOKUP_TABLE_LEN, sin_lt, 0, 2 * M_PI);
    // mapping the values between 0.36 and 1
    ARRAY_MAP_SAME_M(sin_lt, float, positive_sin);
    ARRAY_MAP_SAME_M(sin_lt, float, scale_sin);
    ARRAY_MAP_SAME_M(sin_lt, float, clip_sin);

    // converting the colors to rgb
    RGB_t colors_rgb[4];
    RGB_t colors_rgb_scaled[4];
    FOREACH_M(_, i, colors_rgb, RGB_t) {
        HEX_TO_RGB(colors[i], colors_rgb[i]);
    }

    float update_pediod = 1.0f / UPDATE_RATE;
    unsigned int n = 0;

    while (1) {
        float s = sin_lt[n];

        FOREACH_M(c, i, colors_rgb, RGB_t) {
            colors_rgb_scaled[i].r = (float)c.r * s;
            colors_rgb_scaled[i].g = (float)c.g * s;
            colors_rgb_scaled[i].b = (float)c.b * s;
        }
        FOREACH_M(c, i, colors_rgb_scaled, RGB_t) {
            RGB_TO_HEX(c, hex);
            if (i == 0) SET_COLOR_M(hex, 0);
            if (i == 1) SET_COLOR_M(hex, 1);
            if (i == 2) SET_COLOR_M(hex, 2);
            if (i == 3) SET_COLOR_M(hex, 3);
        }
        n += LOOKUP_TABLE_LEN * frequency / UPDATE_RATE;
        if (n >= LOOKUP_TABLE_LEN)
            n %= LOOKUP_TABLE_LEN;
        sleep(update_pediod);
    }

    return EXIT_FAILURE;
}

/**
 * @brief Waves RGB animation
 *
 * @param colors array of colors
 * @param frequency frequency of color change
 * @return int error message
 */
int wave_uniform(char colors[4][7], float frequency) {
    // defining sin lookup table
    GET_LOOKUP_TABLE_M(sinf, LOOKUP_TABLE_LEN, sin_lt, 0, 2 * M_PI);
    // mapping the values between 0 and 2
    ARRAY_MAP_SAME_M(sin_lt, float, positive_sin);

    // converting the colors to rgb
    RGB_t colors_rgb[4];
    RGB_t color_rgb_interpol;
    FOREACH_M(_, i, colors_rgb, RGB_t) {
        HEX_TO_RGB(colors[i], colors_rgb[i]);
    }

    float update_pediod = 1.0f / UPDATE_RATE;
    unsigned int n = 0;

    while (1) {
        float s = sin_lt[n];

        if (s < 1) {
            // interpolation color 0-1
            color_rgb_interpol.r = LIN_INTERPOL_M(colors_rgb[0].r, colors_rgb[1].r, s);
            color_rgb_interpol.g = LIN_INTERPOL_M(colors_rgb[0].g, colors_rgb[1].g, s);
            color_rgb_interpol.b = LIN_INTERPOL_M(colors_rgb[0].b, colors_rgb[1].b, s);

        } else {
            // interpolation color 2-3
            s -= 1;
            color_rgb_interpol.r = LIN_INTERPOL_M(colors_rgb[2].r, colors_rgb[3].r, s);
            color_rgb_interpol.g = LIN_INTERPOL_M(colors_rgb[2].g, colors_rgb[3].g, s);
            color_rgb_interpol.b = LIN_INTERPOL_M(colors_rgb[2].b, colors_rgb[3].b, s);
        }
        RGB_TO_HEX(color_rgb_interpol, hex);
        SET_COLOR_M(hex, 0);
        SET_COLOR_M(hex, 1);
        SET_COLOR_M(hex, 2);
        SET_COLOR_M(hex, 3);

        n += LOOKUP_TABLE_LEN * frequency / UPDATE_RATE;
        if (n >= LOOKUP_TABLE_LEN)
            n %= LOOKUP_TABLE_LEN;
        sleep(update_pediod);
    }

    return EXIT_FAILURE;
}

// 6930c3
// 48cae4
// 5e60ce
// 5e60ce