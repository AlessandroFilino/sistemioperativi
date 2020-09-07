#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include "../include/utility.h"
#include "../include/pfc.h"
#include "../include/path.h"
#include "../include/messages.h"

enum boolean PFC1_sigUsr;
enum boolean PFC1_sigRestart;
void signalHandler(int signal);

int main(int argc, const char * argv[]) {
    ssize_t numberOfCharsRead = 0;
    double previousLatitude = 0;
    double previousLongitude = 0;

    signal(SIGUSR1, &signalHandler);
    signal(SIGSTOP, &signalHandler);

    //TODO char *filename_g18 = argv[1];
    char *filename_g18 = "../doc/G18.txt";
    FILE *fp_g18 = openFile(filename_g18, "r");

    /*
     * last_read.txt va aperto in "r+" e non in "w+"
     * perchè altrimenti viene cancellato il contenuto
     * del file se il file è già esistente (non andrebbe bene
     * perchè se un processo viene rimosso e pfcDisconnectedSwitch lo
     * ricrea, a quel punto il processo, aprendo last_read.txt in "w+"
     * cancellerebbe il contenuto e non potrebbe riprendere a leggere
     * dall'ultima lettura)
     */
    FILE *lastRead = openFile(FILENAME_LAST_READ, "r+");
    changePointerPosition(fp_g18, lastRead);

    //TODO unlink va rimosso
    //unlink(FILENAME_PFC1_PIPE);
    int transducersPipe = connectPipe(FILENAME_PFC1_PIPE, O_WRONLY);

    numberOfCharsRead = setPreviousGeographicCoordinates(fp_g18, &previousLatitude, &previousLongitude);

    while(numberOfCharsRead != -1) {
        //TODO usare sleep(1)
        usleep((1 * 1000) * 100); //1000 millisecondi = 1 secondo

        numberOfCharsRead = exe(transducersPipe, fp_g18, lastRead, &previousLatitude, &previousLongitude, &PFC1_sigUsr, &PFC1_sigRestart);
    }

    char message[] = concat(APPLICATION_ENDED_MESSAGE, "\n");
    int messageLength = string_length(APPLICATION_ENDED_MESSAGE) + 1;

    write(transducersPipe, message, sizeof(char) * messageLength);
    printf("%s\n", message);

    fclose(fp_g18);
    fclose(lastRead);
    close(transducersPipe);

    return 0;
}

void signalHandler(int signal) {
    setSignalStatus(signal, &PFC1_sigUsr, &PFC1_sigRestart);
}






