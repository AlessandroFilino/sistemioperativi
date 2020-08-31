#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <string.h>
#include "../include/generatoreFallimenti.h"
#include "../include/utility.h"
#include "../include/path.h"

int main(int argc, const char * argv[]) {
    /*
     * TODO usare strtol invece che atoi
     * TODO leggere il nuovo pid dalla pipe del pfcDisconnetedSwitch
     * TODO inserire le istruzione del generatoreFallimenti in un ciclo
     *      while che termina quando dalla pipe del pfcDisconnetedSwitch
     *      si riceve "terminated"
     */

    srand(time(NULL));

    //int signals[4] = {SIGSTOP, SIGINT, SIGCONT, SIGUSR1};
    int pfcProcess[3] = {atoi(argv[1]), atoi(argv[2]), atoi(argv[3])};

    FILE *failures = fopen(FILENAME_FAILURES_LOG, "w");
    
    sleep(1);
    
    int pfc = rand() % 3;
    unsigned char fallimenti = calcoloProb();
    
    if (fallimenti & 1){
        /*
         * SIGSTOP = sospensione da dentro un programma
         */
        kill(pfcProcess[pfc], SIGSTOP);
    }
    
    if (fallimenti & 2){
        /*
         * SIGINT = quando l'utente digits ctrl-c
         */
        kill(pfcProcess[pfc], SIGINT);
    }
    
    if (fallimenti & 4) {
        /*
         * SIGCONT = riprende l'esecuzione di un programma dopo la sospensione
         */
        kill(pfcProcess[pfc], SIGCONT);
    }
    
    if (fallimenti & 8){
        /*
         * SIGUSR1 = segnale definito dall'utente
         */
        kill(pfcProcess[pfc], SIGUSR1);
    }
 
}

unsigned char calcoloProb() {
    unsigned char prob = 0;
    int value = rand();

    if((value % PROB_TOT) < (PROB_SIGSTOP * PROB_TOT)) {
        prob |= 1;
    }

    if((value % PROB_TOT) < (PROB_SIGINT * PROB_TOT)) {
        prob |= 2;
    }

    if((value % PROB_TOT) < (PROB_SIGCONT * PROB_TOT)) {
        prob |= 4;
    }

    if((value % PROB_TOT) < (PROB_SIGUSR1 * PROB_TOT)) {
        prob |= 8;
    }

    return prob;
}
