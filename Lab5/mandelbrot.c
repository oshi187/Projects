/** Doxygen-Info
 * @file haw_statistics.c
 * @file mandelbrot.c
 * @author Oshini Jayaweera
 * @author Prof. Rainer Schoenen
 * @copyright Prof. Dr. Rainer Schoenen
 * @date SS20240625
 * @version 20230522
 * @brief support library for statistics
 * @note Support Library at HAW Hamburg, TI-ETECH, KLAB, Prof. Schoenen
 * @note Do not try to understand this code in your precious (exam) time. Just use the library without thinking.
 * Detailed decription:
 */

 /* all the important includes and settings ... */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h> /* printf(), scanf() */
#include <stdlib.h> /*malloc, free, ...*/
#include <math.h>  /* fabs(), sqrt() */
#include "mandelbrot.h"

//Function to calculate each point of the mandelbrot set
void mandelbrot_algorithm(point_t** set,  int x_len, int y_len, double x_step, double y_step) {

	int n;

	//Calculate the convergence or iteration of divergence for each cartesian coordinate
	for (int i = 0; i < x_len; i++) {
		for (int j = 0; j < y_len; j++) {
			set[j][i].x_start = XMIN + i * x_step;
			set[j][i].y_start = YMIN + j * y_step;

			n = calculate_n(set[j][i].x_start, set[j][i].y_start);

			//Define the color based on the convergence or iteration of divergence
			set[j][i].convergence_color = colors[n];
			set[j][i].convergence = (n >= MAX_ITERATIONS ? convergent : divergent);
		}
	}	
}

//Function to calculate n: The iteration where the series diverges
int calculate_n(double x_start, double y_start) {

	int n = 0;

	//Your implementation here
	double x = 0.0, y = 0.0; // Initial values for x1 and y1
	int iterations;

	for (iterations = 0; iterations < MAX_ITERATIONS; ++iterations) {
		// Mandelbrot iterative formula
		double x_new = x * x - y * y + x_start;
		double y_new = 2 * x * y + y_start;

		// Update x and y for next iteration
		x = x_new;
		y = y_new;

		// Check divergence criteria
		double z = x * x + y * y;
		if (z > CRITERIA_DIV) {
			return iterations; // Diverged
		}
	}

	return MAX_ITERATIONS; // Converged
} //calculate_n

//Allocation of the two dimensional array for 
point_t** alloc_mem_mandelbrot(int dim_x, int dim_y) {

	//Allocate the memory for an array of pointers of type point_*
	point_t** set = (point_t**)malloc(dim_x * sizeof(point_t*));

	//check if malloc returned NULL
	if (set == NULL) { 
		printf("Memory error!\n");
		free(set);
		exit(1);
	}
	
	//Allocate the memory for each pointer of type point_*
	for (int i = 0; i < dim_x; i++) {
		set[i] = (point_t*)malloc(dim_y * sizeof(point_t));

		//Check if malloc returend NULL
		if (set[i] == NULL) {
			printf("Memory error!\n");
			for (int j = 0; j < i; j++)
				free(set[j]);
			free(set);
			exit(1);
		}
		
	}

	return set;
} //alloc_mem_mandelbrot

//Function to free the allocated memory for the mandelbrot set
void free_mandelbrot_set(point_t** set, int x_dim, int y_dim) {
	for (int i = 0; i < x_dim; i++)
		free(set[i]);
	free(set);
} //free_mandelbrot_set

//Function to print out the mandelbrot in ones and zeros to the command line
void printf_mandelbrot_cmdline(point_t** set, int x_dim, int y_dim) {
	for (int i = 0; i < x_dim; i++) {
		for (int j = 0; j < y_dim; j++) {
			//Change i and j because it looks better in the cmd-line if its transposed
			printf("%i", set[j][i].convergence);
		}
		printf("\n");
	}
}

//Function to print the mandelbrot set into a ppm file
int fprintf_mandelbrot_ppn(point_t** set, int x_len, int y_len) {

	// Your implementation here: 

	FILE* file = fopen("mandelbrot.ppm", "w");
	if (file == NULL) {
		printf("Error opening file!\n");
		return -1;
	}

	fprintf(file, "P3\n");
	fprintf(file, "%d %d\n", x_len, y_len);
	fprintf(file, "255\n");

	for (int i = 0; i < y_len; i++) {
		for (int j = 0; j < x_len; j++) {
			color_t color = set[i][j].convergence_color;
			fprintf(file, "%d %d %d ", color.r, color.g, color.b);
		}
		fprintf(file, "\n");
	}

	fclose(file);

	return 0;

}
	 //fprintf_ppn_mandelbrot()

//Initialization of color array
void initialize_colors() {
	for (int i = 0; i < MAX_ITERATIONS; i++) {
		// Simple way to code the colors and to get a smooth transition of colors
		colors[i].r = (i % 256);
		colors[i].g = ((i * 5) % 256);
		colors[i].b = ((i * 13) % 256);
	}
	// The last element is used as color for convergence (black->RGB 0 0 0)
	colors[MAX_ITERATIONS].r = 0;
	colors[MAX_ITERATIONS].g = 0;
	colors[MAX_ITERATIONS].b = 0;
} //initialize_colors()
