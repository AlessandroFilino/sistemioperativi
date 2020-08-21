#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "../include/utility.h"
#include "../include/main.h"

//TODO eliminare header main

int main(int argc, const char* argv[]) {
    /*
     * argv[1] --> g18.txt
     */

    char command1[100] = {0}; //86 caratteri utilizzati
    sprintf(command1, COMMAND_REMOVE_FILES, PATHNAME_TEMP, PATHNAME_TEMP);

    char command2[100] = {0}; //84 caratteri utilizzati
    sprintf(command2, COMMAND_REMOVE_FILES, PATHNAME_LOG, PATHNAME_LOG);

    if(system(command1) || system(command2) != 0) {
        fprintf(stderr, "%s", "main: path error\n");
        exit(EXIT_FAILURE);
    }

    int status;
    char *filename_G18 = "../sistemioperativi/doc/G18.txt";

    if(access(filename_G18, F_OK) == -1) {
        char error[50];
        sprintf(error, "main: File %s not found\n", filename_G18);
        fprintf(stderr, "%s", error);
        exit(EXIT_FAILURE);
    }

    printf("%c", command2[84]);

    /*char *pfcDisconnectedSwitch_argv[] = {"pfcDisconnectedSwitch", filename_G18, NULL};
    createChild(&execv, "pfcDisconnectedSwitch", pfcDisconnectedSwitch_argv);

    char *transducer_argv[] = {"transducer", NULL};
    createChild(&execv, "transducer", transducer_argv);

    char *wes_argv[] = {"wes", NULL};
    createChild(&execv, "wes", wes_argv);

    for(int i=0; i<2; i++) {
        wait(&status);
    }*/

    return 0;
}
