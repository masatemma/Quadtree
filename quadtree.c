#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "quadtree.h"
#include "data.h"
#include "listops.h"


//Makes an empty quadtree root and returns it
quadtreeNode_t 
*makeQuadtreeRoot(void){
    quadtreeNode_t *root;
    root = (quadtreeNode_t*)malloc(sizeof(*root));    
    assert(root!=NULL);
    root->point = NULL;
    root->isBlack = root->isGrey = FALSE;
    root->SW = root->NW = root->SE = root->NE = NULL;
    return root;
}

//Makes an empty rectangle and returns it 
rectangle2D_t 
*makeRectangle(void){
    rectangle2D_t *newRec = (rectangle2D_t*)malloc(sizeof(*newRec));
    newRec->botLeft = (point2D_t*)malloc(sizeof(*newRec->botLeft));
    newRec->topRight = (point2D_t*)malloc(sizeof(*newRec->topRight));
    return newRec;
}

//Takes inFile, root of the quadtree, and the empty linked list as arguments
//Builds quadtree from the data in the inFile
//Firstly, insert read footpaths into linked list by calling InserinToList
//Secondly, makes two data points, coordinate pairs, from one footpath by calling buildNewPoint 
//Then, calls InserNewPoint to insert those two data points into the root   
void 
buildQuadtreeAndList(FILE *inFile, quadtreeNode_t *root, list_t *list){
    char line[MAX_LINE_LEN];
    assert(root!=NULL);
    assert(inFile != NULL);
    // takes the header line
    fscanf(inFile, "%[^\n] ", line);
     //reads footpath data and store them into linked list    
    footpath_t *newFootpath = NULL;
    point2D_t *newPoint1;
    point2D_t *newPoint2;  
   
   //while there's still data in the InFile, keep inserting new 
   //data point into the quadtree
    while((newFootpath = readFootpath(inFile))){
        
        InsertIntoList(newFootpath, list);   

        newPoint1 = buildNewPoint(newFootpath->start_lon, newFootpath->start_lat);   
        newPoint2 = buildNewPoint(newFootpath->end_lon, newFootpath->end_lat);      

        InsertNewPoint(root, newPoint1);                                
        InsertNewPoint(root, newPoint2);              
                     
    }
    

}
//Takes longitude, and latitude from the footpath as arguments
//Makes a new point and fill in the location coordinates x and y
//Returns the new point 
point2D_t 
*buildNewPoint(long double lon, long double lat){
    point2D_t *newPoint = (point2D_t*) malloc(sizeof(*newPoint));
    newPoint->x = lon;
    newPoint->y = lat;

    return newPoint;
}

//Takes a quadtree node and a data point as arguments
//It is a recursive function to insert new data points into the quadtree
//returns TRUE if a new data point is inserted or the same 
//data point was inserted previously and returns FALSE otherwise
int 
InsertNewPoint(quadtreeNode_t *root, point2D_t *newPoint){
    quadtreeNode_t *search_node = root;
    
    //returns FALSE if the new data point is not in the 
    //bound of the current node
    if(!inRectangle(search_node->rectangle, newPoint)){        
        return FALSE;
    }

    //if the node is empty and is a leaf node, insert the new data point
    if(!search_node->isBlack && !search_node->isGrey){  
        search_node->point = newPoint;
        search_node->isBlack = TRUE;
        search_node->isGrey = FALSE;
        return TRUE;
    }

    //if there's a data point in the lead node, divide the node into an internal node 
    //and re-insert the existing new data point
    if(search_node->isBlack && !search_node->isGrey){
        if(samePoint(search_node->point, newPoint)){
            free(newPoint);
            return TRUE;
        }
        
    
        divideQuad(search_node);
        search_node->isGrey = TRUE;
        search_node->isBlack = FALSE;

        point2D_t *tempPoint = search_node->point;
        //it's an internal node so it does not have a data point
        search_node->point = NULL;
        
        if(inRectangle(search_node->SW->rectangle, tempPoint)){
            search_node->SW->point = tempPoint;            
            search_node->SW->isBlack = TRUE;
        }
        else if(inRectangle(search_node->NW->rectangle, tempPoint)){
            search_node->NW->point = tempPoint;
            search_node->NW->isBlack = TRUE;
        }
        else if (inRectangle(search_node->NE->rectangle, tempPoint)){
            search_node->NE->point = tempPoint;
            search_node->NE->isBlack = TRUE;
        }
        else if(inRectangle(search_node->SE->rectangle, tempPoint)){
            search_node->SE->point = tempPoint;
            search_node->SE->isBlack = TRUE;
        }
    }

    //repeat the same process for each subtree
    if(InsertNewPoint(search_node->SW, newPoint)){
        return TRUE;
    }
     if(InsertNewPoint(search_node->NW, newPoint)){
        return TRUE;
    }
     if(InsertNewPoint(search_node->NE, newPoint)){
        return TRUE;
    }
     if(InsertNewPoint(search_node->SE, newPoint)){
        return TRUE;
    }
                      
    return FALSE;   
}

//Takes the read footpath from the inFile and the linked list as arguments
// Add two data points for each footpath info read. One for start_lat and start_lon pairs
//and another for end_lat and end_lon pairs.  
void InsertIntoList(footpath_t *newFootpath, list_t *list){
    
    //create the node for start_lat and start_lon pairs 
    node_t *newNode1 = (node_t*)malloc(sizeof(*newNode1));
    newNode1->dataPoint = (dataPoint_t*)malloc(sizeof(*newNode1->dataPoint));
    newNode1->dataPoint->location = (point2D_t*)malloc(sizeof(*newNode1->dataPoint->location));
    newNode1->dataPoint->location->x = newFootpath->start_lon;
    newNode1->dataPoint->location->y = newFootpath->start_lat;   

    //create the node for end_lat and end_lon pairs
    node_t *newNode2 = (node_t*)malloc(sizeof(*newNode2));
    newNode2->dataPoint = (dataPoint_t*)malloc(sizeof(*newNode2->dataPoint));
    newNode2->dataPoint->location = (point2D_t*)malloc(sizeof(*newNode2->dataPoint->location));
    newNode2->dataPoint->location->x = newFootpath->end_lon;
    newNode2->dataPoint->location->y = newFootpath->end_lat;        
    
    //each node would have the same footpath info
    newNode1->dataPoint->footpath = newFootpath;   
    newNode2->dataPoint->footpath = newFootpath;  

    //toBePrinted is only used in stage 4 for printing out footpath info
    //in footpath_id order 
    newNode1->toBePrinted  = newNode2->toBePrinted = FALSE;

    newNode1->next = newNode2;
    newNode2->next = NULL;

    if(list->foot == NULL){
            list->head = newNode1;
            list->foot = newNode2; 
    }
    else{
        list->foot->next = newNode1;
        list->foot = newNode2;  
    }
}


//Takes a rectangle (bound) and a data point as arguments 
//Check if a point is within the rectangle
//Returns TRUE if it is and FALSE if otherwise
int 
inRectangle(rectangle2D_t *rectangle, point2D_t *point){
    if(rectangle->botLeft->x < point->x && rectangle->topRight->x >= point->x 
    && rectangle->botLeft->y <= point->y && rectangle->topRight->y > point->y){
        return TRUE;
    }
    return FALSE;
}


//Takes a quadtree node as an argument
//Assigns new bound rectangle and a rectangle centre to each subtree 
void 
divideQuad(quadtreeNode_t *parent_node){
    
    //SW's bottom left coordinate is the same as the parent's bottom left 
    //SW's top right coordinate is the same as the centre of the parent's rectangle 
    rectangle2D_t *SW_rec = (rectangle2D_t*)malloc(sizeof(*SW_rec));
    SW_rec->botLeft = (point2D_t*)malloc(sizeof(*SW_rec->botLeft));
    SW_rec->topRight = (point2D_t*)malloc(sizeof(*SW_rec->topRight));

    SW_rec->botLeft->x = parent_node->rectangle->botLeft->x;
    SW_rec->botLeft->y =parent_node->rectangle->botLeft->y;
    SW_rec->topRight->x = parent_node->rec_centre->x;
    SW_rec->topRight->y = parent_node->rec_centre->y;
    parent_node->SW = makeNewQuad(SW_rec,calculateRecCentre(SW_rec));
 

    //NW's bottom left x is the same as the parent's bottom left x
    //NW's bottom left y is the same as the parent's recentagle centre y
    //NW's top right x is the same as the parent's rectangle centre x
    //NW's top right y is the same as the parent's top right y
    rectangle2D_t *NW_rec = (rectangle2D_t*)malloc(sizeof(*SW_rec));
    NW_rec->botLeft = (point2D_t*)malloc(sizeof(*NW_rec->botLeft));
    NW_rec->topRight = (point2D_t*)malloc(sizeof(*NW_rec->topRight));
    NW_rec->botLeft->x = parent_node->rectangle->botLeft->x;
    NW_rec->botLeft->y = parent_node->rec_centre->y;
    NW_rec->topRight->x = parent_node->rec_centre->x;
    NW_rec->topRight->y = parent_node->rectangle->topRight->y;       
    parent_node->NW = makeNewQuad(NW_rec, calculateRecCentre(NW_rec));
 
    //NE's bottom left coordinate is the same as the parent's rectangle centre
    //NE's top right coordinate is the same as the parent's top right 
    rectangle2D_t *NE_rec = (rectangle2D_t*)malloc(sizeof(*SW_rec));
    NE_rec->botLeft = (point2D_t*)malloc(sizeof(*NE_rec->botLeft));
    NE_rec->topRight = (point2D_t*)malloc(sizeof(*NE_rec->topRight));
    NE_rec->botLeft->x = parent_node->rec_centre->x;
    NE_rec->botLeft->y = parent_node->rec_centre->y;
    NE_rec->topRight->x = parent_node->rectangle->topRight->x;
    NE_rec->topRight->y = parent_node->rectangle->topRight->y;
    parent_node->NE = makeNewQuad(NE_rec, calculateRecCentre(NE_rec));
 
    //SE's bottom left x is the same as the parent's rectangle centre x 
    //SE's bottom left y is the same as the parent's bottom left y 
    //SE's top right x is the same as the parent's top right x 
    //SE's top right y is the same as parent's rectangle centre y
    rectangle2D_t *SE_rec = (rectangle2D_t*)malloc(sizeof(*SW_rec));
    SE_rec->botLeft = (point2D_t*)malloc(sizeof(*SE_rec->botLeft));
    SE_rec->topRight = (point2D_t*)malloc(sizeof(*SE_rec->topRight));
    SE_rec->botLeft->x = parent_node->rec_centre->x;
    SE_rec->botLeft->y = parent_node->rectangle->botLeft->y;
    SE_rec->topRight->x = parent_node->rectangle->topRight->x;
    SE_rec->topRight->y = parent_node->rec_centre->y;
    parent_node->SE = makeNewQuad(SE_rec, calculateRecCentre(SE_rec));
   

}

//Takes a rectangle and a rectangle centre as arguments
//Returns a newly created quadtree based on the arguments
quadtreeNode_t 
*makeNewQuad(rectangle2D_t *rectangle, point2D_t *rec_centre){
    quadtreeNode_t *newTree = (quadtreeNode_t*)malloc(sizeof(*newTree));
    newTree->rectangle = rectangle;
    newTree->rec_centre = rec_centre;
    newTree->SW = newTree->NW = newTree->SE = newTree->NE = NULL;
    newTree->isBlack = FALSE;
    newTree->isGrey = FALSE;

    return newTree;
}

//Takes a rectangle as an argument
//Calculates the centre of the rectangle and returns the centre 
point2D_t 
*calculateRecCentre(rectangle2D_t *rectangle){
    point2D_t *rec_centre = (point2D_t*)malloc(sizeof(*rec_centre));
    rec_centre->x = (rectangle->topRight->x + rectangle->botLeft->x) / (long double) 2;
    rec_centre->y = (rectangle->topRight->y + rectangle->botLeft->y) / (long double) 2;
    
    return rec_centre;
}

//Takes two data points as arguments
//Returns TRUE if they are the same points 
int 
samePoint(point2D_t *point1, point2D_t *point2){
    if(point1->x == point2->x && point1->y == point2->y){
        return TRUE;
    }
    return FALSE;
}

//Takes outFile, the root of the node, the coordinates read from stdin in stage 3, the sorted array with addresses of the linked 
//list based on footpath_id, and the number of data in the linked list as arguments
//Finds the point region containing the read coordinate by calling searchPoint
//Prints out the footpaths with the matching data point in that point region 
void
printOutMatch(FILE *outFile, quadtreeNode_t *root, point2D_t *read_cord, node_t *sorted_array[], int dataPoint_num){
        
    quadtreeNode_t *search_node = root;
    point2D_t *matchedPoint = NULL;
    matchedPoint = searchPoint(search_node, read_cord);

    if((matchedPoint !=NULL)){
        printFootpaths(outFile, matchedPoint, sorted_array, dataPoint_num);
    }
     
}

//Takes the root of the quadtree and the coordinates as arguments
//Searches for the data point that matches with the read coordinate from stdin
//Prints out the path it took to find the data point
//Returns the found data point or returns NULL if the coordinates is not in the quadtree  
point2D_t 
*searchPoint(quadtreeNode_t *root, point2D_t *read_cord){    
    if(!inRectangle(root->rectangle, read_cord)){
        return NULL;
    }
    
    while(TRUE){
        if(root->isGrey){
            if(inRectangle(root->SW->rectangle, read_cord)){
                root = root->SW;
                printf("SW ");
            }
            else if(inRectangle(root->NW->rectangle, read_cord)){
                root = root->NW;
                printf("NW ");
            }
            else if(inRectangle(root->NE->rectangle, read_cord)){
                root = root->NE;
                printf("NE ");
            }
            else if(inRectangle(root->SE->rectangle, read_cord)){
                root = root->SE;
                printf("SE ");
            }
        }
        else if(root->isBlack){
            return root->point;          
        }
    }
}

//Takes outFile, the rectangle bound read from stdin in stage 4, the root of he quadtree, the 
//sorted array with addresses of the linked list based on footpath_id, and the number of data 
//in the linked list as arguments
//Calls findAllPoints to find all the data points within the bounds
//Prints out all the footpaths with those data points by calling printFootpathsForDict4 
void 
rangeQuery(FILE *outFile, rectangle2D_t *readBounds, quadtreeNode_t *root, node_t *sorted_array[], int data_num){
     
    findAllPoints(outFile, readBounds, root, data_num, sorted_array, ""); 
    printFootpathsForDict4(outFile, sorted_array, data_num);     
}

//Takes outFile, the rectangle bound read from stdin in stage 4, node of a quadtree, 
//the number of data in the linked list, the sorted_array, and the direction path for the substrees,
//SW NW NE or SE
//It's a recursive fucntion that finds the data points within the bound read from stdin and 
//label it in node of the linked list to be printed later
//It also prints out the path it has taken to stdout while searching for the data points
int
findAllPoints(FILE *outFile, rectangle2D_t *readBounds, quadtreeNode_t *root, int data_num, 
node_t *sorted_array[], char direction[]){

    //if the bounds read from stdin does not overlap the rectangle of the current node
    //return FALSE  
    if(!rectangleOverlap(readBounds, root->rectangle)){
        return FALSE;
    }
    
    //if the node is a leaf node and the point is also in the read bound, 
    //label it to be printed
    //if it's a leaf node but not in the read bound, do nothing
    if(root->isBlack){         
        printf("%s", direction);
        if(inRectangle(readBounds, root->point)){            
            labelToBePrinted(sorted_array, data_num, root->point);        
            return TRUE;
        }
        else{
            return FALSE;
        }   
    }

    //if the node is an internal node, re call the function for each subnode 
    //in the order of SW NW NE SE 
    if(root->isGrey){
        printf("%s", direction);
        findAllPoints(outFile, readBounds, root->SW, data_num, sorted_array, SW_DIR);
        findAllPoints(outFile,readBounds, root->NW, data_num, sorted_array, NW_DIR);
        findAllPoints(outFile,readBounds, root->NE, data_num, sorted_array, NE_DIR);
        findAllPoints(outFile,readBounds, root->SE, data_num, sorted_array, SE_DIR);                   
    } 

    
    return FALSE;
}

//Takes sorted_array of a linked list based on the footpath_id, the number data 
//in the linked list, and the data point that is within the bound of read 
//rectangle from stdin in stage 4 as arguments 
//label the footpaths that has the data point to be printed 
void labelToBePrinted(node_t *sorted_array[], int data_num, point2D_t* point){
    for(int i=0; i<data_num; i++){
        if(sorted_array[i]->dataPoint->location->x == point->x && 
        sorted_array[i]->dataPoint->location->y == point->y){
            if(i + 1 < data_num && sorted_array[i]->dataPoint->footpath->footpath_id == 
            sorted_array[i+1]->dataPoint->footpath->footpath_id && !sorted_array[i+1]->toBePrinted){
                sorted_array[i]->toBePrinted = TRUE;                
            }
            else if(i - 1 >= 0 && sorted_array[i]->dataPoint->footpath->footpath_id == 
            sorted_array[i-1]->dataPoint->footpath->footpath_id && !sorted_array[i-1]->toBePrinted){
                sorted_array[i]->toBePrinted = TRUE;             
            }
            else if(i + 1 < data_num && i - 1 >= 0 && sorted_array[i]->dataPoint->footpath->footpath_id != 
            sorted_array[i-1]->dataPoint->footpath->footpath_id && sorted_array[i]->dataPoint->footpath->footpath_id != 
            sorted_array[i+1]->dataPoint->footpath->footpath_id){
                sorted_array[i]->toBePrinted = TRUE;             
            }
            else if(i + 1 == data_num && sorted_array[i-1]->dataPoint->footpath->footpath_id != 
            sorted_array[i]->dataPoint->footpath->footpath_id){
                sorted_array[i]->toBePrinted = TRUE;  
            }
            else if(i == 0 && sorted_array[i]->dataPoint->footpath->footpath_id != 
            sorted_array[i+1]->dataPoint->footpath->footpath_id){
                sorted_array[i]->toBePrinted = TRUE; 
            }
            
        }
    }
}

//Takes two rectangles as arguments
//Returns TRUE if they overlaps or FALSE otherwise
int 
rectangleOverlap(rectangle2D_t *rectangle_A, rectangle2D_t *rectangle_B){
    if(rectangle_A->botLeft->x > rectangle_B->topRight->x || rectangle_B->botLeft->x > rectangle_A->topRight->x){
        return FALSE;
    }
    else if(rectangle_A->topRight->y < rectangle_B->botLeft->y || rectangle_B->topRight->y < rectangle_A->botLeft->y){
        return FALSE;
    }

    return TRUE;
}

//Takes the root of the qudtree an argument
//Frees the quadtree
void 
freeQuadTree(quadtreeNode_t *root){
    
    if(root->isGrey){
        freeQuadTree(root->SW);
        freeQuadTree(root->NW);
        freeQuadTree(root->NE);
        freeQuadTree(root->SE);
    }

    if(root->isBlack){
        free(root->point);
    }
    
    free(root->rectangle->botLeft);
    free(root->rectangle->topRight);
    free(root->rectangle);
    free(root->rec_centre);
    free(root);
}









