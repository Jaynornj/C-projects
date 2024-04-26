#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

//AUTHOR: Jaynor Neiva
//Description: Final test XC
//Date: 04/26/24
//Course: COP 2220

// Constants
#define PI 3.14159
#define MAX_CIRCLES 10
#define BUFFER_SIZE 256

// Struct Circle
typedef struct {
    double radius;
    double area;
    double circumference;
}  Circle;

// Function prototypes
void populateCircles(Circle *circles);
void calculateAreas(Circle *circles);
void calculateCircumferences(Circle *circles);
void displayCircle(Circle circle);
void searchAndDisplayCircle(Circle *circles);
void displayLargestAndSmallest(Circle *circles);
char *input(char *buffer, int size);

int main() {
    // Seed the random number generator
    srand(time(NULL));

    // Allocate memory for array of circles
    Circle *circles = (Circle *)malloc(MAX_CIRCLES * sizeof(Circle));
    if (circles == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    int option;
    int populated = 0; // Flag to track if circles are populated

    do {
        // Display menu
        printf("\nMenu:\n");
        printf("1 - Populate all circles\n");
        printf("2 - Search and display a circle by radius range\n");
        printf("3 - Quit Program\n");
        printf("Enter your choice: ");

        char buffer[BUFFER_SIZE];
        input(buffer, BUFFER_SIZE);
        sscanf(buffer, "%d", &option);

        switch (option) {
            case 1:
                if (!populated) {
                    populateCircles(circles);
                    calculateAreas(circles);
                    calculateCircumferences(circles);
                    populated = 1;
                } else {
                    printf("Circles already populated.\n");
                }
                break;
            case 2:
                if (populated) {
                    searchAndDisplayCircle(circles);
                } else {
                    printf("Circles must be populated first. Select option 1\n");
                }
                break;
            case 3:
                if (populated) {
                    displayLargestAndSmallest(circles);
                } else {
                    printf("Circles must be populated first.\n");
                }
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    } while (option != 3);

    // Free memory
    free(circles);

    return 0;
}

void populateCircles(Circle *circles) {
    for (int i = 0; i < MAX_CIRCLES; i++) {
        circles[i].radius = (rand() % 99) + 2; // Generate random radius between 2 and 100
    }
    printf("Circles populated with random radius values.\n\n");
}

void calculateAreas(Circle *circles) {
    for (int i = 0; i < MAX_CIRCLES; i++) {
        circles[i].area = PI * circles[i].radius * circles[i].radius;
    }
}

void calculateCircumferences(Circle *circles) {
    for (int i = 0; i < MAX_CIRCLES; i++) {
        circles[i].circumference = 2 * PI * circles[i].radius;
    }
}

void displayCircle(Circle circle) {
    printf("Circle with radius %.2f\n", circle.radius);
    printf("Area: %.2f\n", circle.area);
    printf("Circumference: %.2f\n", circle.circumference);
}

void searchAndDisplayCircle(Circle *circles) {
    double lowerBound, upperBound;
    printf("Enter lower bound of radius range: ");
    char buffer[BUFFER_SIZE];
    input(buffer, BUFFER_SIZE);
    sscanf(buffer, "%lf", &lowerBound);

    printf("Enter upper bound of radius range: ");
    input(buffer, BUFFER_SIZE);
    sscanf(buffer, "%lf", &upperBound);

    int found = 0;
    for (int i = 0; i < MAX_CIRCLES; i++) {
        if (circles[i].radius >= lowerBound && circles[i].radius <= upperBound) {
            displayCircle(circles[i]);
            found = 1;
        }
    }

    if (!found) {
        printf("No circles found within the specified radius range.\n");
    }
}

void displayLargestAndSmallest(Circle *circles) {
    int largestIndex = 0;
    int smallestIndex = 0;

    for (int i = 1; i < MAX_CIRCLES; i++) {
        if (circles[i].radius > circles[largestIndex].radius) {
            largestIndex = i;
        }
        if (circles[i].radius < circles[smallestIndex].radius) {
            smallestIndex = i;
        }
    }

    printf("Circle with largest radius:\n");
    displayCircle(circles[largestIndex]);

    printf("\nCircle with smallest radius:\n");
    displayCircle(circles[smallestIndex]);
}

char *input(char *buffer, int size) {
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline character
    return buffer;
}
