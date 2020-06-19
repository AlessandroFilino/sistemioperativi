//
//  pfcDisconnectedSwitch.h
//  sistemiOperativi
//
//  Created by Alessandro Filino on 26/05/2020.
//  Copyright © 2020 Alessandro Filino. All rights reserved.
//

#ifndef pfcDisconnectedSwitch_h
#define pfcDisconnectedSwitch_h

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define READ 0
#define WRITE 1

char* intToString(int number);
int creaFiglio(int* pfcProcess);

#endif /* pfcDisconnectedSwitch_h */