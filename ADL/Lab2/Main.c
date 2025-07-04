#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sorting (1).h"


int main() {
    const int runs = 5;
    int array[MAX_SIZE] = { 0 };
    int tempArray[MAX_SIZE] = { 0 };
    int k = 1000;

    srand(time(NULL));  // Seed random number generator once


    while (k <= MAX_SIZE) {
        printf("\nEvaluating for k = %d\n", k);
        //initArray(array, k);

        double totalTime = 0.0;
        int totalComparisons = 0;
        int totalSwaps = 0;

        for (int i = 0; i < runs; i++) {
            initArray(array, k);
            memcpy(tempArray, array, sizeof(int) * MAX_SIZE);
            //printf("Unsorted Array\n");
         
            resetCounters();
            clock_t start = clock();

            //CHANGE THIS LINE FOR EACH ALGORITHM
            //mergesort(tempArray, 0, MAX_SIZE-1);
            //insertionSort(tempArray);
            //selectionSort(tempArray);
            shellSort(tempArray);
            //mergesort(tempArray, int l, int r);
            //mergesortBU(tempArray, MAX_SIZE);
            //quicksort(tempArray, 0, MAX_SIZE-1);
            //bubbleSort(tempArray);
           // combSort(tempArray);
            //printArray(tempArray);
            clock_t end = clock();
            totalTime += (double)(end - start) / CLOCKS_PER_SEC;
            totalComparisons += getComparisons();
            totalSwaps += getSwaps();
            // printf("Sorted Array\n");
            // printArray(tempArray);
        }

        printf("CombSort | Avg Time: %f sec | Comparisons: %d | Swaps: %d\n",
            totalTime / runs, totalComparisons / runs, totalSwaps / runs);

        k *= 2;
    }

    return 0;
}
