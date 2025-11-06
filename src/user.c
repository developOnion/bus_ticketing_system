#include "user.h"
#include "util.h"
#include "validation.h"
#include "booking.h"
#include "bus.h"

User User_register() {
    User newUser;
    do {
        printf("Character allowed: A-Z | a-z | 1-9 | a space\n");
        printf("Enter username: ");
        scanf("%[^\n]c", newUser.username);
        Util_clearInputBuffer();

        if (!Validation_isValidUsername(newUser.username)) {
            printf("\nInvalid username. Please try again\n\n");
            continue;
        }

        if (!User_isUsernameUnique(newUser.username)) {
            printf("\nUsername already exist. Please enter another name.\n\n");
        }
           
    } while (!User_isUsernameUnique(newUser.username) || !Validation_isValidUsername(newUser.username));

    do {
        printf("\nPassword must have\n");
        printf("_ Minimum length 8\n");
        printf("_ Combination of lowercase, uppercase, numbers or symbols.\n");
        printf("Enter Password: ");
        scanf("%[^\n]c", newUser.password);
        Util_clearInputBuffer();

        if (!Validation_isValidPassword(newUser.password)) 
            printf("\nInvalid password, Please try again.\n");
    } while (!Validation_isValidPassword(newUser.password));

    do {
        printf("Enter phonenumber: ");
        scanf("%[^\n]c", newUser.phoneNumber);
        Util_clearInputBuffer();

        if (!Validation_isValidPhonenumber(newUser.phoneNumber))
            printf("\nInvalid phonenumber. Please try again.\n");
    } while (!Validation_isValidPhonenumber(newUser.phoneNumber));

    char previousId[MAX_ID_LEN];
    FILE *scanner_counterFile = NULL;

    scanner_counterFile = fopen(USERS_COUNTER_FILE, "r");
    if (scanner_counterFile == NULL) {
        printf("Registration failed.\n");
        newUser.userID[0] = '\0'; // Mark as invalid
        return newUser;
    }

    fscanf(scanner_counterFile, "%s", previousId);
    sprintf(newUser.userID, "U%03d", atoi(previousId) + 1);

    fclose(scanner_counterFile);

    scanner_counterFile = fopen(USERS_COUNTER_FILE, "w");
    if (scanner_counterFile == NULL) {
        printf("Registration failed.\n");
        newUser.userID[0] = '\0'; // Mark as invalid
        return newUser;
    }

    fprintf(scanner_counterFile, "%d", atoi(previousId) + 1);

    fclose(scanner_counterFile);

    FILE *scanner_userFile = NULL;
    scanner_userFile = fopen(USERS_FILE, "a");
    if (scanner_userFile == NULL) {
        printf("Registration failed.\n");
        newUser.userID[0] = '\0'; // Mark as invalid
        return newUser;
    }

    fprintf(scanner_userFile, "\n%s,%s,%s,%s,%s", newUser.userID, newUser.username, newUser.password, newUser.phoneNumber, "0");
    fclose(scanner_userFile);

    printf("\nUser added sucessfully.\n");
    printf("UserID: %s\n", newUser.userID);
    printf("Username: %s\n", newUser.username);
    printf("Phone Number: %s\n", newUser.phoneNumber);

    return newUser;
}

bool User_isUsernameUnique(char *username) {
    FILE *scanner = NULL;
    scanner = fopen(USERS_FILE, "r");

    if (scanner == NULL) {
        printf("Error: Cannot load data\n");
        fclose(scanner);
        return false;
    }

    // buffer for storing each line in the file
    char line[MAX_LINE_LEN];

    // skip header line
    fgets(line, sizeof(line), scanner);
 
    bool isUnique = true;
    while (fgets(line, sizeof(line), scanner) != NULL) {
        line[strcspn(line, "\n")] = 0;

        char tmpLine[MAX_LINE_LEN];
        strcpy(tmpLine, line);

        char* token;
        int counter_column = 0;

        token = strtok(tmpLine, ",");

        while (token != NULL) {
            counter_column++;

            if (counter_column == 2) {
                if (strcmp(token, username) == 0) // if found exisiting username
                    isUnique = false;
                break;
            }
            
            token = strtok(NULL, ",");
        }

        // stop checking if found
        if (!isUnique) break;
    }

    fclose(scanner);
    return isUnique;
}

User User_login() {
    User user;
    printf("Enter username: ");
    scanf("%[^\n]c", user.username);
    Util_clearInputBuffer();
    
    printf("Enter password: ");
    scanf("%[^\n]c", user.password);
    Util_clearInputBuffer();

    FILE *scanner_userFile = NULL;
    scanner_userFile = fopen(USERS_FILE, "r");
    if (scanner_userFile == NULL) {
        printf("Error: Cannot load data\n.");
        user.userID[0] = '\0';
        printf("Login failed.\n");
        fclose(scanner_userFile);
        return user;
    }
    
    bool foundUsername = false;
    bool isCorrectPassword = false;
    int role;
    char line[MAX_LINE_LEN];

    // skip the heading
    fgets(line, sizeof(line), scanner_userFile);
    
    while (fgets(line, sizeof(line), scanner_userFile) != NULL) {
        line[strcspn(line, "\n")] = 0;

        char tmpLine[MAX_LINE_LEN];
        strcpy(tmpLine, line);

        char* token;
        int counter_column = 0;

        token = strtok(tmpLine, ",");

        while (token != NULL) {
            counter_column++;

            if (counter_column == 1) {
                strcpy(user.userID, token); // userID
            }
            else if (counter_column == 2) {
                if (strcmp(token, user.username) == 0) { // check username
                    foundUsername = true;
                }
            }
            else if (counter_column == 3) {
                if (foundUsername && strcmp(token, user.password) == 0) { // check password
                    isCorrectPassword = true;
                }
            }
            else if (counter_column == 4) {
                strcpy(user.phoneNumber, token); // get phone number
            }
            else if (counter_column == 5) {
                role = atoi(token); // get role
            }

            token = strtok(NULL, ",");
        }

        if (foundUsername && isCorrectPassword) {
            user.role = role;
            break;
        }
    }

    fclose(scanner_userFile);

    if (!foundUsername) {
        printf("\nUsername does not exits.");
        user.userID[0] = '\0';
        return user;
    }
    else if (!isCorrectPassword) {
        printf("\nIncorrect password.");
        user.userID[0] = '\0';
        return user;
    }

    return user;
}

void User_showUserMenu(User *currentUser) {
    int option;
    do {
        printf("\n========== User Menu ==========\n");
        printf("1. Show Available Buses\n");
        printf("2. Book Ticket\n");
        printf("3. View My Bookings\n");
        printf("4. Cancel a Booking\n");
        printf("5. Logout\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &option);
        Util_clearInputBuffer();

        switch (option) {
            case 1:
                User_showAvailableBuses();
                break;
            case 2:
                Booking_bookTicket(currentUser);
                break;
            case 3:
                Booking_viewMyBookings(currentUser);
                break;
            case 4:
                Booking_cancelBooking(currentUser);
                break;
            case 5:
                User_logout(currentUser);
                return;
            case 6:
                Util_exitProgram();
            default:
                printf("Invalid option. Please try again.\n");
        }

    } while (option != 5);
}

void User_logout(User *currentUser) {
    printf("Logging out...\n");
    if (currentUser) {
        currentUser->userID[0] = '\0'; // Mark as logged out
    }
}

void User_showAvailableBuses() {
    printf("\n========== AVAILABLE BUSES ==========\n");

    char checkDate[MAX_DATE_LEN];
    do {
        printf("\nValid booking date now -> one year from now\n");
        printf("Enter the date you want to check (YYYY-MM-DD): ");
        scanf("%[^\n]c", checkDate);
        Util_clearInputBuffer();
        
        if (!Validation_isValidDate(checkDate)) {
            printf("Invalid date.\n");
        }
    } while (!Validation_isValidDate(checkDate));

    Bus availableBues[MAX_BUS_COUNT];
    int count = Booking_getAvailableBuses(checkDate, availableBues);
    
    for (int i = 0; i < count; i++) {
        printf("\n(%d) %s(%s)\n", i + 1, availableBues[i].name, availableBues[i].busID);
        printf("Departure From: %s\n", availableBues[i].origin);
        printf("Going To: %s\n", availableBues[i].destination);
        printf("Departure Time: %s\n", availableBues[i].departureTime);
        printf("Total Seats: %d\n", availableBues[i].totalSeats);
        printf("Available Seats: %d\n", availableBues[i].availableSeats);
    }
}
