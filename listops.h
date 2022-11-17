#ifndef _LISTOPS_H_
#define _LISTOPS_H_

#include <stdio.h>
#include <stdlib.h>

#include "data.h"



typedef struct node node_t;

struct node {
	dataPoint_t *dataPoint;
    int toBePrinted;
	node_t *next;
};

typedef struct {
	node_t *head;
	node_t *foot;
} list_t;


//Makes an empty linked list with head and foot set to NULL
//Returns the list 
list_t *makeEmptyList(void);

//Takes outFile, the point that maches with the read coordinate from stdin, sorted_array, 
//and the number of data in the linked list as arguments
//Prints out the footpaths that have the point to outFile
void printFootpaths(FILE *outFile, point2D_t *matchedPoint, node_t *sorted_array[], int dataPoint_num);

//Takes outFile, sorted_array, and the number of data in the linked list as arguments
//Prints out the footpaths to outFile if it is labelled as toBePrinted and sets it to 
//FALSE after printing
void printFootpathsForDict4(FILE *outFile, node_t *sorted_array[], int dataPoint_num);

//Takes the linked list as the argument
//Return the size of the linked list 
int getDataNum(list_t *list);

//Assigns pointer of each node into the sorted_array and call arrayBubbleSort
//to sort the array 
void sortArray(list_t *list, node_t *sorted_array[], int data_num);

//Sorts the array based on the footpath_id in ascending order using
//bubble sort algorithm 
void arrayBubbleSort(node_t *sorted_array[], int data_num);

//Frees memories allocated for the linked list
void freeList(list_t *list);

#endif
