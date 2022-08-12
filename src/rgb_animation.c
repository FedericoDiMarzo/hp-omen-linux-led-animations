#include <string.h>
#include <syslog.h>
#include <stdlib.h>
#include <unistd.h>

#include "daemon.h"
#include "utils.h"

#define HEXLEN 6
#define STR_MAX_LEN 10000
#define RGB_ZONE_PATH_0 "/sys/devices/platform/hp-wmi/rgb_zones/zone00"
#define RGB_ZONE_PATH_1 "/sys/devices/platform/hp-wmi/rgb_zones/zone01"
#define RGB_ZONE_PATH_2 "/sys/devices/platform/hp-wmi/rgb_zones/zone02"
#define RGB_ZONE_PATH_3 "/sys/devices/platform/hp-wmi/rgb_zones/zone03"


void set_color(char *color, int zone) {
    if (zone == 0) SET_COLOR_M(color, 0);
    if (zone == 1) SET_COLOR_M(color, 1);
    if (zone == 2) SET_COLOR_M(color, 2);
    if (zone == 3) SET_COLOR_M(color, 3);
}

int main() {
    char colors[][7] = {"fca311", "023e8a", "e63946", "8ac926"};
    size_t colors_len = ARRAY_SIZE_M(colors);

    // run_daemon();

    // Open the log file
    openlog("rgb_animation", LOG_PID, LOG_DAEMON);
    syslog(LOG_NOTICE, "rgb_animation is running");
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

    syslog(LOG_NOTICE, "rgb_animation terminated");
    closelog();

    return EXIT_SUCCESS;
}