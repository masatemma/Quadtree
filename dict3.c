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

void readCoordinates(FILE *outFile, quadtreeNode_t *root, node_t *sorted_array[], int data_num);

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

	//storing coordinates of bottom left corner and top right corner
	rootRectangle->botLeft->x = strtold(argv[BOTLEFT_X],&endptr);
	rootRectangle->botLeft->y = strtold(argv[BOTLEFT_Y],&endptr);
	rootRectangle->topRight->x = strtold(argv[TOPRIGHT_X],&endptr);
	rootRectangle->topRight->y = strtold(argv[TOPRIGHT_Y],&endptr);
	

	//make a linked list to store coordinates each point in the footpath
	//meaning there will be two nodes for one footpath in the linked list
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

	//read the coordinate pairs from stdin and print out the desired output to stdout 
	readCoordinates(outFile, root, sorted_array, dataPoint_num);
	
	
	fclose(inFile);
    fclose(outFile);

	freeQuadTree(root);
	freeList(list);
			
}

// Reads coordinate pairs from stdin and call the method printOutMatch to find the matching point
void readCoordinates(FILE *outFile, quadtreeNode_t *root, node_t *sorted_array[], int dataPoint_num){
	char x_string[MAX_FIGURES], y_string[MAX_FIGURES];
    double cord_x, cord_y;
	char *endptr;	
	point2D_t *read_cord = (point2D_t*)malloc(sizeof(*read_cord));

	while(scanf("%s %s", x_string, y_string)==2){
		cord_x = strtod(x_string,&endptr);
		cord_y = strtod(y_string,&endptr);
		read_cord->x = cord_x;
		read_cord->y = cord_y;
		fprintf(outFile, "%s %s\n", x_string, y_string);
		printf("%s %s --> ", x_string, y_string);
		printOutMatch(outFile, root, read_cord, sorted_array, dataPoint_num);
		printf("\n");
	}
	free(read_cord);
	
}

