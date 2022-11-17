#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#include "data.h"

//Reads a footpath data from inFile and stores each field to build footpath_t data
//Returns the pointer, or NULL if reading is not successful
footpath_t
*readFootpath(FILE *inFile){
    footpath_t *footpath = NULL;
    assert(inFile);

    //char str1[(MAX_CHAR*3) + 1], str2[MAX_CHAR + 1];
    int footpath_id, mcc_id, mccid_int, statusid, streetid, street_group;
    double deltaz, distance, grade1in, rlmax, rlmin, start_lat, start_lon, end_lat, end_lon;
    char address[MAX_CHAR + 1], clue_sa[MAX_CHAR + 1], asset_type[MAX_CHAR + 1], segside[MAX_CHAR + 1];

    if(fscanf(inFile, "%d", &footpath_id) == 1){
        int field_num = 1; 
        char ch;
        //take the first comma
        fscanf(inFile, "%c", &ch); 
        //take next letter
        fscanf(inFile, "%c", &ch); 

        //read 3 strings, address, clue_sa, and asset_type
        while(field_num <= 3){      
            if(ch == '\"'){
                if(field_num == ADDRESS){
                    fscanf(inFile, "%[^\"]", address);
                    field_num++;
                }   
                else if(field_num == CLUE_SA){
                    fscanf(inFile, "%[^\"]", clue_sa);
                    field_num++;
                }
                else if (field_num ==ASSET_TYPE){
                    fscanf(inFile, "%[^\"]", asset_type);
                    field_num++;
                } 
                fscanf(inFile, "%c", &ch);
                fscanf(inFile, "%c", &ch);
                fscanf(inFile, "%c", &ch);
            }
            else if(ch == ','){
                if(field_num == ADDRESS){
                    address[0] = '\0';
                    field_num++;
                }   
                else if(field_num == CLUE_SA){
                    clue_sa[0] = '\0';
                    field_num++;
                }
                else if (field_num ==ASSET_TYPE){
                    asset_type[0] = '\0';
                    field_num++;
                }
                fscanf(inFile, "%c", &ch);
            }
            else{
                int i=0;          
                while(ch != ','){
                    if(field_num == ADDRESS){
                        address[i] = ch;                    
                    }   
                    else if(field_num == CLUE_SA){
                        clue_sa[i] = ch;                    
                    }
                    else if (field_num ==ASSET_TYPE){
                        asset_type[i] = ch;               
                    }
                    fscanf(inFile, "%c", &ch);
                    i++;
                }
                if(field_num == ADDRESS){
                        address[i] = '\0';                    
                    }   
                else if(field_num == CLUE_SA){
                    clue_sa[i] = '\0';                   
                }
                else if (field_num ==ASSET_TYPE){
                    asset_type[i] = '\0';           
                }       
                    
                field_num++;
                if(field_num <= 3){
                    fscanf(inFile, "%c", &ch);
                }            
            }   
        }

        fscanf(inFile, "%lf,%lf,%lf,%d.0,%d.0,%lf,%lf,", &deltaz,&distance,&grade1in,&mcc_id,&mccid_int, &rlmax,&rlmin);
        //take the next letter
        fscanf(inFile, "%c", &ch); 

        //read the 4th string segside
        if(ch == ','){
            segside[0] = '\0';
        }
        else if(ch == '\"'){
            fscanf(inFile, "%[^\"]", segside);
            fscanf(inFile, "%c", &ch);
            fscanf(inFile, "%c", &ch);
        }
        else{
            int i = 0;
            while(ch != ','){
                segside[i] = ch;
                i++;
                fscanf(inFile, "%c", &ch);
            }
            segside[i] = '\0';
        }

        //read the rest of line
        fscanf(inFile, "%d.0,%d.0,%d.0,%lf,%lf,%lf,%lf", &statusid, &streetid, &street_group, &start_lat,&start_lon, &end_lat, &end_lon);
         
        footpath = (footpath_t*)malloc(sizeof(*footpath));
        assert(footpath != NULL);
        footpath->footpath_id = footpath_id;
        footpath->address = strdup(address);
        assert(footpath->address);
        footpath->clue_sa = strdup(clue_sa);
        assert(footpath->clue_sa);
        footpath->asset_type = strdup(asset_type);
        assert(footpath->asset_type);
        footpath->deltaz = deltaz;
        footpath->distance = distance;
        footpath->grade1in = grade1in;
        footpath->mcc_id = mcc_id;
        footpath->mccid_int = mccid_int;
        footpath->rlmax = rlmax;
        footpath->rlmin = rlmin;
        footpath->segside = strdup(segside);
        assert(footpath->segside);
        footpath->statusid = statusid;
        footpath->streetid = streetid;
        footpath->street_group = street_group;
        footpath->start_lat = start_lat;
        footpath->start_lon = start_lon;
        footpath->end_lat = end_lat;
        footpath->end_lon = end_lon;
               
    }

    //assert(footpath != NULL);
    return footpath;
}

//Prints out the footpath in the agrument onto the outFile
void
printToOutfile(FILE *outFile, footpath_t *footpath){
    fprintf(outFile, "--> footpath_id: %d || address: %s || clue_sa: %s || asset_type: %s || deltaz: %.2lf || distance: %.2lf || grade1in: %.1lf || mcc_id: %d || mccid_int: %d || rlmax: %.2lf || rlmin: %.2lf || segside: %s || statusid: %d || streetid: %d || street_group: %d || start_lat: %lf || start_lon: %lf || end_lat: %lf || end_lon: %lf ||\n", footpath->footpath_id, footpath->address, footpath->clue_sa, footpath->asset_type, footpath->deltaz, footpath->distance, footpath->grade1in, footpath->mcc_id, footpath->mccid_int, footpath->rlmax, footpath->rlmin, footpath->segside, footpath->statusid, footpath->streetid, footpath->street_group, footpath->start_lat, footpath->start_lon, footpath->end_lat, footpath->end_lon);
}
