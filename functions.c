/*
Program : functions.c
Course  : SOL1 SS2024
Author  : Oshini Jayaweera
Date    : 07.05.2024
*/


#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
// (a) sum up array elements
double sum_up(double* array, int num_elements) {
	double sum = 0.0;
	for (int i = 0; i < num_elements; i = i + 1) // all from 0 to num_elements-1
		sum = sum + array[i]; // sum up
	return sum;
}
//(b) Euclidean norm
double norm(double* vector, int dim){
	double norm_squared;
	norm_squared = 0.0;
	for (int i = 0; i < dim; i = i + 1) {
	   norm_squared = norm_squared+ (vector[i] * vector[i]); //function to calculate sum of the square of numbers
    }
	return sqrt(norm_squared); // Return the square root of the sum
}
//(c) Minimum value
double minimum(double* vector, int dim) {
	double min  = 0.0;
	for (int i = 0; i < dim; i = i + 1) {
		if (vector[i] < min) {
			min = vector[i];
		}
	}
	return min; // Return the minimum value
}

int main(void) {
	double numbers[5] = { -3.0, 4.5, 2.7, 8.4, -0.6 }; // example numbers
	double sum = sum_up(&numbers[0], 5); // Address of start of array and length of the array
	printf("The sum is %4.2lf\n", sum);

	int vector = numbers;
	double norm_value = norm(&numbers[0], 5); // Calculate Euclidean norm (calling the function)
	printf("The Euclidean norm is %4.2lf\n", norm_value);

	double min_value = minimum(&numbers[0], 5); //Calculate the minimum of the numbers
	printf("The minimum value is %4.2lf\n", min_value);

	return 0;
}




