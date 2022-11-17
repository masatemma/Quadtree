#ifndef _QUADTREE_H_
#define _QUADTREE_H_

#include <stdio.h>
#include <stdlib.h>

#include "data.h"
#include "listops.h"


#define SW_NUM 1
#define NW_NUM 2
#define NE_NUM 3
#define SE_NUM 4
#define DATAPOINT_1 1
#define DATAPOINT_2 2

#define SW_DIR "SW "
#define NW_DIR "NW "
#define NE_DIR "NE "
#define SE_DIR "SE "

typedef struct rectangle2D {
    point2D_t *botLeft;
    point2D_t *topRight;
} rectangle2D_t;

typedef struct quadtreeNode quadtreeNode_t;

struct quadtreeNode{
    int isBlack;
    int isGrey;
    point2D_t *point;
    rectangle2D_t *rectangle;   
    point2D_t *rec_centre;    
    quadtreeNode_t *SW;
    quadtreeNode_t *NW;
    quadtreeNode_t *NE;
    quadtreeNode_t *SE;
};

//Makes an empty quadtree root and returns it
quadtreeNode_t *makeQuadtreeRoot(void);

//make an empty rectangle with memory allocated
rectangle2D_t *makeRectangle(void);

//Takes inFile, root of the quadtree, and the empty linked list as arguments
//Builds quadtree from the data in the inFile
//Firstly, insert read footpaths into linked list by calling InserinToList
//Secondly, makes two data points, coordinate pairs, from one footpath by calling buildNewPoint 
//Then, calls InserNewPoint to insert those two data points into the root 
void buildQuadtreeAndList(FILE *inFile, quadtreeNode_t *root, list_t *list);

//take longitude, and latitude from the footpath as arguments
//make a new point and fill in the location coordinates x and y
//return the new point 
point2D_t *buildNewPoint(long double lon, long double lat);

//Takes a quadtree node and a data point as arguments
//It is a recursive function to insert new data points into the quadtree
//returns TRUE if a new data point is inserted or the same 
//data point was inserted previously and returns FALSE otherwise
int InsertNewPoint(quadtreeNode_t *root, point2D_t *newDataPoint);

//Takes the read footpath from the inFile and the linked list as arguments
// Add two data points for each footpath info read. One for start_lat and start_lon pairs
//and another for end_lat and end_lon pairs.  
void InsertIntoList(footpath_t *newFootpath, list_t *list);

//Takes a rectangle (bound) and a data point as arguments 
//Check if a point is within the rectangle
//Returns TRUE if it is and FALSE if otherwise
int inRectangle(rectangle2D_t *rectangle, point2D_t *point);

//Takes a quadtree node as an argument
//Assigns new bound rectangle and a rectangle centre to each subtree 
void divideQuad(quadtreeNode_t *parent_node);

//Takes a rectangle and a rectangle centre as arguments
//Returns a newly created quadtree based on the arguments
quadtreeNode_t *makeNewQuad(rectangle2D_t *rectangle, point2D_t *rec_centre);

//Takes a rectangle as an argument
//Calculates the centre of the rectangle and returns the centre 
point2D_t *calculateRecCentre(rectangle2D_t *rectangle);

//Takes two data points as arguments
//Returns TRUE if they are the same points 
int samePoint(point2D_t *point1, point2D_t *point2);

//Takes outFile, the root of the node, the coordinates read from stdin in stage 3, the sorted array with addresses of the linked 
//list based on footpath_id, and the number of data in the linked list as arguments
//Finds the point region containing the read coordinate by calling searchPoint
//Prints out the footpaths with the matching data point in that point region 
void printOutMatch(FILE *outFile, quadtreeNode_t *root, point2D_t *read_cord, node_t *sorted_array[], int dataPoint_num);

//Takes the root of the quadtree and the coordinates as arguments
//Searches for the data point that matches with the read coordinate from stdin
//Prints out the path it took to find the data point
//Returns the found data point or returns NULL if the coordinates is not in the quadtree  
point2D_t *searchPoint(quadtreeNode_t *root, point2D_t *read_cord);

//Takes outFile, the rectangle bound read from stdin in stage 4, the root of he quadtree, the 
//sorted array with addresses of the linked list based on footpath_id, and the number of data 
//in the linked list as arguments
//Calls findAllPoints to find all the data points within the bounds
//Prints out all the footpaths with those data points by calling printFootpathsForDict4 
void rangeQuery(FILE *outFile, rectangle2D_t *readBounds, quadtreeNode_t *root, node_t *sorted_array[], int data_num);

//Takes outFile, the rectangle bound read from stdin in stage 4, node of a quadtree, 
//the number of data in the linked list, the sorted_array, and the direction path for the substrees,
//SW NW NE or SE
//It's a recursive fucntion that finds the data points within the bound read from stdin and 
//label it in node of the linked list to be printed later
//It also prints out the path it has taken to stdout while searching for the data points
int findAllPoints(FILE *outFile, rectangle2D_t *readBounds, quadtreeNode_t *root, int data_num, node_t 
*sorted_array[], char direction[]);

//Takes sorted_array of a linked list based on the footpath_id, the number data 
//in the linked list, and the data point that is within the bound of read 
//rectangle from stdin in stage 4 as arguments 
//label the footpaths that has the data point to be printed 
void labelToBePrinted(node_t *sorted_array[], int data_num, point2D_t* point);

//Takes two rectangles as arguments
//Returns TRUE if they overlaps or FALSE otherwise
int rectangleOverlap(rectangle2D_t *rectangle_A, rectangle2D_t *rectangle_B);

//Takes the root of the qudtree an argument
//Frees the quadtree
void freeQuadTree(quadtreeNode_t *root);

#endif
