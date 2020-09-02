#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include "../include/utility.h"
#include "../include/pfc.h"
#include "../include/path.h"
#include "../include/messages.h"

enum boolean PFC1_sigUsr;
enum boolean PFC1_sigRestart;
void signalHandler(int signal);

int main(int argc, const char * argv[]) {
    int fd_pipe;
    int last_read;
    ssize_t read = 0;
    double previousLatitude = 0;
    double previousLongitude = 0;

    signal(SIGUSR1, &signalHandler);
    signal(SIGSTOP, &signalHandler);

    //TODO char *filename_g18 = argv[1];
    char *filename_g18 = "../doc/G18.txt";
    FILE *fp_g18 = openFile(filename_g18, "r");

    last_read = open(FILENAME_LAST_READ, O_CREAT | O_RDWR);
    changePointerPosition(fp_g18, last_read);

    //TODO unlink va rimosso
    unlink(FILENAME_PFC1_PIPE);
    fd_pipe = connectPipe(FILENAME_PFC1_PIPE, O_WRONLY);

    read = setPreviousGeographicCoordinates(fp_g18, &previousLatitude, &previousLongitude);

    while(read != -1) {
        //TODO usare sleep(1)
        usleep((1 * 1000) * 1000); //1000 millisecondi

        read = exe(fd_pipe, fp_g18, last_read, &previousLatitude, &previousLongitude, &PFC1_sigUsr, &PFC1_sigRestart);
    }

    write(fd_pipe, APPLICATION_ENDED_MESSAGE, string_length(APPLICATION_ENDED_MESSAGE));

    fclose(fp_g18);
    close(fd_pipe);

    return 0;
}

void signalHandler(int signal) {
    setSignalStatus(signal, &PFC1_sigUsr, &PFC1_sigRestart);
}






