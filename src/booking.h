#ifndef BOOKING_H
#define BOOKING_H

#include <stdio.h>

#include "bus.h"
#include "user.h"

// booking struct
typedef struct Booking {
    char bookingID[MAX_ID_LEN];
    char userID[MAX_ID_LEN];
    char busID[MAX_ID_LEN];
    int seatNumber;
    char passengerName[MAX_NAME_LEN];
    char contact[MAX_PHONENUMBER_LEN];
    char departureDate[MAX_DATE_LEN];
} Booking;

void Booking_bookTicket(User *currentUser);
void Booking_viewMyBookings(User *currentUser);
void Booking_cancelBooking(User *currentUser);
int Booking_getAvailableBuses(const char *date, Bus buses[]);
void Booking_printAvailableSeats(const Bus *bus, const char *date);
bool Booking_isSeatAvailable(const char *busID, const char *date, int seatNumber);

#endif