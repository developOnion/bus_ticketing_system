#ifndef BUS_H
#define BUS_H

#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>

#include "global.h"

// bus struct
typedef struct Bus {
    char busID[MAX_ID_LEN];
    char name[MAX_NAME_LEN];
    char origin[MAX_LOCATION_LEN];
    char destination[MAX_LOCATION_LEN];
    char departureTime[MAX_DEPARTURE_TIME_LEN];
    int totalSeats;
    int availableSeats;
} Bus;

void Bus_addBus();
void Bus_deleteBus();
void Bus_viewBuses();
void Bus_editBusDepartureTime();

#endif