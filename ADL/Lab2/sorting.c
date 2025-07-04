#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sorting (1).h"


void initArray(int arr[], int k) {
	if (k > MAX_SIZE) {
		k = MAX_SIZE;
	}

	// Step 1: Initialize array with 1 to MAX_SIZE
	for (int i = 0; i < MAX_SIZE; i++) {
		arr[i] = i + 1;
	}

	// Step 2: Randomly select k indices from the array
	int indices[MAX_SIZE];
	for (int i = 0; i < MAX_SIZE; i++) {
		indices[i] = i;
	}

	// Fisher-Yates shuffle to randomly pick k unique indices
	for (int i = MAX_SIZE - 1; i > 0; i--) {
		int j = rand() % i + 1;
		int temp = indices[i];
		indices[i] = indices[j];
		indices[j] = temp;
	}

	// Create a temporary array to hold the final result
	int temp[MAX_SIZE];
	int sorted_index = 0;
	int unsorted_index = MAX_SIZE - k;

	// Mark selected elements to be moved to end
	int selected[MAX_SIZE] = { 0 };
	for (int i = 0; i < k; i++) {
		selected[indices[i]] = 1;
	}

	// First, add sorted elements (not selected)
	for (int i = 0; i < MAX_SIZE; i++) {
		if (!selected[i]) {
			temp[sorted_index++] = arr[i];
		}
	}

	// Then, add the selected (unsorted) elements to the end
	for (int i = 0; i < k; i++) {
		temp[unsorted_index++] = arr[indices[i]];
	}

	// Copy temp back to original array
	for (int i = 0; i < MAX_SIZE; i++) {
		arr[i] = temp[i];
	}
}


void printArray(int array[]) {
	for (int i = 0; i < MAX_SIZE; i++) {
		printf("%10d", array[i]);
	}
	printf("\n");
}

//Counters
int comparisons = 0;
int swaps = 0;

void resetCounters() {
	comparisons = 0;
	swaps = 0;
}

int getComparisons() {
	return comparisons;
}

int getSwaps() {
	return swaps;
}



//Helper Functions
int compare(int i, int j) {
	comparisons++;
	if (i < j)
		return 1;
	else
		return 0;
}

void swap(int array[], int i, int j) {
	swaps++;
	int temp = array[i];
	array[i] = array[j];
	array[j] = temp;
}

int minimum(int x, int y) {
	return (x < y) ? x : y;
}

//Sorting Functions
void insertionSort(int array[]) {
	int i = 1;
	int j = 0;

	while (i < MAX_SIZE) {
		j = i;
		while ((j > 0) && !compare(array[j - 1], array[j])) {
			swap(array, j, j - 1);
			j = j - 1;
		}
		i = i + 1;
	}
}

void selectionSort(int array[]) {
	for (int i = 0; i < MAX_SIZE; i++) {
		int min = i;
		for (int j = i + 1; j < MAX_SIZE; j++) {
			if (compare(array[j], array[min]))
				min = j;
		}
		swap(array, i, min);
	}
}

void shellSort(int array[]) {
	for (int interval = MAX_SIZE / 2; interval > 0; interval /= 2) {
		for (int i = interval; i < MAX_SIZE; i += 1) {
			int temp = array[i];
			int j;
			comparisons++;
			for (j = i; j >= interval && array[j - interval] > temp; j -= interval) {
				comparisons++;
				array[j] = array[j - interval];
			}
			array[j] = temp;
			swaps++;
		}
	}
}

void mergesort(int array[], int l, int r) {
	// End of recursion reached?
	if (l >= r)
		return;

	int m = l + (r - l) / 2;
	mergesort(array, l, m);
	mergesort(array, m + 1, r);
	merge(array, l, m, r);
	return;
}

void merge(int array[], int l, int m, int r) {
	int i = 0, j = 0, k = 0;
	int leftSize = m - l + 1;
	int rightSize = r - m;

	/* create temp arrays */
	int leftArray[MAX_SIZE] = { 0 }, rightArray[MAX_SIZE] = { 0 };

	/* Copy data to temp arrays leftArray[] and rightArray[] */
	for (i = 0; i < leftSize; i++)
		leftArray[i] = array[l + i];
	for (j = 0; j < rightSize; j++)
		rightArray[j] = array[m + 1 + j];

	/* Merge the temp arrays back into array[l..r]*/
	// Initial index of first subarray
	i = 0;
	// Initial index of second subarray
	j = 0;
	// Initial index of merged subarray
	k = l;

	while ((i < leftSize) && (j < rightSize)) {
		if (!compare(leftArray[i], rightArray[j])) {
			array[k] = leftArray[i];
			i++;
		}
		else {
			array[k] = rightArray[j];
			j++;
		}
		k++;
	}

	/* Copy the remaining elements of left side[], if there	are any */
	while (i < leftSize) {
		array[k] = leftArray[i];
		i++;
		k++;
	}

	/* Copy the remaining elements of R[], if there
	are any */
	while (j < rightSize) {
		array[k] = rightArray[j];
		j++;
		k++;
	}
}

void mergesortBU(int array[], int arraySize) {
	// For current size of subarrays to be merged
	// curr_size varies from 1 to n/2
	int currSize = 0;
	// For picking starting index of left subarray
	// to be merged
	int leftStart = 0;

	// Merge subarrays in bottom up manner. First merge subarrays of
	// size 1 to create sorted subarrays of size 2, then merge subarrays
	// of size 2 to create sorted subarrays of size 4, and so on.
	for (currSize = 1; currSize <= arraySize - 1; currSize = 2 * currSize) {
		// Pick starting point of different subarrays of current size
		for (leftStart = 0; leftStart < arraySize - 1; leftStart += 2 * currSize) {
			// Find ending point of left subarray. mid+1 is starting
			// point of right
			int mid = minimum(leftStart + currSize - 1, arraySize - 1);

			int right_end = minimum(leftStart + 2 * currSize - 1, arraySize - 1);

			// Merge Subarrays arr[left_start...mid] & arr[mid+1...right_end]
			mergeBU(array, leftStart, mid, right_end);
		}
	}
}

void mergeBU(int array[], int l, int m, int r) {
	int i = 0, j = 0, k = 0;
	int leftSize = m - l + 1;
	int rightSize = r - m;

	/* create temp arrays */
	int leftPart[MAX_SIZE] = { 0 }, rightPart[MAX_SIZE] = { 0 };

	/* Copy data to temp arrays L[] and R[] */
	for (i = 0; i < leftSize; i++)
		leftPart[i] = array[l + i];
	for (j = 0; j < rightSize; j++)
		rightPart[j] = array[m + 1 + j];

	/* Merge the temp arrays back into arr[l..r]*/
	i = 0;
	j = 0;
	k = l;
	while (i < leftSize && j < rightSize) {
		if (!compare(leftPart[i], rightPart[j])) {
			array[k] = leftPart[i];
			i++;
		}
		else {
			array[k] = rightPart[j];
			j++;
		}
		k++;
	}

	/* Copy the remaining elements of L[], if there are any */
	while (i < leftSize) {
		array[k] = leftPart[i];
		i++;
		k++;
	}

	/* Copy the remaining elements of R[], if there are any */
	while (j < rightSize) {
		array[k] = rightPart[j];
		j++;
		k++;
	}
}

void quicksort(int array[], int l, int r) {
	//printArray(array);
	if (r - l <= 0)
		return;
	//printf("r: %d, array[r]: %d, l: %d, array[l]: %d\n\n", r, array[r], l, array[l]);
	int m = partition(array, l, r);
	quicksort(array, l, m - 1);
	quicksort(array, m + 1, r);
}

int partition(int array[], int l, int r) {
	int i = l - 1;
	int j = r;
	while (1) {
		// find item on left
		while (compare(array[++i], array[r]))
			if (i == r)
				// to swap
				break;
		// find item on right
		while (compare(array[r], array[--j]))
			// to swap
			if (j == l)
				break;
		// do pointers cross ?
		if (i >= j)
			break;
		// otherwise do swap
		swap(array, i, j);
	}
	// final swap
	swap(array, i, r);
	// return index of cross point
	return i;
}

void bubbleSort(int array[]) {
	for (int i = 0; i < MAX_SIZE - 1; i++) {
		for (int j = 0; j < MAX_SIZE - i - 1; j++) {
			if (!compare(array[j], array[j + 1])) {
				swap(array, j, j + 1);
			}
		}
	}
}

void combSort(int array[]) {
	int gap = MAX_SIZE;
	const float shrink = 1.3;
	int sorted = 0;

	while (!sorted) {
		gap = (int)(gap / shrink);
		if (gap <= 1) {
			gap = 1;
			sorted = 1;
		}

		for (int i = 0; i + gap < MAX_SIZE; i++) {
			if (!compare(array[i], array[i + gap])) {
				swap(array, i, i + gap);
				sorted = 0;
			}
		}
	}
}
