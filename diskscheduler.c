#include <stdlib.h>
#include <stdio.h>

#define numCylinders 5000
#define numRequests 1000

int arr[numRequests];
int testArray[numRequests];

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
	int temp = 0;

	for (i = 0; i < numRequests; ++i) {
       	for (j = i + 1; j < numRequests; ++j) {
            if (arr[i] > arr[j]) {
                temp =  arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
    return arr;
}


/* First-Come-First-Serve (fcfs) starts from the index after the starting
index and continually adds the headmovement from the starting index in 
order recieved. If at end of array, startIndex from index zero and continually 
add until starting index */
int fcfs(int *arr) {

	int i = 0, head_movement = 0, this_start = arr[startIndex];

    for(i = startIndex; i < numRequests; i++) {

    	head_movement += abs(arr[i] - this_start);
    }

    for(i = 0; i < startIndex; i++) {

    	head_movement += abs(this_start - arr[i]);
    }
        
    return head_movement;
}

/* Shortest-Seek-Time-First (SSTF) takes the current head position, and
adds the position closest to the current head. This new position now becomes
the head, then this system repeats. 
First we sort the array. Then We have counters for above and below startIndex 
index that we decrement if used. Once these equal to REQUEST-2 (excluding 
startIndex index) we exit. */
int sstf(int * arr) {

	arr = sort_array();

	int small_i = startIndex-1, large_i = startIndex+1;
	int small_diff = 0, large_diff = 0;
	int head_movement = 0, total = numRequests-2, new_head = startIndex, head_value = arr[startIndex];
	
	while(total >= 0) {

		small_diff = abs(arr[new_head] - arr[small_i]);
		large_diff = abs(arr[large_i] - arr[new_head]);

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

/* SCAN - array is already sorted from sstf. SCAN starts from one left of startIndex, 
and continually goes down to zero (if included in randome array or not). Then 
starts at one higher than startIndex and continually goes up to highest value (not 5000) */
int scan(int * ranArray) {

	int i = 0, curr_val = 0, sav_val = arr[startIndex], difference = 0;
	int head_movement = 0, curr_i = 0;

	for(i = startIndex-1; i >= 0; --i) {

		curr_val = arr[i];
		difference = abs(sav_val - curr_val);
		head_movement += difference;
		sav_val = curr_val;

	}

	/* used to subtract value from zero, or just add same value */
	head_movement += sav_val;
	sav_val = 0;

	for(i = startIndex+1; i < numRequests; i++) {

		curr_val = arr[i];
		difference = abs(curr_val - sav_val);
		head_movement += difference;
		sav_val = curr_val;

	}

	return head_movement;

}

/* Circular Scan (C-SCAN) - startIndex at startIndex index, increase to upper boundary 
(even if no value at boundary), save boundary value, go to startIndex boundary 
(zero value) increase till last value before startIndex value */
int cscan(int * ranArray) {

	int i = 0, curr_val = 0, sav_val = arr[startIndex], difference = 0;
	int head_movement = 0, curr_i = 0, upper_bound = 4999;

	for(i = startIndex+1; i < numRequests; i++) {

		curr_val = arr[i];
		difference = abs(sav_val - curr_val);
		head_movement += difference;
		sav_val = curr_val;

	}

	/* add last val - upper bound, go to and add zero bounday (4999)*/
	head_movement += upper_bound - sav_val;
	sav_val = 0;
	head_movement += 4999;

	for(i = 0; i < startIndex; i++) {

		curr_val = arr[i];
		difference = abs(curr_val - sav_val);
		head_movement += difference;
		sav_val = curr_val;

	}

	return head_movement;
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
		fscanf(ptr, "%d", &arr[i]); 
	}

	printf("Start index: %d\n", startIndex);
	printf("startIndex value: %d\n", arr[startIndex]);

	printf("Total amount of FCFS head movements: %d\n", fcfs(arr));
	printf("Total amount of SSTF head movements: %d\n", sstf(arr));
	printf("Total amount of SCAN head movements: %d\n", scan(arr));
	printf("Total amount of CSCAN head movements: %d\n", cscan(arr));

	fclose(ptr);
	return 0;
}