/*
Program : numbers.c
Course  : SOL1 SS2024
Author  : Oshini Jayaweera
Date    : 07.05.2024
*/

#include <stdio.h>


// Function to print an integer array
void print_int_array(int* array, int num) {
    for (int i = 0; i < num; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
}
//Function to shift the numbers in the array one element to the left
void shift_left(int* array, int num) {
    int first_element = array[0];
    for (int i = 0; i < num - 1; i++) {
        array[i] = array[i + 1];
    }
    array[num - 1] = first_element;
}
//Function to shift the numbers one element to the right
void shift_right(int* array, int num) {
    int last_element = array[num-1];
    for (int i = num-1; i > 0; i--) {
        array[i] = array[i - 1];
    }
    array[0] = last_element;
}

// Function to swap the values of two variables
void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int main() {
    int numbers[10]; // Definition of an array of int with 10 elements

    // (a) Ask the user to input numbers for each array element
    for (int i = 0; i < 10; i++) {
        printf("Please enter a number [%d]: ", i + 1);
        scanf_s("%d", &numbers[i]);
    }

    // (b) Output the 10 numbers to the screen
    printf("\n");
    print_int_array(numbers, 10);

    // (c) Determine the memory size occupied by the array
    int size_of_int = sizeof(int); // Size of an int in bytes
    printf("\nSize of an int: %d bytes\n", size_of_int);
    printf("Size of an int: %d bits\n", size_of_int * 8);
    printf("Memory size occupied by the whole array: %d bytes\n", sizeof(numbers));

    // (d) Shift numbers towards the front and print the modified array
    printf("Shift towards the front: ");
    shift_left(numbers, 10);
    print_int_array(numbers, 10);

    // (e) Shift numbers towards the back and print the modified array
    printf("Shift towards the back: ");
    shift_right(numbers, 10);
    print_int_array(numbers, 10);

    // (f) Swap the second and ninth elements of the array
    swap(&numbers[1], &numbers[8]);
    printf("Swap numbers 2 and 9: ");
    print_int_array(numbers, 10);

    // (g) Swap unsorted neighbors if the first number is greater than the second
    for (int i = 0; i < 10 - 1; i++) {
        if (numbers[i] > numbers[i + 1]) {
            swap(&numbers[i], &numbers[i + 1]);
        }
    }
    printf("Swap unsorted neighbors: ");
    print_int_array(numbers, 10);

    // (h) Swap unsorted neighbors 9 times
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 10 - 1; j++) {
            if (numbers[j] > numbers[j + 1]) {
                swap(&numbers[j], &numbers[j + 1]);
            }
        }
    }
    printf("Swap unsorted neighbors 9 times: ");
    print_int_array(numbers, 10);



    return 0;
}