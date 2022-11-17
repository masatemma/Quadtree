#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>


#define ARGUMENTS_NUM 7
#define INFILE 2
#define OUTFILE 3
#define BOTLEFT_X 4 
#define BOTLEFT_Y 5
#define TOPRIGHT_X 6
#define TOPRIGHT_Y 7
#define MAX_FIGURES 20

#include "quadtree.h"
#include "listops.h"
#include "data.h"

void readBounds(FILE *outFile, quadtreeNode_t *root, node_t *sorted_array[], int data_num);
int main(int argc, char *argv[]){
    
	if(argc < ARGUMENTS_NUM + 1){
		fprintf(stderr, "Usage: %s input_file_name output_file_name\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	//open input file and out file 
	FILE *inFile = fopen(argv[INFILE], "r");
	FILE *outFile = fopen(argv[OUTFILE], "w");
	assert(inFile && outFile);

	//make a rectangle to store the coordinates from the arguments
	rectangle2D_t *rootRectangle = makeRectangle();
	char *endptr;	

	//assign the coordinates as bottom left corner and top right 
	//corner of the quadtree root
	rootRectangle->botLeft->x = strtold(argv[BOTLEFT_X],&endptr);
	rootRectangle->botLeft->y = strtold(argv[BOTLEFT_Y],&endptr);
	rootRectangle->topRight->x = strtold(argv[TOPRIGHT_X],&endptr);
	rootRectangle->topRight->y = strtold(argv[TOPRIGHT_Y],&endptr);
	

	//make a linked list to store location and footpath info
	list_t *list = makeEmptyList();
	//make the root of the quadtree;
	quadtreeNode_t *root = makeQuadtreeRoot();
	root->rectangle = rootRectangle;

	//calculate and store the centre of the rectangle	
	root->rec_centre = calculateRecCentre(root->rectangle);

	//call the method buildQuadtreeAndList to read and insert data points 
	//into the quadtree 
	buildQuadtreeAndList(inFile, root, list);
	assert(list->head != NULL);

	//get the number of nodes in the linked list 
	int dataPoint_num = getDataNum(list);
	//make a sorted arraay for data in the linked list based on the footpath_id
	node_t *sorted_array[dataPoint_num];
	sortArray(list, sorted_array, dataPoint_num);

	//read the bounds from stdin and print out the desired output to stdout 
    readBounds(outFile, root, sorted_array, dataPoint_num);
	
	
	fclose(inFile);
    fclose(outFile);	
	freeList(list);
	freeQuadTree(root);
			
}


//Reads the coordinate pairs from stdin as bounds
//Calls rangeQuery to find and prints out all the footpaths with data points within the bound
void readBounds(FILE *outFile, quadtreeNode_t *root, node_t *sorted_array[], int data_num){
	rectangle2D_t *readBounds = makeRectangle();
	
	char botLeftX[MAX_FIGURES], botLeftY[MAX_FIGURES], topRightX[MAX_FIGURES], topRightY[MAX_FIGURES];
	char *endptr;	

	while(scanf("%s %s %s %s", botLeftX, botLeftY, topRightX, topRightY) == 4){
		readBounds->botLeft->x = strtold(botLeftX,&endptr);
		readBounds->botLeft->y = strtold(botLeftY,&endptr);
		readBounds->topRight->x = strtold(topRightX,&endptr);
		readBounds->topRight->y = strtold(topRightY,&endptr);
		fprintf(outFile, "%s %s %s %s\n", botLeftX, botLeftY, topRightX, topRightY);
		printf("%s %s %s %s --> ", botLeftX, botLeftY, topRightX, topRightY);
		rangeQuery(outFile, readBounds, root, sorted_array, data_num);
		printf("\n");
	}

	free(readBounds->botLeft);
	free(readBounds->topRight);
	free(readBounds);
}



