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
	int i = 0; 
	int j = 0;
	int swap = 0;

	for (i = 0; i < numRequests; ++i) {
       	for (j = i + 1; j < numRequests; ++j) {
            if (inputArray[i] > inputArray[j]) {
                swap =  inputArray[i];
                inputArray[i] = inputArray[j];
                inputArray[j] = swap;
            }
        }
    }
    return inputArray;
}

/* First-Come-First-Serve (fcfs) starts from the index after the starting
index and continually adds the headmovement from the starting index in 
order recieved. If at end of array, startIndex from index zero and continually 
add until starting index */
int fcfs(int *inputArray) {

	int i = 0;
	headMovement = 0;
	newStart = inputArray[startIndex];

    for(i = startIndex; i < numRequests; i++) {

    	headMovement += abs(inputArray[i] - newStart);
    }

    for(i = 0; i < startIndex; i++) {

    	headMovement += abs(newStart - inputArray[i]);
    }
        
    return headMovement;
}

/* Shortest Seek Time First (SSTF) algorithm has the current position and
adds the position closest to curr. The new position becomes the head and this
will cyclically repeat. First the array is sorted. We made counters for the indexes
above and below startIndex that are decremented if used. When the counters are equal
to numRequests - 2 it exits. */
int sstf(int * inputArray) {

	inputArray = sort_array();

	int prevIndex = startIndex-1;
	int nextIndex = startIndex+1;
	int prevDiff = 0;
	int nextDiff = 0;
	int headMovement = 0;
	int total = numRequests-2;
	int newHead = startIndex;
	int headVal = inputArray[startIndex];
	
	while(total >= 0) {
		prevDiff = abs(inputArray[newHead] - inputArray[prevIndex]);
		nextDiff = abs(inputArray[nextIndex] - inputArray[newHead]);

		if(prevDiff < nextDiff) {
			headMovementment += prevDiff;
			newHead = prevIndex;
			prevIndex--;
		} else {
			headMovementment += nextDiff;
			newHead = nextIndex;
			nextIndex++;
		}
		total--;
	}

	return headMovementment;
}

/* SCAN - array is already sorted from sstf. SCAN starts from one left of startIndex, 
and continually goes down to zero (if included in randome array or not). Then 
starts at one higher than startIndex and continually goes up to highest value (not 5000) */
int scan(int * inputArray) {
	int i = 0;
	int currVal = 0;
	int tempVal = inputArray[startIndex];
	int diff = 0;

	int headMovement = 0;
	int currIndex = 0;

	for(i = startIndex-1; i >= 0; --i) {
		currVal = inputArray[i];
		diff = abs(tempVal - currVal);
		headMovement += diff;
		tempVal = currVal;
	}

	/* used to subtract value from zero, or just add same value */
	headMovement += tempVal;
	tempVal = 0;

	for(i = startIndex+1; i < numRequests; i++) {
		currVal = inputArray[i];
		diff = abs(currVal - tempVal);
		headMovement += diff;
		tempVal = currVal;
	}

	return headMovement;
}

/* Circular Scan (C-SCAN) - startIndex at startIndex index, increase to upper boundary 
(even if no value at boundary), save boundary value, go to startIndex boundary 
(zero value) increase till last value before startIndex value */
int cscan(int * inputArray) {

	int i = 0;
	int currVal = 0;
	int tempVal = inputArray[startIndex];
	int diff = 0;
	int headMovement = 0;
	int currIndex = 0;
	int upperBound = 4999;

	for(i = startIndex+1; i < numRequests; i++) {
		currVal = inputArray[i];
		diff = abs(tempVal - currVal);
		headMovement += diff;
		tempVal = currVal;
	}

	/* add last val - upper bound, go to and add zero bounday (4999)*/
	headMovement += upperBound - tempVal;
	tempVal = 0;
	headMovement += 4999;

	for(i = 0; i < startIndex; i++) {
		currVal = inputArray[i];
		diff = abs(currVal - tempVal);
		headMovement += diff;
		tempVal = currVal;
	}

	return headMovement;
}



int main (int argc, char *argv[]) {
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

	printf("Start index: %d\n", startIndex);
	printf("startIndex value: %d\n", inputArray[startIndex]);

	printf("Total amount of FCFS head movements: %d\n", fcfs(inputArray));
	printf("Total amount of SSTF head movements: %d\n", sstf(inputArray));
	printf("Total amount of SCAN head movements: %d\n", scan(inputArray));
	printf("Total amount of CSCAN head movements: %d\n", cscan(inputArray));

	fclose(ptr);
	return 0;
}