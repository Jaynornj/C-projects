#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>

#define BUFFER_SIZE 256



typedef struct {
    int operation_date; // days offset from the current date
    int start_time; // in minutes from midnight
    int end_time; // in minutes from midnight
    int total_minutes;
    int sequence;
    int skipped; // 0 = false, 1 = true
    int state; // 0 = off, 1 = on
} Zone;

typedef struct {
    struct tm current_datetime;
    float rain_sensor_value;
    float rain_sensor_limit; // User-defined rain sensor limit
    int num_zones;
    Zone *zones;
} Controller;

// Function prototypes
void displayMenu();
void clearAndDisplayTime(Controller *controller);
void setDateTime(Controller *controller);
void selectNumZones(Controller *controller);
void configureZones(Controller *controller);
void setRainSensor(Controller *controller);
void scheduleOperation(Controller *controller);
void resetSystem(Controller *controller);
void loadDefaultSettings(Controller *controller);
char *input(char *buffer, int size);
bool validateDigits(const char *input);

int main() {
    Controller controller;
    loadDefaultSettings(&controller);
    char buffer[BUFFER_SIZE];
    int option;

    do {
        // Display menu
        printf("\n\nMenu:\n");
        printf("1 - Set date and time\n");
        printf("2 - Select number of zones\n");
        printf("3 - Configure operation of all zones\n");
        printf("4 - Set the rain sensor limits\n");
        printf("5 - Schedule the operation of zones\n");
        printf("6 - Reset system to default settings\n");
        printf("7 - Exit program\n");
        printf("Enter your choice (1-7): ");

        input(buffer, BUFFER_SIZE);

        if (!validateDigits(buffer)) {
            printf("Please enter digits.\n\n");
            continue;
        }

        sscanf(buffer, "%d", &option);

        switch (option) {
            case 1:
                setDateTime(&controller);
                break;
            case 2:
                selectNumZones(&controller);
                break;
            case 3:
                configureZones(&controller);
                break;
            case 4:
                setRainSensor(&controller);
                break;
            case 5:
                scheduleOperation(&controller);
                break;
            case 6:
                resetSystem(&controller);
                break;
            case 7:
                printf("Exiting the program.\n");
                return 0;
            default:
                printf("Invalid choice. Please keep between option 1 and 7.\n");
                break;
        }
    } while (option != 7);

    // Free memory
    free(controller.zones);

    return 0;
}

void clearAndDisplayTime(Controller *controller) {
    system("cls"); // use clear for unix OS
    char buffer[80];
    time_t current_time = time(NULL);
    struct tm *timeinfo = localtime(&current_time);
    strftime(buffer, sizeof(buffer), "%c", timeinfo);
    printf("Current system date and time: %s\n\n", buffer);
}

void setDateTime(Controller *controller) {
    struct tm newTime;
    time_t timeoftheday;

    // Set a hardcoded date and time
    newTime.tm_year = 2024 - 1900; // Year 2024
    newTime.tm_mon = 1 - 1;        // January (month is 0-indexed, 0 for January)
    newTime.tm_mday = 1;           // 1st day of the month
    newTime.tm_hour = 12;          // 12:00 PM
    newTime.tm_min = 30;           // 30 minutes
    newTime.tm_sec = 30;           // 30 seconds
    newTime.tm_isdst = 0;          // Daylight saving time is not in effect

    // Make the time based on the tm structure
    timeoftheday = mktime(&newTime);

    // Set the current_datetime of the controller to newTime
    controller->current_datetime = newTime;

    // Clear the screen and display the newly set time
    clearAndDisplayTime(controller);
    printf("Date and time set to: %s\n", ctime(&timeoftheday));
}

void selectNumZones(Controller *controller) {
    char buffer[BUFFER_SIZE];
    printf("Enter the number of zones: ");
    input(buffer, BUFFER_SIZE);
    if (!validateDigits(buffer)) {
        printf("Please enter digits.\n\n");
        return;
    }while (!validateDigits(buffer));
    sscanf(buffer, "%d", &controller->num_zones);


    // Free the existing zones array if it already exists
    if (controller->zones != NULL) {
        free(controller->zones);
    }

    // Allocate memory using malloc as per the new number of zones
    controller->zones = (Zone *)malloc(controller->num_zones * sizeof(Zone));
    if (controller->zones == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE); // Use EXIT_FAILURE for portability
    }

    // Initialize attributes for each zone
    for (int i = 0; i < controller->num_zones; i++) {
        controller->zones[i].operation_date = 0; // Default setting
        controller->zones[i].start_time = 0;     // Default setting
        controller->zones[i].end_time = 0;       // Default setting
        controller->zones[i].total_minutes = 0;  // Default setting
        controller->zones[i].sequence = i + 1;   // Sequencing starts at 1
        controller->zones[i].skipped = 0;        // Initially not skipped
        controller->zones[i].state = 0;          // Initially off
    }

    // Clear and display current time after updating zones
    clearAndDisplayTime(controller);
}

void configureZones(Controller *controller) {
    char buffer[BUFFER_SIZE];
    for (int i = 0; i < controller->num_zones; i++) {
        printf("Configuring zone %d:\n", i + 1);

        printf("Enter operation date (days from today): ");
        input(buffer, BUFFER_SIZE);
        if (!validateDigits(buffer)) {
            printf("Please enter digits.\n\n");
            return;
        }while (!validateDigits(buffer));
        sscanf(buffer, "%d", &controller->zones[i].operation_date);

        printf("Enter start time (minutes from midnight): ");
        input(buffer, BUFFER_SIZE);
        if (!validateDigits(buffer)) {
            printf("Please enter digits.\n\n");
            return;
        }while (!validateDigits(buffer));
        sscanf(buffer, "%d", &controller->zones[i].start_time);

        printf("Enter duration (minutes): ");
        input(buffer, BUFFER_SIZE);
        if (!validateDigits(buffer)) {
            printf("Please enter digits.\n\n");
            return;
        }while (!validateDigits(buffer));
        sscanf(buffer, "%d", &controller->zones[i].total_minutes);

        controller->zones[i].end_time = controller->zones[i].start_time + controller->zones[i].total_minutes;
    }
    clearAndDisplayTime(controller);
}

void setRainSensor(Controller *controller) {
    char buffer[BUFFER_SIZE];
    printf("Enter the rain sensor limit (in inches, 0 to 2): "); // User-defined rain sensor limit
    input(buffer, BUFFER_SIZE);
    if (!validateDigits(buffer)) {
        printf("Please enter digits.\n\n");
        return;
    }while (!validateDigits(buffer));
    sscanf(buffer, "%f", &controller->rain_sensor_limit);

    // Generate a random rain sensor value between 0 and 2 inches
    srand(time(NULL));
    controller->rain_sensor_value = (float)(rand() % 21) / 10.0; // Random value between 0 and 2 with 0.1 increment
    printf("Rain sensor value set to: %.1f inches\n", controller->rain_sensor_value);
}

void scheduleOperation(Controller *controller) {

    // Display current rain sensor state
    printf("Rain sensor state: %.1f inches\n", controller->rain_sensor_value);

    // Display operation of all zones
    printf("Zone Operation:\n");

    // Determine which zone to activate (example: activate the first zone)
    int activeZoneIndex = 0;
    // Ensure the active zone index is within the valid range
    if (activeZoneIndex >= 0 && activeZoneIndex < controller->num_zones) {
        // Set the state of the selected zone to active (1)
        controller->zones[activeZoneIndex].state = 1;
    }

    for (int i = 0; i < controller->num_zones; i++) {
        printf("Zone %d: ", i + 1);
        if (i == activeZoneIndex) {
            // Display the active zone details
            printf("Active\n");
            printf("Operation Date: %d\n", controller->zones[i].operation_date);
            printf("Start Time: %d minutes\n", controller->zones[i].start_time);
            printf("End Time: %d minutes\n", controller->zones[i].end_time);
            printf("Total Minutes: %d\n", controller->zones[i].total_minutes);
            printf("Sequence: %d\n", controller->zones[i].sequence);
            printf("Skipped: %d\n", controller->zones[i].skipped);
        } else {
            // Display inactive for other zones
            printf("Inactive\n");
        }
    }

    // Wait for user input to continue
    printf("\nPress Enter to return to the main menu.");
    getchar(); // Wait for Enter key press
    clearAndDisplayTime(controller);
}

void resetSystem(Controller *controller) {
    free(controller->zones);
    loadDefaultSettings(controller);
    printf("System reset to default settings.\n");
    clearAndDisplayTime(controller);
}


void loadDefaultSettings(Controller *controller) {
    controller->num_zones = 5; // default number of zones
    controller->rain_sensor_limit = 2.0; // default rain sensor limit
    controller->rain_sensor_value = 0.0; // no rain by default
    controller->zones = (Zone *)malloc(controller->num_zones * sizeof(Zone));
    for (int i = 0; i < controller->num_zones; i++) {
        controller->zones[i] = (Zone) {.operation_date = 0, .start_time = 480 +
                                                                          i * 30, .total_minutes = 30, .sequence = i + 1};
        controller->zones[i].end_time = controller->zones[i].start_time + controller->zones[i].total_minutes;
    }
    clearAndDisplayTime(controller);
}

char *input(char *buffer, int size) {
    if (fgets(buffer, size, stdin)) {
        buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline character
    }
    return buffer;
}

#include <stdbool.h>

bool validateDigits(const char *input) {
    for (int i = 0; input[i] != '\0'; i++) {
        if (!isdigit(input[i])) {
            return false;
        }
    }
    return true;
}
