#include "admin.h"
#include "util.h"
#include "global.h"
#include "bus.h"

void Admin_showAdminMenu(User *currentUser) {
    int option;
    do {
        printf("\n========== Admin Menu ==========\n");
        printf("1. Manage buses\n");
        printf("2. View all booking\n");
        printf("3. Log out\n");
        printf("4. Exit\n");
        printf("Enter option: ");
        scanf("%d", &option);
        Util_clearInputBuffer();

        switch (option) {
            case 1:
                Admin_manageBuses();
                break;
            case 2:
                Admin_viewAllBookings();
                break;
            case 3:
                Admin_Logout(currentUser);
                return;
            case 4:
                Util_exitProgram();  
            default:
                printf("Invalid option.\n");
        }

    } while (option != 3 && option != 4);
}   

void Admin_manageBuses() {
    int option;
    do {
        printf("\n========== Manage Buses ==========\n");
        printf("1. Add Bus\n");
        printf("2. Delete Bus\n");
        printf("3. Edit Bus Departure Time\n");
        printf("4. View All Buses\n");
        printf("5. Back to Admin Menu\n");
        printf("Enter option: ");
        scanf("%d", &option);
        Util_clearInputBuffer();

        switch (option) {
            case 1:
                Bus_addBus();
                break;
            case 2:
                Bus_deleteBus();
                break;
            case 3:
                Bus_editBusDepartureTime();
                break;
            case 4:
                Bus_viewBuses();
                break;
            case 5:
                return; // Back to Admin Menu
            default:
                printf("Invalid option.\n");
        }
    } while (option != 5);
}

void Admin_viewAllBookings() {
    printf("\n========== ALL BOOKING ==========\n");

    FILE *scanner_bookingsFile = fopen(BOOKING_FILE, "r");
    if (scanner_bookingsFile == NULL) {
        printf("Error: Cannot open file\n");
        return;
    }

    char line[MAX_LINE_LEN];
    bool hasBookings = false;

    // skip heading line
    fgets(line, sizeof(line), scanner_bookingsFile);

    while (fgets(line, sizeof(line), scanner_bookingsFile) != NULL) {
        hasBookings = true;
        char bookingID[MAX_ID_LEN], userID[MAX_ID_LEN], busID[MAX_ID_LEN],
             seatNumber[MAX_ID_LEN], passengerName[MAX_NAME_LEN], date[MAX_DATE_LEN],
             contact[MAX_PHONENUMBER_LEN];

        int parsed = sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,\n]", 
               bookingID, userID, busID, seatNumber, passengerName, contact, date);
        if (parsed != 7) {
            printf("Warning: Could not parse booking info: %s\n", line);
            continue;
        }

        printf("\nBooking ID: %s\n", bookingID);
        printf("User ID: %s\n", userID);
        printf("Bus ID: %s\n", busID);
        printf("Seat Number: %s\n", seatNumber);
        printf("Passenger Name: %s\n", passengerName);
        printf("Contact: %s\n", contact);
        printf("Departure Date: %s\n", date);
    }

    fclose(scanner_bookingsFile);

    if (!hasBookings) {
        printf("No bookings found.\n");
    }
}

void Admin_Logout(User *currentUser) {
    printf("\nLogging out...\n");
    if (currentUser) {
        currentUser->userID[0] = '\0'; // Mark as logged out
    }
}