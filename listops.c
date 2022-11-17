#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "listops.h"
#include "data.h"

//Makes an empty linked list with head and foot set to NULL
//Returns the list 
list_t 
*makeEmptyList(void){
    list_t *list;
    list = (list_t*)malloc(sizeof(*list));
    assert(list!=NULL);
    list->head = list->foot = NULL;
    return list;
}

//Takes outFile, the point that maches with the read coordinate from stdin, sorted_array, 
//and the number of data in the linked list as arguments
//Prints out the footpaths that have the point to outFile
void printFootpaths(FILE *outFile, point2D_t *matchedPoint, node_t *sorted_array[], int dataPoint_num){
    for(int i=0; i<dataPoint_num; i++){
        if(sorted_array[i]->dataPoint->location->x == matchedPoint->x && 
        sorted_array[i]->dataPoint->location->y == matchedPoint->y){
            printToOutfile(outFile, sorted_array[i]->dataPoint->footpath);
        }
    }
}

//Takes outFile, sorted_array, and the number of data in the linked list as arguments
//Prints out the footpaths to outFile if it is labelled as toBePrinted and sets it to 
//FALSE after printing
void printFootpathsForDict4(FILE *outFile, node_t *sorted_array[], int dataPoint_num){
    for(int i=0; i<dataPoint_num; i++){
        if(sorted_array[i]->toBePrinted){
            printToOutfile(outFile, sorted_array[i]->dataPoint->footpath);
            sorted_array[i]->toBePrinted = FALSE;                                       
        }   
    }
}


//Takes the linked list as the argument
//Return the size of the linked list 
int 
getDataNum(list_t *list){
    int n=0;
    node_t *node = list->head;
    while(node != NULL){
        n++;
        node = node->next;
    }
    //free(node);
    
    return n;
}

//Assigns pointer of each node into the sorted_array and call arrayBubbleSort
//to sort the array 
void 
sortArray(list_t *list, node_t *sorted_array[], int data_num){

    node_t *cur_node = list->head;
    
    for(int i=0; i<data_num; i++){
        sorted_array[i] = cur_node;
        cur_node = cur_node->next;
    }
    
    //call the method to sort the array of pointers to each node
    arrayBubbleSort(sorted_array, data_num);
    free(cur_node);
}


//Sorts the array based on the footpath_id in ascending order using
//bubble sort algorithm 
void
arrayBubbleSort(node_t *sorted_array[], int data_num){
    node_t *temp;
    for(int i=0; i<data_num; i++){
        for(int j=0; j<data_num - 1; j++){
            if(sorted_array[j]->dataPoint->footpath->footpath_id > 
                sorted_array[j+1]->dataPoint->footpath->footpath_id){
                    temp = sorted_array[j];
                    sorted_array[j] = sorted_array[j+1];
                    sorted_array[j+1] = temp;
            }
        }
    }             
}



//Frees memories allocated for the linked list
void freeList(list_t *list){
    node_t *new_head = list->head;
    node_t *prev_node;
    dataPoint_t *prev_dataPoint;
    int atPoint1 = TRUE;
    while(new_head != NULL){
        prev_node = new_head;
        prev_dataPoint = new_head->dataPoint;
        new_head = new_head->next;

        if(new_head != NULL){            
            if(atPoint1){
                free(prev_dataPoint->footpath->address);
                free(prev_dataPoint->footpath->clue_sa);
                free(prev_dataPoint->footpath->asset_type);
                free(prev_dataPoint->footpath->segside);     
                free(prev_dataPoint->footpath); 
                atPoint1 = FALSE;
            }
            else{
                atPoint1 = TRUE;
            }                                                                     
        }   
        free(prev_dataPoint->location);
        free(prev_dataPoint);
        free(prev_node);        
    }
             
    free(list);    
}
