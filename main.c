#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Zone structure
typedef struct {
    int operation_date; // days offset from the current date
    int start_time; // in minutes from midnight
    int end_time; // in minutes from midnight
    int total_minutes;
    int sequence;
    int skipped; // 0 = false, 1 = true
    int state; // 0 = off, 1 = on
} Zone;

// Controller structure
typedef struct {
    struct tm current_datetime;
    float rain_sensor_value;
    int num_zones;
    Zone *zones;
} Controller;

// Function prototypes
void displayMenu(Controller *controller);
void setDateTime(Controller *controller);
void selectNumZones(Controller *controller);
void configureZones(Controller *controller);
void setRainSensor(Controller *controller);
void scheduleOperation(Controller *controller);
void resetSystem(Controller *controller);
void loadDefaultSettings(Controller *controller);
void saveSettings(Controller *controller);
void deleteSettings(Controller *controller);
void clearAndDisplayTime(Controller *controller);
void flushStdin(void);

int main() {
    Controller controller;
    loadDefaultSettings(&controller);

    int choice;
    while (1) {
        clearAndDisplayTime(&controller);
        displayMenu(&controller);
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
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
            default:
                printf("Invalid choice, please try again.\n");
                break;
        }
    }
    return 0;
}
void clearAndDisplayTime(Controller *controller) {
    system("cls");  // Use "clear" if on Unix/Linux
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%c", &controller->current_datetime);
    printf("Current system date and time: %s\n\n", buffer);
}

void displayMenu(Controller *controller) {
    // Display current date and time
    char buffer[30];
    time_t rawtime = time(NULL);
    struct tm *timeinfo = localtime(&rawtime);
    strftime(buffer, 30, "%c", timeinfo);
    printf("\nCurrent system date and time: %s\n", buffer);

    // Menu options
    printf("1 - Set date and time\n");
    printf("2 - Select number of zones\n");
    printf("3 - Configure operation of all zones\n");
    printf("4 - Set the rain sensor limits\n");
    printf("5 - Schedule the operation of zones\n");
    printf("6 - Reset system to default settings\n");
    printf("7 - Exit program\n");
}

void setDateTime(Controller *controller) {
    // Assuming this function sets system date and time manually for simplicity
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    printf("Enter year: ");
    scanf("%d", &timeinfo->tm_year);
    timeinfo->tm_year -= 1900; // tm_year is year since 1900

    printf("Enter month (1-12): ");
    scanf("%d", &timeinfo->tm_mon);
    timeinfo->tm_mon -= 1; // tm_mon is month from 0 to 11

    printf("Enter day (1-31): ");
    scanf("%d", &timeinfo->tm_mday);

    printf("Enter hour (0-23): ");
    scanf("%d", &timeinfo->tm_hour);

    printf("Enter minute (0-59): ");
    scanf("%d", &timeinfo->tm_min);

    printf("Enter second (0-59): ");
    scanf("%d", &timeinfo->tm_sec);

    mktime(timeinfo); // normalize the time structure
    controller->current_datetime = *timeinfo;
}

void selectNumZones(Controller *controller) {
    printf("Enter the number of zones: ");
    scanf("%d", &controller->num_zones);
    controller->zones = (Zone *)malloc(controller->num_zones * sizeof(Zone));
    for (int i = 0; i < controller->num_zones; i++) {
        controller->zones[i].sequence = i + 1;
        controller->zones[i].skipped = 0;
        controller->zones[i].state = 0;
    }
}

void configureZones(Controller *controller) {
    for (int i = 0; i < controller->num_zones; i++) {
        printf("Configuring zone %d:\n", i + 1);
        printf("Enter operation date (days from today): ");
        scanf("%d", &controller->zones[i].operation_date);
        printf("Enter start time (minutes from midnight): ");
        scanf("%d", &controller->zones[i].start_time);
        printf("Enter duration (minutes): ");
        scanf("%d", &controller->zones[i].total_minutes);
        controller->zones[i].end_time = controller->zones[i].start_time + controller->zones[i].total_minutes;
    }
}

void setRainSensor(Controller *controller) {
    float tempValue;
    int isValid =0; //flag to check valid input

    while (!isValid) {
        printf("Enter rain sensor value (0 to 2 inches): ");
        scanf("%f", &tempValue);

        // Check if the value is within the allowable range and is an increment of 0.5
        if ((tempValue >= 0.5 && tempValue <= 2.0) && ((int)(tempValue * 10) % 5 == 0)) {
            isValid = 1; // Set flag to true if value is valid
            controller->rain_sensor_value = tempValue;
        } else {
            printf("Invalid input. Please enter a value of 0.5, 1.0, 1.5, or 2.0 inches.\n");
        }
    }

    printf("Rain sensor limit set to %.1f inches.\n", controller->rain_sensor_value);
}

void scheduleOperation(Controller *controller) {
    // Simulation of operation
    printf("Scheduling operation:\n");
    for (int i = 0; i < controller->num_zones; i++) {
        if (controller->rain_sensor_value > 0) {
            printf("Zone %d operation skipped due to rain.\n", i + 1);
            controller->zones[i].skipped = 1;
        } else {
            printf("Zone %d operating from %d to %d minutes.\n", i + 1, controller->zones[i].start_time, controller->zones[i].end_time);
            controller->zones[i].skipped = 0;
        }
    }
}

void resetSystem(Controller *controller) {
    deleteSettings(controller);
    loadDefaultSettings(controller);
    printf("System reset to default settings.\n");
}

void loadDefaultSettings(Controller *controller) {
    // Loads default settings, could be extended to read from a file
    controller->num_zones = 7; // default number of zones
    controller->rain_sensor_value = 0.0;
    controller->zones = (Zone *)malloc(controller->num_zones * sizeof(Zone));
    for (int i = 0; i < controller->num_zones; i++) {
        controller->zones[i].operation_date = 0;
        controller->zones[i].start_time = 480 + i * 30; // starting at 8:00 AM, each 30 minutes apart
        controller->zones[i].total_minutes = 30;
        controller->zones[i].end_time = controller->zones[i].start_time + controller->zones[i].total_minutes;
        controller->zones[i].sequence = i + 1;
        controller->zones[i].skipped = 0;
        controller->zones[i].state = 0;
    }
}

void saveSettings(Controller *controller) {
    // Save settings to file
}

void deleteSettings(Controller *controller) {
    // Delete settings file
}

void flushStdin(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}
