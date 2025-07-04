#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include "string.h"

//Macros
#define MAX_ARRAY 50
#define MAX_STRING 30
#define MAX_LINE 100

//data type for chained hashing
typedef struct sElementCH {
	int key;
	char* value;
	struct sElementCH* next;
} sElementCH;



int putCH(sElementCH* hashtableCH[MAX_ARRAY], int key, const char* value);
char* getCH(sElementCH* hashtableCH[MAX_ARRAY], int key);
void printHashTableCH(sElementCH* hashtableCH[MAX_ARRAY]);
void readCSVCH(FILE * fP, sElementCH * hashtableCH[MAX_ARRAY]);
