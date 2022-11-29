/*
Drew Bozarth | Romtin Rezvani
2373658 | 2341841
dbozarth@chapman.edu | rrezvani@chapman.edu
CPSC 380-01
Assignment 7 - Disk Scheduling Algorithms - diskscheduler.c
*/

#include <stdio.h>
#include <stdlib.h>

#define CYLINDERS 5000
#define REQUESTS 1000

int initialPos = 0;

int input_array[REQUESTS];

FILE *ptr;

/*
Uses bubble sort method to sort the array of requests.
Used in the Shortest-Seek-Time-First algorithm, and is already sorted
after that algorithm to be used in SCAN algorithm.
*/
int* sort_array() {

	int swap = 0, i = 0, j = 0;

	for (i = 0; i < REQUESTS; ++i) {

       	for (j = i + 1; j < REQUESTS; ++j) {

            if (input_array[i] > input_array[j]) {

                swap =  input_array[i];
                input_array[i] = 	input_array[j];
                input_array[j] = swap;
            }
        }
    }

    return input_array;
}


/* First-Come-First-Serve (fcfs) executes queued requests in the order of their
arrival. Begins at the index after the initial position index and tallies up the
headmovement which is the difference between the initial position and the current
incrementing index. Once it reaches the end of the array, it starts from index 0
and adds the difference from the initial position to the current index, until it
reaches the initial position index.
*/
int fcfs(int *input_array) {

	int i = 0;
	head_movement = 0;
	start_pos = input_array[initialPos];

    for(i = initialPos; i < REQUESTS; i++) {
    	head_movement += abs(input_array[i] - start_pos);
    }

    for(i = 0; i < initialPos; i++) {
    	head_movement += abs(start_pos - input_array[i]);
    }
        
    return head_movement;
}

/* Shortest-Seek-Time-First (SSTF) takes the current head position, and
adds the position closest to the current head. This new position now becomes
the head, then this system repeats. 
First we sort the array. Then We have counters for above and below initial pos
index that we decrement if used. Once these equal to REQUEST-2 (excluding 
initial pos index) we exit. */
int sstf(int * input_array) {

	input_array = sort_array();

	int small_i = initialPos-1, large_i = initialPos+1;
	int small_diff = 0, large_diff = 0;
	int head_movement = 0, total = REQUESTS-2, new_head = initialPos, head_value = input_array[initialPos];
	
	while(total >= 0) {
		small_diff = abs(input_array[new_head] - input_array[small_i]);
		large_diff = abs(input_array[large_i] - input_array[new_head]);

		if(small_diff < large_diff) {

			head_movement += small_diff;
			new_head = small_i;
			small_i--;
			
		} else {

			head_movement += large_diff;
			new_head = large_i;
			large_i++;
		}

		total--;
	}

	return head_movement;
}

/* SCAN - array is already sorted from sstf. SCAN starts from one left of initial pos, 
and continually goes down to zero (if included in randome array or not). Then 
starts at one higher than initial pos and continually goes up to highest value (not 5000) */
int scan(int * ranArray) {

	int i = 0, curr_val = 0, sav_val = input_array[initialPos], difference = 0;
	int head_movement = 0, curr_i = 0;

	for(i = initialPos-1; i >= 0; --i) {

		curr_val = input_array[i];
		difference = abs(sav_val - curr_val);
		head_movement += difference;
		sav_val = curr_val;

	}

	/* used to subtract value from zero, or just add same value */
	head_movement += sav_val;
	sav_val = 0;

	for(i = initialPos+1; i < REQUESTS; i++) {

		curr_val = input_array[i];
		difference = abs(curr_val - sav_val);
		head_movement += difference;
		sav_val = curr_val;

	}

	return head_movement;

}

/* Circular Scan (C-SCAN) - start at initial pos index, increase to upper boundary 
(even if no value at boundary), save boundary value, go to initial pos boundary 
(zero value) increase till last value before initial pos value */
int cscan(int * ranArray) {

	int i = 0, curr_val = 0, sav_val = input_array[initialPos], difference = 0;
	int head_movement = 0, curr_i = 0, upper_bound = 4999;

	for(i = initialPos+1; i < REQUESTS; i++) {

		curr_val = input_array[i];
		difference = abs(sav_val - curr_val);
		head_movement += difference;
		sav_val = curr_val;

	}

	/* add last val - upper bound, go to and add zero bounday (4999)*/
	head_movement += upper_bound - sav_val;
	sav_val = 0;
	head_movement += 4999;

	for(i = 0; i < initialPos; i++) {

		curr_val = input_array[i];
		difference = abs(curr_val - sav_val);
		head_movement += difference;
		sav_val = curr_val;

	}

	return head_movement;
}





int main (int argc, char *argv[]) {

	int i = 0;

	if(argc != 3) {
		printf("Please retry and make sure the first argument is the initial position and the second argument is the cylinder file\n");
		exit(-1);
	} else {
		// If correct amount of arguments given, the first argument is the initial postiion
		initialPos = atoi(argv[1]);
	}

	// try to open the file given as the second argument
	ptr = fopen(argv[2], "r");

	if (ptr == NULL) {
		printf("Please retry and make sure the file name is correct and it exists.\n");
		exit(1);
	}


	for (int i = 0; i < REQUESTS; ++i) {
		fscanf(ptr, "%d", &input_array[i]); 
	}

	for (int i = 0; i < REQUESTS; ++i) {
		printf("Num: %d\n", input_array[i]); 
	}

	printf("\nInitial Position Index: %d, Initial Position Value: %d\n\n", initialPos, input_array[initialPos]);

	printf("FCFS cylinder requests: %d\n", fcfs(input_array));
	printf("SSTF cylinder requests: %d\n", sstf(input_array));
	printf("SCAN cylinder requests: %d\n", scan(input_array));
	printf("CSCAN cylinder requests: %d\n", cscan(input_array));


	fclose(ptr);
	return 0;
}