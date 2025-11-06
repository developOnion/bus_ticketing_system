#include "bus.h"
#include "validation.h"
#include "util.h"

void Bus_addBus() {
    Bus newBus;
    
    FILE *scanner_counterFile = fopen(BUSES_COUNTER_FILE, "r");
    if (scanner_counterFile == NULL) {
        printf("Error: Cannot load data.\n");
        fclose(scanner_counterFile);
        return;
    }

    char previousBusID[MAX_ID_LEN];

    fscanf(scanner_counterFile, "%s", previousBusID);
    sprintf(newBus.busID, "BUS%03d", atoi(previousBusID) + 1);

    fclose (scanner_counterFile);

    scanner_counterFile = fopen(BUSES_COUNTER_FILE, "w");
    if (scanner_counterFile == NULL) {
        printf("Error: Cannot load data.\n");
        fclose(scanner_counterFile);
        return;
    }
    fprintf(scanner_counterFile, "%d", atoi(previousBusID) + 1);
    fclose(scanner_counterFile);

    FILE *scanner_busesFile = fopen(BUSES_FILE, "a");
    if (scanner_busesFile == NULL) {
        printf("Error: Cannot load data.\n");
        return;
    }
    
    printf("Enter Bus Name: ");
    scanf("%[^\n]c", newBus.name);
    Util_clearInputBuffer();

    do {
        printf("Enter Origin: ");
        scanf("%[^\n]c", newBus.origin);
        Util_clearInputBuffer();

        printf("Enter Destination: ");
        scanf("%[^\n]c", newBus.destination);
        Util_clearInputBuffer();

        if (!Validation_isValidRoute(newBus.destination, newBus.origin)) {
            printf("Invalid route. It must me different and not empty\n");
        }

    } while (!Validation_isValidRoute(newBus.destination, newBus.origin));
    

    do {
        printf("Enter Departure Time (HH:MM)24 hours format: ");
        scanf("%s", newBus.departureTime);

        if (!Validation_isValidTimeFormat(newBus.departureTime)) {
            printf("Invalid time format. Please use HH:MM format.\n");
        }
    } while (!Validation_isValidTimeFormat(newBus.departureTime));

    do {
        printf("Enter Total Seats: ");
        scanf("%d", &newBus.totalSeats);
        Util_clearInputBuffer();

        if (Validation_isValidSeats(newBus.totalSeats) == false) {
            printf("Invalid number of seats. It must be a positive integer.\n");
        }
    } while (Validation_isValidSeats(newBus.totalSeats) == false);
    

    char route[MAX_LOCATION_LEN * 2 + 2];
    sprintf(route, "%s-%s", newBus.origin, newBus.destination);

    fprintf(scanner_busesFile, "%s,%s,%s,%s,%d\n", 
            newBus.busID, newBus.name, route, 
            newBus.departureTime, newBus.totalSeats);

    fclose(scanner_busesFile);
    printf("New bus added successfully.\n");
}

void Bus_viewBuses() {
    printf("\n========== ALL BUSES ==========\n");
    FILE *scanner_busesFile = fopen(BUSES_FILE, "r");
    if (scanner_busesFile == NULL) {
        printf("Error: Cannot load data.\n");
        return;
    }

    char line[MAX_LINE_LEN];
    bool hasBuses = false;

    // skip heading line
    fgets(line, sizeof(line), scanner_busesFile);

    while (fgets(line, sizeof(line), scanner_busesFile)) {
        hasBuses = true;
        char busID[MAX_ID_LEN], name[MAX_NAME_LEN], origin[MAX_LOCATION_LEN], 
             destination[MAX_LOCATION_LEN], departureTime[MAX_DEPARTURE_TIME_LEN];
        int totalSeats;

        int parsed = sscanf(line, "%[^,],%[^,],%[^-]-%[^,],%[^,],%d", 
            busID, name, origin, destination, departureTime, &totalSeats);
        if (parsed != 6) {
            printf("Warning: Could not parse bus info: %s\n", line);
            continue;
        }

        printf("\nBus ID: %s\n", busID);
        printf("Name: %s\n", name);
        printf("Origin: %s\n", origin);
        printf("Destination: %s\n", destination);
        printf("Departure Time: %s\n", departureTime);
        printf("Total Seats: %d\n", totalSeats);
    }

    fclose(scanner_busesFile);

    if (!hasBuses) {
        printf("No buses available.\n");
    }
}

void Bus_deleteBus() {
    char busID[MAX_ID_LEN];
    printf("Enter Bus ID to delete: ");
    scanf("%s", busID);
    Util_clearInputBuffer();

    FILE *scanner_busesFile = fopen(BUSES_FILE, "r");
    if (scanner_busesFile == NULL) {
        printf("Error: Cannot load data.\n");
        return;
    }

    FILE *tempFile = fopen("temp_buses.txt", "w");
    if (tempFile == NULL) {
        fclose(scanner_busesFile);
        printf("Error: Cannot create temporary file.\n");
        return;
    }

    char line[MAX_LINE_LEN];
    bool found = false;

    // skip heading line
    fgets(line, sizeof(line), scanner_busesFile);
    fprintf(tempFile, "%s", line);

    while (fgets(line, sizeof(line), scanner_busesFile)) {
        char currentBusID[MAX_ID_LEN];
        sscanf(line, "%[^,]", currentBusID);

        if (strcmp(currentBusID, busID) == 0) {
            found = true;
            continue; // Skip this bus
        }
        fprintf(tempFile, "%s", line);
    }

    fclose(scanner_busesFile);
    fclose(tempFile);

    if (!found) {
        printf("Bus with ID %s not found.\n", busID);
        remove("temp_buses.txt");
        return;
    }

    remove(BUSES_FILE);
    rename("temp_buses.txt", BUSES_FILE);
    
    printf("Bus with ID %s deleted successfully.\n", busID);
}

void Bus_editBusDepartureTime() {
    printf("Enter Bus ID to edit departure time: ");
    char busID[MAX_ID_LEN];
    scanf("%s", busID);
    Util_clearInputBuffer();

    FILE *scanner_busesFile = fopen(BUSES_FILE, "r");
    if (scanner_busesFile == NULL) {
        printf("Error: Cannot load data.\n");
        return;
    }

    FILE *tempFile = fopen("temp_buses.txt", "w");
    if (tempFile == NULL) {
        fclose(scanner_busesFile);
        printf("Error: Cannot create temporary file.\n");
        return;
    }

    char line[MAX_LINE_LEN];
    bool found = false;

    fgets(line, sizeof(line), scanner_busesFile);
    fprintf(tempFile, "%s", line);

    while (fgets(line, sizeof(line), scanner_busesFile)) {
        char currentBusID[MAX_ID_LEN], departureTime[MAX_DEPARTURE_TIME_LEN], busName[MAX_NAME_LEN],
             origin[MAX_LOCATION_LEN], destination[MAX_LOCATION_LEN];
        int totalSeats;
        int parsed = sscanf(line, "%[^,],%[^,],%[^-]-%[^,],%[^,],%d",
            currentBusID, busName, origin, destination, departureTime, &totalSeats);

        if (parsed != 6) {
            printf("Warning: Could not parse bus info: %s\n", line);
            remove("temp_buses.txt");
            fclose(scanner_busesFile);
            fclose(tempFile);
            return;
        }

        if (strcmp(currentBusID, busID) == 0) {
            found = true;
            char newDepartureTime[MAX_DEPARTURE_TIME_LEN];
            do {
                printf("\nCurrent Departure Time: %s\n", departureTime);
                printf("Enter New Departure Time (HH:MM)24 hours format: ");
                scanf("%s", newDepartureTime);
                Util_clearInputBuffer();

                if (!Validation_isValidTimeFormat(newDepartureTime)) {
                    printf("Invalid time format. Please use HH:MM format.\n");
                }
                else if (strcmp(newDepartureTime, departureTime) == 0) {
                    printf("New departure time cannot be the same as the current one.\n");
                }
            } while (!Validation_isValidTimeFormat(newDepartureTime) || strcmp(newDepartureTime, departureTime) == 0);

            fprintf(tempFile, "%s,%s,%s-%s,%s,%d\n", 
                    currentBusID, busName, origin, destination, newDepartureTime, totalSeats);
            printf("Departure time updated successfully for Bus ID %s.\n", busID);
        } else {
            fprintf(tempFile, "%s", line);
        }
    }

    fclose(scanner_busesFile);
    fclose(tempFile);

    if (found) {
        remove(BUSES_FILE);
        rename("temp_buses.txt", BUSES_FILE);
    } 
    else {
        remove("temp_buses.txt");
        printf("Bus with ID %s not found.\n", busID);
    }
}
