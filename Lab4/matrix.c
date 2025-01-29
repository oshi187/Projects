#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

// Function to allocate memory for a matrix
double** alloc_mem_matrix(int dim) {
	double** matrix = (double**)calloc(dim, sizeof(double*));
	for (int i = 0; i < dim; i++) {
		matrix[i] = (double*)calloc(dim, sizeof(double));
	}
	return matrix;
}

// Function to free memory of a matrix
void free_mem_matrix(double** matrix, int dim) {
	for (int i = 0; i < dim; i++) {
		free(matrix[i]);
	}
	free(matrix);
}

// Function to display a matrix
void display_matrix(double** matrix, int dim) {
	for (int i = 0; i < dim; i++) {
		for (int j = 0; j < dim; j++) {
			printf("%.3f ", matrix[i][j]);
		}
		printf("\n");
	}
}

// Function to initialize matrix A1
void init_matrix_A1(double** matrix, int dim) {
	for (int i = 0; i < dim; i++) {
		for (int j = 0; j < dim; j++) {
			matrix[i][j] = (double)(i + 1) / (j + 1);
		}
	}
}

// Function to initialize matrix A2
void init_matrix_A2(double** matrix, int dim) {
	for (int i = 0; i < dim; i++) {
		for (int j = 0; j < dim; j++) {
			if (i > j) {
				matrix[i][j] = 0.0;
			}
			else {
				matrix[i][j] = (i == j) ? 1.0 : (double)(i + 1) * (j + 1) / (i + j + 2);
			}
		}
	}
}

// Function to multiply matrices A1 and A2
void multiply_matrices(double** a1, double** a2, double** a_res, int dim) {
	for (int i = 0; i < dim; i++) {
		for (int j = 0; j < dim; j++) {
			a_res[i][j] = 0.0;
			for (int k = 0; k < dim; k++) {
				a_res[i][j] += a1[i][k] * a2[k][j];
			}
		}
	}
}

// Function to calculate the determinant of a 3x3 matrix
double calc_determinant(double** matrix) {
	double determinant = 0;

	for (int i = 0; i < 3; i++) {
		double term1 = matrix[0][i] * (matrix[1][(i + 1) % 3] * matrix[2][(i + 2) % 3] - matrix[1][(i + 2) % 3] * matrix[2][(i + 1) % 3]);
		double term2 = matrix[0][i] * (matrix[1][(i + 2) % 3] * matrix[2][(i + 1) % 3] - matrix[1][(i + 1) % 3] * matrix[2][(i + 2) % 3]);

		determinant += term1 - term2;
	}

	return determinant;
}


int main() {
	int dim;
	printf("Please insert the dimension of the matrix: ");
	scanf("%d", &dim);

	double** A1 = alloc_mem_matrix(dim);
	double** A2 = alloc_mem_matrix(dim);
	double** A_res = alloc_mem_matrix(dim);

	init_matrix_A1(A1, dim);
	init_matrix_A2(A2, dim);

	printf("This is matrix A1:\n");
	display_matrix(A1, dim);

	printf("This is matrix A2:\n");
	display_matrix(A2, dim);

	multiply_matrices(A1, A2, A_res, dim);

	printf("This is matrix A_res which results from A1*A2:\n");
	display_matrix(A_res, dim);

	free_mem_matrix(A1, dim);
	free_mem_matrix(A2, dim);
	free_mem_matrix(A_res, dim);

	return 0;

}







