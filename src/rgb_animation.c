#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <syslog.h>
#include <unistd.h>

#include "daemon.h"
#include "utils.h"

#define HEXLEN 6
#define RGB_ZONE_COUNT 4
#define RGB_ZONE_PATH_0 "/sys/devices/platform/hp-wmi/rgb_zones/zone00"
#define RGB_ZONE_PATH_1 "/sys/devices/platform/hp-wmi/rgb_zones/zone01"
#define RGB_ZONE_PATH_2 "/sys/devices/platform/hp-wmi/rgb_zones/zone02"
#define RGB_ZONE_PATH_3 "/sys/devices/platform/hp-wmi/rgb_zones/zone03"
#define COLOR_FILE "/.rgb_keyboard/rgb_default"

void set_color(char *color, int zone) {
    if (zone == 0) SET_COLOR_M(color, 0);
    if (zone == 1) SET_COLOR_M(color, 1);
    if (zone == 2) SET_COLOR_M(color, 2);
    if (zone == 3) SET_COLOR_M(color, 3);
}

int load_colors(FILE *f, char colors[4][7]) {
    for (int i = 0; i < RGB_ZONE_COUNT; i++) {
        fgets(colors[i], HEXLEN + 2, f);
    }
}

int main() {
    // obtaining the color file path
    STR_MAKE_SAFE_M(colorpath, GET_HOME_DIR_M(), STR_MAX_LEN);
    strcat(colorpath, COLOR_FILE);

    // obtaining the color from the saved configuration
    char colors[4][7];
    WITH_FILE_M(f, colorpath, "r",
                // file open success
                if (load_colors(f, colors) == EXIT_FAILURE) return EXIT_FAILURE,
                // file open failure
                return EXIT_FAILURE);

    // executing as a daemon
    if(run_daemon() == EXIT_FAILURE)
        return EXIT_FAILURE;

    // open the log file
    openlog("rgb_animation", LOG_PID, LOG_DAEMON);
    syslog(LOG_NOTICE, "rgb_animation is running");

    // steps animation
    int counter = 0;
    int zone = 0;
    while (1) {
        set_color(colors[WRAP_M(counter + 0, 4)], 0);
        set_color(colors[WRAP_M(counter + 1, 4)], 1);
        set_color(colors[WRAP_M(counter + 2, 4)], 2);
        set_color(colors[WRAP_M(counter + 3, 4)], 3);

        counter = (counter + 1);
        sleep(1);
    }

    // closing the log file
    syslog(LOG_NOTICE, "rgb_animation terminated");
    closelog();

    return EXIT_SUCCESS;
}