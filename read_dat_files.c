/******** read_dat_files.c ********/
/* Timothy Ellsworth Bowers
   28 February 2009

   These subroutines are designed to read in n-column data files, and
   places the columns into the n-D array provided by the calling function.
   The calling function need not pre-allocate space for the array, as it
   will call countlines(), and place the n_lines x n-D data from the file
   into the total_array pointer.  The read_ncolumn() function returns the
   number of lines, and that is used, in conjuntion with the n-D value to
   call the parse_array() function n-D times to put the total_array into
   n-D 1-D vectors to use.  The value col_num is ordinal from 1 -> n-D.

   These routines recognize commented lines starting with '#', and skip them.

   Calling sequence:
     total_array = read_ncolumn(filename,&n_lines,n-D);
     x_array = parse_array(*total_array,n_lines,col_num);
     .
     .
     .

   Uses: fileio.c, countlines() [included in this source file]

   CALLING FUNCTION MUST FREE ARRAYS AND VECTORS ASSOCIATED WITH THE
   ROUTINES IN THIS SOURCE FILE!

*/

#include <stdio.h>
#include <stdlib.h>
#include "fileio.h"          // Contains file I/O shortcuts


/* Get number of lines in the file, return as int */
int countlines(FILE *fp){

  int i=0;
  char line[211];

  while(!feof(fp)){
    fgets(line,210,fp);
    if(line[0] != '#')                 // Skip commented lines when counting
      i++;
  }
  rewind(fp);   // Put file back at starting point for next use
  return i-1;   // Empty line (w/ CR-EOF) at end of file.
}


/* n-column reading routine */
double *read_ncolumn(char *filename, int *N, int m){

  /* Variable Declarations */
  int i,j;
  char s[211];
  double *array;
  FILE *fp;

  /* Open file, and count number of lines */
  fp = fileopenr(filename);
  *N = countlines(fp);
  
  /* Allocate space for array */
  array = (double *)calloc(*N * m, sizeof(double));
  
  /* Read in the file */
  for(i=0;i<*N;i++){                    // Read each line of the file
    for(j=0;j<m;j++){                   // Read each column of the file
      fscanf(fp,"%s",s);
      if(j == 0 && s[0] == '#'){        // If '#' 1st character of line
	i--;                            // Ignore line -- reduce counting index
	fgets(s,210,fp);                // get remainder of line to clear
	break;                          // Move to next line index i
      }
      else
	*(array + (*N) * j + i) = atof(s);
    }
  }
  
  /* Clean up */
  fclose(fp);
  
  return array;
}


/* Parse total_array into individual 1-D arrays */
double *parse_array(double *total_array, int n_lines, int n_dim){
  
  int i;
  double *one_d;
  
  one_d = (double *)calloc(n_lines,sizeof(double));

  for(i=0;i<n_lines;i++)
    one_d[i] = *(total_array + (n_dim-1)*n_lines + i);

  return one_d;
}
