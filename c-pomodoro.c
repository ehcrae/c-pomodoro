#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "termcolor-c.h"

#define MAX_CMD_LEN 10
#define DEFAULT_MINS 25

int to_12hr(int hour) {
    if (hour > 12) {
        hour -= 12;
    }
    return hour;
}

void update_time(int *hour, int *min, int *sec) {
    time_t rawtime;
    struct tm *info;
    time(&rawtime);
    info = localtime(&rawtime);

    *hour = info->tm_hour;
    *hour = to_12hr(*hour);
    *min = info->tm_min;
    *sec = info->tm_sec;

    printf("\t\t\t");
    text_bold(stdout);
    background_red(stdout);
    printf("%02d:%02d:%02d", *hour, *min, *sec);
    reset_colors(stdout);
    printf("\n");

}
void refresh_line() {
    printf("\x1b[2K");
    printf("\x1b[1F"); // Move to beginning of previous line
    printf("\x1b[2K"); // Clear entire line [ANSI escape codes]
}

void success_time(int hour, int min, int sec, int mins) {
    min += mins;
    if (mins > 59) {
        mins = mins % 60;
        hour++;
        to_12hr(hour);
    }

    text_bold(stdout);
    printf("\t\t\t");
    background_cyan(stdout);
    printf("%02d:%02d:%02d", hour, min, sec);
    reset_colors(stdout);
    printf("\n");
}

void pd_success() {
    printf("\n");
    printf("\t\t\t");
    text_bold(stdout);
    text_green(stdout);
    printf("Success!");
    reset_colors(stdout);

    printf("\n");
    
}

int main(int argc, char *argv[]) {
    char *command = argv[1];
    time_t rawtime;
    struct tm *info;
    time(&rawtime);
    info = localtime(&rawtime);


    int hour = info->tm_hour;
    hour = to_12hr(hour);
    int min = info->tm_min;
    int sec = info->tm_sec;

    int mins = DEFAULT_MINS;
    int i = 0;
    if (command) {
        mins = atoi(command);
    }

    int length = mins * 60;

    printf("\e[1;1H\e[2J"); // works on ANSI terminals, demands POSIX
    printf("\n\n\n");

    success_time(hour, min, sec, mins);

    while (i < length) {
        update_time(&hour, &min, &sec);
        sleep(1);
        refresh_line();
        i++;
    }

    pd_success();

    return 0;
}
