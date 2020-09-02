#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "../include/messages.h"
#include "../include/wes.h"
#include "../include/utility.h"
#include "../include/path.h"

int main(int argc, const char *argv[]) {
    int fd = connectPipe(FILENAME_WES_PIPE, O_WRONLY);
    FILE *status = openFile(FILENAME_STATUS_LOG, "w+");

    int speedPFC1Log = open(FILENAME_SPEEDPFC1_LOG, O_RDONLY | O_CREAT);
    int speedPFC2Log = open(FILENAME_SPEEDPFC2_LOG, O_RDONLY | O_CREAT);
    int speedPFC3Log = open(FILENAME_SPEEDPFC3_LOG, O_RDONLY | O_CREAT);

    double velocita_pfc1 = 0;
    double velocita_pfc2 = 0;
    double velocita_pfc3 = 0;

    char buffer_pfc1[string_length(APPLICATION_ENDED_MESSAGE) + 1] = {0};
    char buffer_pfc2[string_length(APPLICATION_ENDED_MESSAGE) + 1] = {0};
    char buffer_pfc3[string_length(APPLICATION_ENDED_MESSAGE) + 1] = {0};

    enum boolean terminated = FALSE;

    while(!terminated) {
        sleep(1);

        readLine(speedPFC1Log, buffer_pfc1, '\n');
        readLine(speedPFC2Log, buffer_pfc2, '\n');
        readLine(speedPFC3Log, buffer_pfc3, '\n');

        if((strcmp(buffer_pfc1, APPLICATION_ENDED_MESSAGE) == 0) ||
                (strcmp(buffer_pfc2, APPLICATION_ENDED_MESSAGE) == 0) ||
                (strcmp(buffer_pfc2, APPLICATION_ENDED_MESSAGE)) == 0) {
            terminated = TRUE;
        } else {
            velocita_pfc1 = readSpeed(speedPFC1Log);
            velocita_pfc2 = readSpeed(speedPFC2Log);
            velocita_pfc3 = readSpeed(speedPFC3Log);

            /*
             * TODO: controllare che velocita_pfc1, velocita_pfc2
             *       e velocita_pfc3 abbiano valori diversi da '\0'
             */

            if (velocita_pfc1 == velocita_pfc2) {
                if (velocita_pfc1 == velocita_pfc3) {
                    char *message = concat(WES_MESSAGE_SUCCESS, "\n");
                    int messageLength = string_length(WES_MESSAGE_SUCCESS) + 1;

                    printf("%s", message);
                    fwrite(message, sizeof(char), messageLength, status);
                } else {
                    char *message = concat(WES_MESSAGE_PFC3_ERROR, "\n");
                    int messageLength = string_length(WES_MESSAGE_PFC3_ERROR) + 1;

                    printf("%s", message);
                    fwrite(message, sizeof(char), messageLength, status);
                    write(fd, WES_MESSAGE_PFC3_ERROR, sizeof(char) * string_length(WES_MESSAGE_PFC3_ERROR));
                }
            } else {
                if (velocita_pfc1 == velocita_pfc3) {
                    char *message = concat(WES_MESSAGE_PFC2_ERROR, "\n");
                    int messageLength = string_length(WES_MESSAGE_PFC2_ERROR) + 1;

                    printf("%s", message);
                    fwrite(message, sizeof(char), messageLength, status);
                    write(fd, WES_MESSAGE_PFC2_ERROR, sizeof(char) * string_length(WES_MESSAGE_PFC2_ERROR));
                } else if (velocita_pfc2 == velocita_pfc3) {
                    char *message = concat(WES_MESSAGE_PFC1_ERROR, "\n");
                    int messageLength = string_length(WES_MESSAGE_PFC1_ERROR) + 1;

                    printf("%s", message);
                    fwrite(message, sizeof(char), messageLength, status);
                    write(fd, WES_MESSAGE_PFC1_ERROR, sizeof(char) * string_length(WES_MESSAGE_PFC1_ERROR));
                } else {
                    char *message = concat(WES_MESSAGE_EMERGENCY, "\n");
                    int messageLength = string_length(WES_MESSAGE_EMERGENCY) + 1;

                    printf("%s", message);
                    fwrite(message, sizeof(char), messageLength, status);
                    write(fd, WES_MESSAGE_EMERGENCY, sizeof(char) * string_length(WES_MESSAGE_EMERGENCY));
                }
            }
        }
    }
    char *message = concat(APPLICATION_ENDED_MESSAGE, "\n");
    int messageLength = string_length(APPLICATION_ENDED_MESSAGE) + 1;

    printf("%s", message);
    fwrite(message, sizeof(char), messageLength, status);
    write(fd, APPLICATION_ENDED_MESSAGE, sizeof(char) * string_length(APPLICATION_ENDED_MESSAGE));

    close(speedPFC1Log);
    close(speedPFC2Log);
    close(speedPFC3Log);
    close(fd);
    fclose(status);

    return 0;
}

double readSpeed(int fd) {
    double speed;
    char buffer[10] = {0};

    readLine(fd, buffer, '\n');
    speed = strtod(buffer, NULL);

    return speed;
}
