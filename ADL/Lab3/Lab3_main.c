#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include "Lab3_HashAHLib.h"
#include "Lab3_HashCHLib.h"

int main() {
	//Variables
	int choice = 0;
	int key = -1;
	char value[MAX_STRING] = { 0 };
	char* valuePointer = NULL;
	int counter = 0;

	//Open csv-file
	FILE* fP = NULL;
	fP = fopen("students.csv", "r");
	if (fP == NULL) {
		printf("CSV file could not be opened.\n");
		exit(1);
	}

	//Welcome and choice of collision strategy
	printf("Welcome to our Hashing program!\n");
	printf("What kind of collision strategy do you prefer?\n1: Address Hashing\n2: Chained Hashing\nAny other choice: exit program\n");
	choice = getInt();

	switch (choice) {
	case 1:
		;
		//Create a hash table and initialize it
		sElementAH hashtableAH[MAX_ARRAY];
		for (int i = 0; i < MAX_ARRAY; i++) {
			hashtableAH[i].key = -1;
			strcpy(hashtableAH[i].value, "");
		}

		//Read in the key-value-pairs from the csv-file and print the hash table
		readCSVAH(fP, hashtableAH);
		printHashTableAH(hashtableAH);

		//User can choose what to do next
		printf("OK, lets do Hashing using Address Hashing strategy:-)\n");
		do {
			printf("What do you like to do?\n1: Adding new element\n2: Searching for an element\n3: Delete key\nAny other choice: exit program\n");
			choice = getInt();
			switch (choice) {
				//Adding a new element to the hash table
			case 1:
				printf("Type in the key: \n");
				key = getInt();
				printf("\nType in the value: \n");
				getString(value);
				counter = putAH(hashtableAH, key, value);
				printHashTableAH(hashtableAH);
				printf("There were %d collision(s).\n", counter);
				break;
				//Searching for an element
			case 2:
				printf("Type in the key: \n");
				key = getInt();
				valuePointer = getAH(hashtableAH, key);
				if (valuePointer == NULL)
					printf("Key not found in hashtable\n");
				else
					printf("Value is %s\n", valuePointer);
				valuePointer = NULL;
				break;
				//Delete a key-value pair if it exists
			case 3:
				printf("Type in the key: \n");
				key = getInt();
				deleteAH(hashtableAH, key);
				printHashTableAH(hashtableAH);
				break;
				//Exit program
			default:
				return 0;
			}
		} while (1);
		break;
	case 2: {
		//Todo: Assignment 3.2
		//Create a hash table and initialize it
		sElementCH* hashtableCH[MAX_ARRAY] = { NULL };

		//Read in the key-value pairs from the csv file and print hash table
		readCSVCH(fP, hashtableCH);
		printHashTableCH(hashtableCH);

		//User can choose what to do next
		printf("Ok, let's do Hashing using Chained Hashing strategy:-)\n");

		do {
			printf("What do you like to do?\n");
			printf("1: Adding a new element.\n");
			printf("2: Searching for an element.\n");
			printf("Any other choice: exit program.\n");

			choice = getInt();

			switch (choice) {
				//Adding a new element to the hash table
			case 1:
				printf("Type in the key: \n");
				key = getInt();

				printf("Type in the value: \n");
				getString(value);

				counter = putCH(hashtableCH, key, value);
				printHashTableCH(hashtableCH);
				printf("There were %d collision(s).\n", counter);
				break;

				//Searching for an element
			case 2:
				printf("Type in the key: \n");
				key = getInt();

				valuePointer = getCH(hashtableCH, key);
				if (valuePointer == NULL) {
					printf("Key not found in the hashtable.\n");
				}
				else {
					printf("Value is %s.\n", valuePointer);
				}
				valuePointer = NULL;
				break;


			default:
				freeHashTableCH(hashtableCH);
				return 0;
			}
		} while (1);
		break;
	}
	default:
		return 0;
	}
}
