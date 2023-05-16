/*
BSD 2-Clause License

Copyright (c) 2023, Archie Collison

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "included/termcolor-c.h"

#define MAX_CMD_LEN 3
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
    printf("success!");
    reset_colors(stdout);

    printf("\n\n\n");
}

void command_prompt(char length_str[MAX_CMD_LEN], char rest_length_str[MAX_CMD_LEN], int *length, int *rest_length) {
    printf("pomodoro length (1-60): ");
    fgets(length_str, MAX_CMD_LEN, stdin);
    printf("rest length (1-60): ");
    fgets(rest_length_str, MAX_CMD_LEN, stdin);

    *length = atoi(length_str);
    *rest_length = atoi(rest_length_str);
}

void pomodoro_display(int *hour,int *min,int *sec,int length) {
    int i = 0;
    while (i < length * 60) {
        update_time(hour, min, sec);
        sleep(1);
        refresh_line();
        i++;
    }
}
int continue_prompt() {
    text_underline(stdout);
    printf("begin rest timer?");
    reset_colors(stdout);
    printf(" (y/n): ");
    char input;
    fgets(&input, 2, stdin);

    if (input == 'y') {
        return 1;
    } else if (input == 'n') {
        return 0;
    } else {
        continue_prompt();
    }
}

int main(void) {
    char length_str[MAX_CMD_LEN];
    char rest_length_str[MAX_CMD_LEN];
    int length = 0;
    int rest_length = 0;
    printf("\e[1;1H\e[2J"); // works on ANSI terminals, demands POSIX
    command_prompt(length_str, rest_length_str, &length, &rest_length);

    printf("\e[1;1H\e[2J"); // works on ANSI terminals, demands POSIX
    printf("\n\n\n");

    // get time values
    time_t rawtime;
    struct tm *info;
    time(&rawtime);
    info = localtime(&rawtime);
    int hour = to_12hr(info->tm_hour);
    int min = info->tm_min;
    int sec = info->tm_sec;
    // pomodoro times
    success_time(hour, min, sec, length);
    pomodoro_display(&hour, &min, &sec, length);
    pd_success();
    if (continue_prompt()) {
        update_time(&hour, &min, &sec);
        printf("\e[1;1H\e[2J"); // works on ANSI terminals, demands POSIX
        printf("\n\n\n");
        success_time(hour, min, sec, rest_length);
        pomodoro_display(&hour, &min, &sec, rest_length);
    }
    return 0;
}