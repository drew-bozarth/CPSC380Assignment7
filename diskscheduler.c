/*
Drew Bozarth | Romtin Rezvani
2373658 | 2341841
dbozarth@chapman.edu | rrezvani@chapman.edu
CPSC 380-01
Assignment 7 - Disk Scheduling Algorithms - diskscheduler.c
*/

#include <stdlib.h>
#include <stdio.h>

#define numCylinders 5000
#define numRequests 1000

int inputArray[numRequests];

int startIndex = 0;

FILE *ptr;

/*
Uses bubble sort method to sort the array of requests.
Used in the Shortest-Seek-Time-First algorithm, and is already sorted
after that algorithm to be used in SCAN algorithm.
*/
int* sort_array() {
	int temp = 0;

	for (int i = 0; i < numRequests; ++i) {
       	for (int j = i + 1; j < numRequests; ++j) {
            if (inputArray[i] > inputArray[j]) {
                temp =  inputArray[i];
                inputArray[i] = inputArray[j];
                inputArray[j] = temp;
            }
        }
    }
    return inputArray;
}

int fcfsAlgorithm(int *inputArray) {
	int res = 0;
	int newStart = inputArray[startIndex];

    for (int i = startIndex; i < numRequests; ++i) {
    	res += abs(inputArray[i] - newStart);
    }

    for (int i = 0; i < startIndex; ++i) {
    	res += abs(newStart - inputArray[i]);
    }
        
    return res;
}

int sstfAlgorithm(int *inputArray) {
	inputArray = sort_array();

	int nextIndex = startIndex + 1;
	int prevIndex = startIndex - 1;
	int res = 0;
	int headVal = inputArray[startIndex];
	int total = numRequests - 2;
	int newHead = startIndex;
	
	while (total >= 0) {
		int nextDiff = abs(inputArray[nextIndex] - inputArray[newHead]);
		int prevDiff = abs(inputArray[newHead] - inputArray[prevIndex]);

		if (nextDiff > prevDiff) {
			res += prevDiff;
			newHead = prevIndex;
			prevIndex--;
		} else {
			res += nextDiff;
			newHead = nextIndex;
			nextIndex++;
		}
		total--;
	}

	return res;
}

int scanAlgorithm(int * nputArray) {
	int currVal = 0;
	int tempVal = inputArray[startIndex];
	int diff = 0;

	int res = 0;
	int currIndex = 0;

	for (int i = startIndex - 1; i >= 0; --i) {
		currVal = inputArray[i];
		diff = abs(tempVal - currVal);
		res += diff;
		tempVal = currVal;
	}
	res += tempVal;
	tempVal = 0;

	for (int i = startIndex + 1; i < numRequests; ++i) {
		currVal = inputArray[i];
		diff = abs(currVal - tempVal);
		res += diff;
		tempVal = currVal;
	}
	return res;
}

int cscanAlgorithm(int *inputArray) {
	int currVal = 0;
	int tempVal = inputArray[startIndex];
	int diff = 0;
	int res = 0;
	int currIndex = 0;
	int upperBound = 4999;

	for (int i = startIndex + 1; i < numRequests; ++i) {
		currVal = inputArray[i];
		diff = abs(tempVal - currVal);
		res += diff;
		tempVal = currVal;
	}

	res += upperBound - tempVal;
	tempVal = 0;
	res += 4999;

	for (int i = 0; i < startIndex; ++i) {
		currVal = inputArray[i];
		diff = abs(currVal - tempVal);
		res += diff;
		tempVal = currVal;
	}
	return res;
}

int main(int argc, char **argv) {
	startIndex = atoi(argv[1]);

	if (argc != 3) {
		printf("Please retry and make sure the first argument is the startIndex position and the second argument is the file\n");
		exit(-1);
	} else {
		// If correct amount of arguments given, the first argument is the initial postiion
		startIndex = atoi(argv[1]);
	}

	// try to open the file given as the second argument
	ptr = fopen(argv[2], "r");
	if (ptr == NULL) {
		printf("Please retry and make sure the file name is correct and it exists.\n");
		exit(1);
	}

	for (int i = 0; i < numRequests; ++i) {
		fscanf(ptr, "%d", &inputArray[i]); 
	}

	printf("Starting index: %d\n", startIndex);
	printf("Corresponding startIndex value: %d\n", inputArray[startIndex]);

	printf("Total amount of FCFS head movements: %d\n", fcfsAlgorithm(inputArray));
	printf("Total amount of SSTF head movements: %d\n", sstfAlgorithm(inputArray));
	printf("Total amount of SCAN head movements: %d\n", scanAlgorithm(inputArray));
	printf("Total amount of CSCAN head movements: %d\n", cscanAlgorithm(inputArray));

	fclose(ptr);
	return 0;
}