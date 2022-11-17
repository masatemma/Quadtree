#ifndef _DATA_H_
#define _DATA_H_

#include <stdio.h>
#include <stdlib.h>

//constant definitions
#define MAX_CHAR 128
#define MAX_LINE_LEN 512
#define MAX_FOOTPATHS 1000

#define ADDRESS 1
#define CLUE_SA 2
#define ASSET_TYPE 3
#define SEGSIDE 4

#define TRUE 1
#define FALSE 0

#define MAX_FIGURES 20

//footpath data definition
typedef struct {
    int footpath_id;
    char *address;
    char *clue_sa;
    char *asset_type;
    double deltaz;
    double distance;
    double grade1in;
    int mcc_id;
    int mccid_int;
    double rlmax;
    double rlmin;
    char *segside;
    int statusid;
    int streetid;
    int street_group;
    double start_lat;
    double start_lon;
    double end_lat;
    double end_lon;
} footpath_t;

typedef struct point2D{
    long double x;
    long double y;
} point2D_t;

typedef struct dataPoint {
    point2D_t *location;
    footpath_t *footpath;
} dataPoint_t;



//Reads a footpath data from inFile and stores each field to build footpath_t data
//Returns the pointer, or NULL if reading is not successful
footpath_t *readFootpath(FILE *inFile);

//Prints out the footpath in the agrument onto the outFile
void printToOutfile(FILE *outFile, footpath_t *footpath);

#endif
