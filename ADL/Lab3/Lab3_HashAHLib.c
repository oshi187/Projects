#pragma once

#include "Lab3_HashAHLib.h"

/**
* @brief Function reads an integer value safely from the keyboardand.
*
* @param Parameter
* None.
*
* @return Function returns the integer value.
*/
int getInt() {
	int val = 0;
	while (1) {
		if ((scanf("%d", &val) != 1) || (val < 0)) {
			while (getchar() != '\n');
			printf("Not a valid integer.\n");
		}
		else {
			while (getchar() != '\n');
			break;
		}
	}
	return val;
}

/**
* @brief Function reads a string value safely from the keyboardand.
*
* @param string The parameter string is transferred via CbR and contains the read string after it has been read in.
*
* @return
* None.
*/
void getString(char string[]) {
	while (1) {
		if (scanf("%s", string) != 1) {
			while (getchar() != '\n');
			printf("Not a valid integer.\n");
		}
		else {
			while (getchar() != '\n');
			break;
		}
	}
	return;
}


/*
Function calculates the hash value for the passed key.
Function return the calculated hash value.
*/
int hashing(int key) {
	// Choose ONE of these hashing strategies

	// 1. Division method
	return key % MAX_ARRAY;

	// 2. Truncated Key method
	// return (key / 10) % MAX_ARRAY;

	// 3. Digit Folding
	/*int hash = 0;
	while (key > 0) {
	  hash += key % 10;
	  key /= 10;
	}
	return hash % MAX_ARRAY;
	*/

	// 4. Mid-square method
	// long long square = (long long)key * (long long)key;
	// int mid = (square / 100) % 1000;
	// return mid % MAX_ARRAY;
}



/****************************************
Prototypes for address hashing
****************************************/
/*
Function creates a new entry in the hash table or if the key already exists overwrites the value with the passed value.
Function receives the hash table and the key-value-pair to be inserted.
Function returns the number of collisions.
*/
int putAH(sElementAH hashtableAH[MAX_ARRAY], int key, char value[MAX_STRING]) {
	//Todo: Assignment 1.1
	int index = hashing(key);
	int collisions = 0;
	int startIndex = index;

	while (1) {
		//Case 1: Empty slot --> insert here
		if (hashtableAH[index].key == -1 || hashtableAH[index].key == -2) {
			hashtableAH[index].key = key;
			strcpy(hashtableAH[index].value, value);
			return collisions;
		}

		//Case 2: Key already exists --> overwrites the value
		if (hashtableAH[index].key == key) {
			strcpy(hashtableAH[index].value, value);
			return collisions;
		}

		//Case 3: Collision --> move to next index
		index = (index + 1) % MAX_ARRAY;
		collisions++;

		//The hash table is full
		if (index == startIndex) {
			printf("Hash table is full. Cannot insert key %d.\n", key);
			return collisions;
		}
	}

}

/*
Function searches for the entry in the hash table having the passed key.
Function returns the value of entry or NULL if the key does not exist in the hash table.
*/
char* getAH(sElementAH hashtableAH[MAX_ARRAY], int key) {
	//Todo: Assignment 1.2
	int index = hashing(key);        // Start from hashed index
	int startIndex = index;          // Remember where we started

	while (hashtableAH[index].key != -1) {  // While the slot is not empty
		if (hashtableAH[index].key == key) {
			return hashtableAH[index].value;  // Found the key
		}

		index = (index + 1) % MAX_ARRAY;      // Move to next index (wrap around if needed)
		if (index == startIndex) {
			break;  // We've looped through the table
		}
	}

	return NULL;  // Key not found
}

/*
Function searches for the entry in the hash table having the passed key.
If it finds an entry the function deletes it from the hash table.
*/
void deleteAH(sElementAH hashtableAH[MAX_ARRAY], int key) {
	//Todo: Assignment 1.3
	int index = hashing(key);
	int startIndex = index;

	while (hashtableAH[index].key != -1) { // While not empty
		if (hashtableAH[index].key == key) {
			// Found it: mark as deleted
			hashtableAH[index].key = -2;
			hashtableAH[index].value[0] = '\0'; // or "" if NULL is unsafe
			return;
		}
		index = (index + 1) % MAX_ARRAY;
		if (index == startIndex) {
			break;  // Full loop, key not found
		}
	}
}

/*
Function prints the hash table.
If a field in the array is empty, it prints ?empty?.
*/
void printHashTableAH(sElementAH hashtableAH[MAX_ARRAY]) {
	//Todo: Assignment 1.4
	for (int i = 0; i < MAX_ARRAY; i++) {
		if (hashtableAH[i].key == -1) {
			printf("Index [%d]: empty\n", i);
		}
		else {
			printf("Index [%d]: Key = %d, Value = %s\n", i, hashtableAH[i].key, hashtableAH[i].value);
		}
	}
}

/*
Function gets a file pointer to a csv-file which contains some key-value-pairs.
Additionally, the function gets a hash table in which the function inserts the key-value-pairs in the csv-file.
*/
void readCSVAH(FILE* fP, sElementAH hashtableAH[MAX_ARRAY]) {
	int number = 0;
	char buffer[MAX_LINE];
	char* field;
	int key;
	char string[MAX_STRING];
	int totalCollisions = 0;

	if (fP == NULL) {
		printf("File does not exist.");
		exit(0);
	}
	else
		// Get number of lines
		while (!feof(fP)) {
			char ch = fgetc(fP);
			if (ch == '\n')
				number++;
		}

	if (fP != NULL)
		rewind(fP);

	for (int i = 0; i < number; i++) {
		fgets(buffer, MAX_LINE, fP);


		field = strtok(buffer, ";");
		key = atoi(field);

		field = strtok(NULL, ";");
		strcpy(string, field);
		//replace \n in name
		string[strlen(string) - 1] = '\0';

		putAH(hashtableAH, key, string);
	}
	return;
}
