#pragma once

#include "Lab3_HashCHLib.h"


/****************************************
Prototypes for chained hashing
****************************************/
/*
Function creates a new entry in the hash table or if the key already exists overwrites the value with the passed value.
Function receives the hash table and the key-value-pair to be inserted.
Function returns the number of collisions.
*/
int putCH(sElementCH* hashtableCH[MAX_ARRAY], int key, const char* value) {
	int index = hashing(key);
	sElementCH* current = hashtableCH[index];
	int collisions = 0;

	// Check if key exists - no collision counted here
	while (current != NULL) {
		if (current->key == key) {
			free(current->value);
			current->value = _strdup(value);
			return collisions; // No new collision here
		}
		current = current->next;
		collisions++; // Each next means collision chain longer
	}

	// Insert new element at head
	sElementCH* newElement = (sElementCH*)malloc(sizeof(sElementCH));
	newElement->key = key;
	newElement->value = _strdup(value);
	newElement->next = hashtableCH[index];
	hashtableCH[index] = newElement;

	// If this slot was not empty before, count 1 collision
	if (newElement->next != NULL)
		collisions++;

	return collisions;
}

char* getCH(sElementCH* hashtableCH[MAX_ARRAY], int key) {
	int index = hashing(key);
	sElementCH* current = hashtableCH[index];

	while (current != NULL) {
		if (current->key == key) {
			return current->value;  // Found the key
		}
		current = current->next;
	}
	return NULL;  // Key not found
}


void printHashTableCH(sElementCH* hashtableCH[MAX_ARRAY]) {
	for (int i = 0; i < MAX_ARRAY; i++) {
		printf("Index %d: ", i);
		sElementCH* current = hashtableCH[i];

		if (current == NULL) {
			printf("empty\n");
		}
		else {
			while (current != NULL) {
				printf("[Key: %d, Value: %s] -> ", current->key, current->value);
				current = current->next;
			}
			printf("NULL\n");  // End of chain
		}
	}
}


/*
Function gets a file pointer to a csv-file which contains some key-value-pairs.
Additionally, the function gets a hash table in which the function inserts the key-value-pairs in the csv-file.
*/
void readCSVCH(FILE* fP, sElementCH* hashtableCH[MAX_ARRAY]) {
	int number = 0;
	char buffer[MAX_LINE];
	char* field;
	int key;
	char string[MAX_STRING];

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

		putCH(hashtableCH, key, string);
	}

	return;
}

void freeHashTableCH(sElementCH* hashtableCH[MAX_ARRAY]) {
	for (int i = 0; i < MAX_ARRAY; i++) {
		sElementCH* current = hashtableCH[i];
		while (current != NULL) {
			sElementCH* temp = current;
			current = current->next;
			free(temp->value);  // free strdup'ed value
			free(temp);         // free node
		}
		hashtableCH[i] = NULL;  // clean up pointer
	}
}
