#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <syslog.h>
#include <unistd.h>

#define HEXLEN 6
#define STR_MAX_LEN 10000
#define RGB_ZONE_PATH_0 "/sys/devices/platform/hp-wmi/rgb_zones/zone00"
#define RGB_ZONE_PATH_1 "/sys/devices/platform/hp-wmi/rgb_zones/zone01"
#define RGB_ZONE_PATH_2 "/sys/devices/platform/hp-wmi/rgb_zones/zone02"
#define RGB_ZONE_PATH_3 "/sys/devices/platform/hp-wmi/rgb_zones/zone03"

#define ARRAY_SIZE_M(XS) sizeof(XS) / sizeof(*XS)

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

static void run_daemon() {
    pid_t pid;

    // Fork off the parent process
    pid = fork();

    // An error occurred
    if (pid < 0)
        exit(EXIT_FAILURE);

    // Success: Let the parent terminate
    if (pid > 0)
        exit(EXIT_SUCCESS);

    // On success: The child process becomes session leader
    if (setsid() < 0)
        exit(EXIT_FAILURE);

    // Catch, ignore and handle signals
    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    // Fork off for the second time
    pid = fork();

    // An error occurred
    if (pid < 0)
        exit(EXIT_FAILURE);

    // Success: Let the parent terminate
    if (pid > 0)
        exit(EXIT_SUCCESS);

    // Set new file permissions
    umask(0);

    // Change the working directory to the root directory
    // or another appropriated directory
    chdir("/");

    // Close all open file descriptors
    int x;
    for (x = sysconf(_SC_OPEN_MAX); x >= 0; x--) {
        close(x);
    }
}

void set_color(char *color, int zone) {
    if (zone == 0) SET_COLOR_M(color, 0);
    if (zone == 1) SET_COLOR_M(color, 1);
    if (zone == 2) SET_COLOR_M(color, 2);
    if (zone == 3) SET_COLOR_M(color, 3);
}

int main() {
    char colors[][7] = {"C5979D", "4B8F8C", "484D6D", "2C365E", "2B193D"};
    size_t colors_len = ARRAY_SIZE_M(colors);

    // run_daemon();

    // Open the log file
    // openlog("firstdaemon", LOG_PID, LOG_DAEMON);
    int counter = 0;
    int zone = 0;
    while (1) {
        // syslog(LOG_NOTICE, "rgb_animation is running");
        for (int i = 0; i < 4; i++)
            set_color(colors[counter], i);

        counter = (counter + 1) % colors_len;
        sleep(1);
    }

    // syslog(LOG_NOTICE, "rgb_animation is terminated");
    closelog();

    return EXIT_SUCCESS;
}