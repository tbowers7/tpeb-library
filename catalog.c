/******** catalog.c ********/
/* Timothy Ellsworth Bowers
   11 June 2009

   Library routines related to various catalog types, and reading / writing
   those types of catalogs.

*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <tpeb/fileio.h>            // File I/O shortcuts
#include <tpeb/read_dat_files.h>    // Contains countlines()
#include <tpeb/coord.h>             // Coordinate header file
#include <tpeb/catalog.h>           // Catalog header file
#include <tpeb/strings.h>           // Strings header file

/* Function for reading a Master Catalog into an array of catalog_lib
   structures.
   NOTE: RA coordinates from this routine are in ddd.dddddd format!   */
catalog_lib *catalog_read_lib(char *filename, int *n){
  
  /* Variable declrations */
  int i;
  char line[211],*f2,f3[20],f4[20],f5[20],f6[20];
  char f7[20],f9[20],f10[20];
  char *space=" ";
  size_t buflen;
  FILE *fp;
  astrom_coords object;
  catalog_lib *objects,catline;
  
  /* Open catalog file & count # of entries */
  fp = fileopenr(filename);
  *n = countlines(fp);
  
  printf("Catalog %s has %d entries.\n",filename,*n);
    
  /* Allocate space for the structure array */
  objects = (catalog_lib *)malloc(*n * sizeof(catalog_lib)); 
  f2 = (char *)calloc(30, sizeof(char));

  /* FOR loop reading in the lines of the catalog file */
  for(i=0; i<*n; i++){
    
    // Get the line first, check for '#'. then read in 8 fields
    strings_getline(fp,line,&buflen);
    if(line[0] == '#')      // Ignore commented lines
      i--;
    else{                   // If not commented, read in line
      
      /* Parse fields into structure members */
      // Catalog is character-wise laid out
      
      // 15 characters for Object Name
      strncpy(catline.id,line,15);
      catline.id[15] = '\0';
      
      // 15 characters each for R.A. & Dec
      free(f2);
      f2 = (char *)calloc(35, sizeof(char));
      strncpy(f2,line+15,15);
      strncpy(f3,line+30,15);
      f3[15] = '\0';            // And why do I need this???

      // Take RA & Dec, concatenate, send through parserd() & dmstodeg
      strcat(f2,space);
      strcat(f2,f3);

      object = coord_parserd(f2);
      
      catline.ra  = object.ra;
      catline.dec = object.dec;
      
      // 5 characters each for Proper Motions
      strncpy(f4,line+45,5);
      strncpy(f5,line+50,5);
      
      catline.ra_pm  = atof(f4);
      catline.dec_pm = atof(f5);
      
      // 10 characters each for Magnitude and Color
      strncpy(f6,line+55,10);
      strncpy(f7,line+65,10);
      
      catline.mag   = atof(f6);
      catline.color = atof(f7);
      
      // 10 characters for Spectral Type
      strncpy(catline.spectyp,line+75,10);
      
      // 10 characters for the Epoch
      strncpy(f9,line+85,10);

      // Series of if-statements to sort out epoch
      if(strchr(f9,'J') != NULL)
	catline.epoch = 2000.0;
      else if(strchr(f9,'B') != NULL)
	catline.epoch = 1950.0;
      else{
	catline.epoch = atof(f9);
      }
    
      // 10 characters for the Position Angle
      strncpy(f10,line+95,10);
      catline.pa = atof(f10);
      
      // Put this all in the objects structure array
      objects[i] = catline;
    }
  }  
  
  fclose(fp);
  
  return objects;
}

/* Function for reading an MMT-style catalog into an array of catalog_mmt
   structures.
   NOTE: RA coordinates from this routine are in ddd.dddddd format!   */
catalog_mmt *catalog_read_mmt(char *filename, int *n){
  
  /* Variable declrations */
  int i;
  char line[211],f1[20],f2[20],f3[20],f4[20],f5[20],f6[20],f7[20],f8[20];
  char *space=" ",*j2000="J2000.0",*b1950="B1950.0";
  FILE *fp;
  astrom_coords object;
  catalog_mmt *objects,catline;

  /* Open catalog file & count # of entries */
  fp = fileopenr(filename);
  *n = countlines(fp);
  
  printf("Catalog %s has %d entries.\n",filename,*n);
  
  /* Allocate space for the structure array */
  objects = (catalog_mmt *)malloc(*n * sizeof(catalog_mmt)); 
  
  /* FOR loop reading in the lines of the catalog file */
  for(i=0; i<*n; i++){
    
    // Get the line first, check for '#'. then read in 8 fields
    fgets(line,210,fp);
    if(line[0] == '#')      // Ignore commented lines
      i--;
    else{                   // If not commented, read in line
      sscanf(line,"%s %s %s %s %s %s %s %s",f1,f2,f3,f4,f5,f6,f7,f8);
      
      // Take RA & Dec, concatenate, send through parserd() & dmstodeg
      strcat(f2,space);
      strcat(f2,f3);

      object = coord_parserd(f2);

      // Parse fields into structure members
      strncpy(catline.id,line,15);
      catline.id[15] = '\0';
      catline.ra      = object.ra;
      catline.dec     = object.dec;
      catline.ra_pm   = atof(f4);
      catline.dec_pm  = atof(f5);
      catline.mag     = atof(f6);
      strcpy(catline.spectyp,f7);
      
      if(!strcmp(f8,j2000))
	catline.epoch = 2000.0;
      else if(!strcmp(f8,b1950))
	catline.epoch = 1950.0;
      else
	catline.epoch = atof(f8);
      
      objects[i] = catline;

    }
  }  
  
  fclose(fp);

  return objects;
}

/* Function for reading an TUI-style catalog into an array of catalog_tui
   structures.
   NOTE: RA coordinates from this routine are in ddd.dddddd format!   */
catalog_tui *catalog_read_tui(char *filename, int *n){
  
  /* Variable Declarations */
  int i;
  char line[211];//,*space=" ";
  FILE *fp;
  catalog_tui *objects;//,catline;
  
  /* Open catalog file & count # of entries */
  fp = fileopenr(filename);
  *n = countlines(fp);
  
  printf("Catalog %s has %d entries.\n",filename,*n);
  
  /* Allocate space for the structure array */
  objects = (catalog_tui *)malloc(*n * sizeof(catalog_tui)); 
  
  /* FOR loop reading in the lines of the catalog file */
  for(i=0; i<*n; i++){
    
    // Get the line first, check for '#'. then read in 8 fields
    fgets(line,210,fp);
    if(line[0] == '#')      // Ignore commented lines
      i--;
    else{                   // If not commented, read in line
      
     
    } 
  }

  fclose(fp);
  
  return objects;
}
