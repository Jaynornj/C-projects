#include <stdio.h>
#include <stdlib.h> //  for malloc
#include <string.h> //  for strcspn
#include <stdbool.h> //  for bool type
#include <time.h> //  for srand
#include <ctype.h> //  for isdigit


//AUTHOR: Jaynor Neiva
//Description: Final test 
//Date: 04/26/24
//Course: COP 2220

// Constants
#define PI 3.14159
#define BUFFER_SIZE 256                                 // Buffer size for input

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
bool validateRadius(const char *input);
bool validateDigits(const char *input);

int main() {

    srand(time(NULL));                                // for the random number generator

    // Allocate memory for array of circles
    Circle *circles = (Circle *)malloc(10 * sizeof(Circle));
    if (circles == NULL) {
        printf("Memory allocation failed.\n");
        return 1; // Exit program
    }

    int option;
    int populated = 0;                                             // Flag to track if circles are populated

    do {
        // Display menu
        printf("\nMenu:\n");
        printf("1 - Populate all circles\n");
        printf("2 - Search and display a circle by radius range\n");
        printf("3 - Quit Program\n");
        printf("Enter your choice: ");

        char buffer[BUFFER_SIZE];                                   // Buffer for input
        input(buffer, BUFFER_SIZE);
        if(!validateDigits(buffer)) {                         // Validate input
            printf("Please enter digits.\n\n");
            continue;                                               //repeat the loop if the input is invalid
        }
        sscanf(buffer, "%d", &option);

        switch (option) {
            case 1:
                if (!populated) {                             // if (populated):Check if circles are already populated
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
                printf("Invalid choice. Please keep between option 1 and 3.\n");
                break;
        }
    } while (option != 3);

    // Free memory
    free(circles);

    return 0;
}

void populateCircles(Circle *circles) {
    for (int i = 0; i < 10; i++) {
        circles[i].radius = (rand() % 99) + 2;                      // Generate random radius between 2 and 100
    }
    printf("Circles populated with random radius values.\n\n");
}

void calculateAreas(Circle *circles) {
    for (int i = 0; i < 10; i++) {
        circles[i].area = PI * circles[i].radius * circles[i].radius;    // Area = PI * r^2
    }
}

void calculateCircumferences(Circle *circles) {
    for (int i = 0; i < 10; i++) {
        circles[i].circumference = 2 * PI * circles[i].radius;         // Circumference = 2 * PI * r
    }
}

void displayCircle(Circle circle) {
    printf("Circle with radius %.2f\n", circle.radius);
    printf("Area: %.2f\n", circle.area);
    printf("Circumference: %.2f\n", circle.circumference);
}

void searchAndDisplayCircle(Circle *circles) {
    double lowerBound, upperBound;
    char buffer[BUFFER_SIZE];                                       // Buffer for input

    do {
        printf("Enter lower bound of radius range: ");
        input(buffer, BUFFER_SIZE);
        if (!validateDigits(buffer)) {                        // Validate input
            printf("Please enter digits only.\n");
        }
    } while (!validateDigits(buffer));
    sscanf(buffer, "%lf", &lowerBound);

    do {
        printf("Enter upper bound of radius range: ");
        input(buffer, BUFFER_SIZE);
        if (!validateDigits(buffer)) {
            printf("Please enter digits only.\n");
        }
    } while (!validateDigits(buffer));
    sscanf(buffer, "%lf", &upperBound);

    int found = 0;
    for (int i = 0; i < 10; i++) {
        if (circles[i].radius >= lowerBound && circles[i].radius <= upperBound) { // Check if radius is within range
            displayCircle(circles[i]);
            found = 1;
        }
    }

    if (!found) { // If no circles found
        printf("No circles found with this radius range.\n");
    }
}

bool validateDigits(const char *input) {                                // Validate if input is digits only
    for (int i = 0; i < strlen(input); i++) {
        if (!isdigit(input[i])) {
            return false;
        }
    }
    return true;
}

void displayLargestAndSmallest(Circle *circles) {
    int largestIndex = 0;
    int smallestIndex = 0;

    for (int i = 1; i < 10; i++) {
        if (circles[i].radius > circles[largestIndex].radius) { // Check if radius is larger
            largestIndex = i;
        }
        if (circles[i].radius < circles[smallestIndex].radius) { // Check if radius is smaller
            smallestIndex = i;
        }
    }

    printf("Circle with largest radius:\n");
    displayCircle(circles[largestIndex]);                // Display circle with largest radius

    printf("\nCircle with smallest radius:\n");
    displayCircle(circles[smallestIndex]);               // Display circle with smallest radius
}
char *input(char *buffer, int size) {
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';          // Remove newline character
    return buffer; // Return input
}

// Good day Dr.Saylani, Thank you for the opportunity to retake the final test.
// I have made the necessary changes to the code and I am submitting it for your review.
