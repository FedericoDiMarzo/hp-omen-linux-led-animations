#include <string.h>
#include <syslog.h>

#include "animations.h"
#include "daemon.h"
#include "utils.h"

#define COLOR_FILE "/.rgb_keyboard/rgb_default"

int main(int argc, char** argv) {
    // argument parsing
    if (argc != 3) return EXIT_FAILURE;
    char* animation = argv[1];
    char* frequency_str = argv[2];
    float frequency;

    STR_TRYPARSE_M(STR_TO_FLOAT_M, argv[2], frequency,
                   printf("error parsing the frequency\n");
                   return EXIT_FAILURE);

    // obtaining the color file path
    STRCPY_SAFE_M(colorpath, GET_HOME_DIR_M(), STR_MAX_LEN);
    strcat(colorpath, COLOR_FILE);

    // obtaining the color from the saved configuration
    char colors[4][7];
    WITH_FILE_M(f, colorpath, "r",
                // file open failure
                return EXIT_FAILURE,
                // file open success
                load_colors(f, colors));

    // executing as a daemon
    // if(run_daemon() == EXIT_FAILURE)
    //     return EXIT_FAILURE;

    // open the log file
    openlog("rgb_animation", LOG_PID, LOG_DAEMON);
    syslog(LOG_NOTICE, "rgb_animation is running");

    // animation
    if (strcmp(animation, "steps") == 0)
        steps(colors, frequency);
    else if (strcmp(animation, "strobo") == 0)
        strobo(colors, frequency);
    else if (strcmp(animation, "breath") == 0)
        breath(colors, frequency);
    else if (strcmp(animation, "wave_uniform") == 0)
        wave_uniform(colors, frequency);
    else {
        printf("%s animation does not exist\n", animation);
        return EXIT_FAILURE;
    }

    float xs[] = {1.0, 1, 4, 1.9};
    for (int keep = 1,
             i = 0,
             size = sizeof(xs) / sizeof *(xs);
         keep && i != size;
         keep = !keep, i++)
        for (float x = *((xs) + i); keep; keep = !keep)
            break;

    // closing the log file
    syslog(LOG_NOTICE, "rgb_animation terminated");
    closelog();

    return EXIT_SUCCESS;
}