#include <stdio.h>
#include <stdlib.h> //  for malloc
#include <string.h> //  for strcspn
#include <time.h> // for srand
#include <stdbool.h> // for bool type
#include <ctype.h> // for isdigit

#define BUFFER_SIZE 256     // Buffer size for input


// Struct Zone... WELCOME TO THE FINAL PROJECT!
typedef struct {
    int operation_date;                            // days offset from the current date
    int start_time;                                // in minutes from midnight
    int end_time;                                  // in minutes from midnight
    int total_minutes;                            // duration in minutes
    int sequence;                                   // sequence number
    int skipped;                                   // 0 = false, 1 = true
    int state;                                    // 0 = off, 1 = on
} Zone;
typedef struct {                                    // Controller structure
    struct tm current_datetime;                     // current date and time
    float rain_sensor_value;                        // current rain sensor value
    float rain_sensor_limit;                        // User-defined rain sensor limit
    int num_zones;                                 // number of zones
    Zone *zones;                                    // array of zones
} Controller;

// Function prototypes
void clearAndDisplayTime(Controller *controller);   // Clear the screen and display the current time
void setDateTime(Controller *controller);           // Set the date and time
void selectNumZones(Controller *controller);        // Select the number of zones
void configureZones(Controller *controller);        // Configure the operation of all zones
void setRainSensor(Controller *controller);         // Set the rain sensor limits
void scheduleOperation(Controller *controller);     // Schedule the operation of zones
void resetSystem(Controller *controller);           // Reset system to default settings
void loadDefaultSettings(Controller *controller);   // Load default settings
char *input(char *buffer, int size);               // Input function
bool validateDigits(const char *input);            // Validate digits



// Main function
int main() {
    Controller controller;                      // Declare a controller structure
    loadDefaultSettings(&controller);           // Load default settings
    char buffer[BUFFER_SIZE];                  // Buffer for input
    int option;                                // User option

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

        input(buffer, BUFFER_SIZE);                        // Get user input

        if (!validateDigits(buffer)) {                    // Validate input
            printf("Please enter digits.\n\n");
            continue;
        }

        sscanf(buffer, "%d", &option);         // Convert input to integer

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
    } while (option != 7);                          // Exit the loop if option is 7


    free(controller.zones);                 // Free the memory allocated for zones

    return 0;
}


// Clear the screen and display the current time
void clearAndDisplayTime(Controller *controller) {
    system("cls"); // should work as well for unix os, but I haven't tried it
    char buffer[80];
    time_t current_time = time(NULL);                                      // Get the current time
    struct tm *timeinfo = localtime(&current_time);                        // Convert to local time
    strftime(buffer, sizeof(buffer), "%c", timeinfo); // Format the time
    printf("Current system date and time: %s\n\n", buffer);             // Display the time
}


// Set the date and time
void setDateTime(Controller *controller) {
    struct tm newTime;      // New time structure
    time_t timeoftheday;    // Time in seconds

    // Set a hardcoded date and time
    newTime.tm_year = 2024 - 1900; // Year 2024
    newTime.tm_mon = 1 - 1;        // January (month is 0-indexed, 0 for January)
    newTime.tm_mday = 1;           // 1st day of the month
    newTime.tm_hour = 12;          // 12:00 PM
    newTime.tm_min = 30;           // 30 minutes
    newTime.tm_sec = 30;           // 30 seconds
    newTime.tm_isdst = 0;

    // Make the time based on the tm structure
    timeoftheday = mktime(&newTime);

    // Set the current_datetime of the controller to newTime
    controller->current_datetime = newTime;

    // Clear the screen and display the newly set time
    clearAndDisplayTime(controller);
    printf("Date and time set to: %s\n", ctime(&timeoftheday));
}


// Select the number of zones
void selectNumZones(Controller *controller) {
    char buffer[BUFFER_SIZE];

    do {
    printf("Enter the number of zones: ");
    input(buffer, BUFFER_SIZE);                    // Get user input
    if (!validateDigits(buffer)) {                      // Validate input
        printf("Please enter digits.\n\n");
        }
    }while (!validateDigits(buffer));                   // Repeat the loop if the input is invalid
    sscanf(buffer, "%d", &controller->num_zones);

    if (controller->zones != NULL) {                    // Check if zones is not NULL
        free(controller->zones);                // Free the memory allocated for zones
    }

    // Allocate memory using malloc as per the new number of zones
    controller->zones = (Zone *)malloc(controller->num_zones * sizeof(Zone));
    if (controller->zones == NULL) {
        fprintf(stderr, "Memory allocation failed\n");  // Print error message
        exit(EXIT_FAILURE); // Exit the program
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


// Configure the operation of all zones
void configureZones(Controller *controller) {
    char buffer[BUFFER_SIZE];
    for (int i = 0; i < controller->num_zones; i++) {
        printf("Configuring zone %d:\n", i + 1);


        do {
        printf("Enter operation date (days from today): ");
        input(buffer, BUFFER_SIZE);
        if (!validateDigits(buffer)) {
            printf("Please enter digits.\n\n");
        }
        }while (!validateDigits(buffer));
        sscanf(buffer, "%d", &controller->zones[i].operation_date); // Set the operation date

        do {
        printf("Enter start time (minutes from midnight): ");
        input(buffer, BUFFER_SIZE);
        if (!validateDigits(buffer)) {
            printf("Please enter digits.\n\n");
            }
        }while (!validateDigits(buffer));
        sscanf(buffer, "%d", &controller->zones[i].start_time); //  Set the start time


        do {
        printf("Enter duration (minutes): ");
        input(buffer, BUFFER_SIZE);
        if (!validateDigits(buffer)) {
            printf("Please enter digits.\n\n");
            }
        }while (!validateDigits(buffer));
        sscanf(buffer, "%d", &controller->zones[i].total_minutes);  // Set the duration

        controller->zones[i].end_time = controller->zones[i].start_time + controller->zones[i].total_minutes;
    }                                                               //  Set the end time
    clearAndDisplayTime(controller);
}


// Set the rain sensor limits
void setRainSensor(Controller *controller) {
    char buffer[BUFFER_SIZE];
    do{
    printf("Enter the rain sensor limit (in inches, 0 to 2): "); // rain sensor limit
    input(buffer, BUFFER_SIZE); // Get user input
    if (!validateDigits(buffer)) {
        printf("Please enter digits.\n\n");
        }
    }while (!validateDigits(buffer));
    sscanf(buffer, "%f", &controller->rain_sensor_limit);   // Set the rain sensor limit

    // Generate a random rain sensor value between 0 and 2 inches
    srand(time(NULL));
    controller->rain_sensor_value = (float)(rand() % 21) / 10.0;
    printf("Rain sensor value set to: %.1f inches\n", controller->rain_sensor_value); // Display the rain sensor value
}



// Schedule the operation of zones
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


// Reset system to default settings
void resetSystem(Controller *controller) {
    free(controller->zones);
    loadDefaultSettings(controller);
    printf("System reset to default settings.\n");
    clearAndDisplayTime(controller);
}


// Load default settings
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


//  I used these two last functions as same my previous FINALXC

// Input function
char *input(char *buffer, int size) {
    if (fgets(buffer, size, stdin)) {
        buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline character
    }
    return buffer;  // Return the buffer
}

// Validate if input is digits only
bool validateDigits(const char *input) {
    for (int i = 0; input[i] != '\0'; i++) {
        if (!isdigit(input[i])) {
            return false;
        }
    }
    return true;    // Return true if all characters are digits
}








