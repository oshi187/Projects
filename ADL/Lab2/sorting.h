#pragma once

#define MAX_SIZE 1000

extern int comparisons;
extern int swaps;


void initArray(int arr[], int k);
void printArray(int array[]);

void resetCounters();
int getComparisons();
int getSwaps();

//Helper Functions
void swap(int array[], int i, int j);
int compare(int i, int j);
int minimum(int x, int y);

//Sorting Functions
void insertionSort(int array[]);
void selectionSort(int array[]);
void shellSort(int array[]);
void mergesort(int array[], int l, int r);
void merge(int array[], int l, int m, int r);
void mergesortBU(int array[], int arraySize);
void mergeBU(int array[], int l, int m, int r);
void quicksort(int array[], int l, int r);
int partition(int array[], int l, int r);
void bubbleSort(int array[]);
void combSort(int array[]);
