/*
Program : fibonacci.c
Course  : SOL1 SS2024
Author  : Oshini Jayaweera
Date    : 07.05.2024
*/



#include <stdio.h>

// Function to calculate the Fibonacci series and the quotient between consecutive elements
void fibonacci_series(int* fibo, int num_elements) {
    // Initialize the first two elements of the series
    fibo[0] = 0;
    fibo[1] = 1;

    // Calculate the subsequent elements of the series and the quotient
    for (int i = 2; i < num_elements; i++) {
        fibo[i] = fibo[i - 1] + fibo[i - 2];
    }
}

int main() {
    int fibonacci[14]; // Array to store Fibonacci series
    double quotient; // Variable to store the quotient

    // Calculate the Fibonacci series
    fibonacci_series(fibonacci, 14);

    // Output the header
    printf("The first 14 elements of the Fibonacci series are:\n");
    printf("i\t f_i\t f_(i+1)/f_i\n");

    // Output the Fibonacci series and the quotient
    for (int i = 0; i < 13; i++) {
        // Calculate the quotient if possible
        if (fibonacci[i] != 0) {
            quotient = (double)fibonacci[i + 1] / (double)fibonacci[i];
            printf("%d\t %d\t %.8lf\n", i + 1, fibonacci[i], quotient);
        }
        else {
            printf("%d\t %d\t n/a\n", i + 1, fibonacci[i]);
        }
    }

    // Handle the last element separately
    printf("14\t %d\t n/a\n", fibonacci[13]);

    return 0;
}
