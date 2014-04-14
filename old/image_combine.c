/******** image_combine.c ********/
/* Timothy Ellsworth Bowers
   24 March 2009

   Library routines for image combining.  These routines take in 2-D arrays
   and combine them using the method in the routine name.  Routines return
   a 2-D array of the same size as the input arrays.

   This source file contains the following routines:

   median_combine();           Median combine image set
   image_arith();              Image arithmetic -- op via int KEYWORD

*/

#include <stdlib.h>
#include <stdio.h>
#include <gsl/gsl_math.h>          // Contains GSL's IS_EVEN macro
#include <gsl/gsl_sort.h>          // Contains GSL's sorting routines
#include <gsl/gsl_statistics.h>    // Contains GSL's statistics routine defs
#include <mylib/image_combine.h>   // Contains symbolic contants
#include <mylib/image_utils.h>     // Contains image utilty routines

/* Routine for median combining of image arrays */
double **median_combine(double **input[], long *size, int num){
  
  /* Variable Declarations & Allocation */
  int k,l,m;
  double pixel_comb[num];
  double **output = alloc_2darray(size);
  
  for(l=0;l<size[1];l++){
    for(m=0;m<size[0];m++){
      for(k=0;k<num;k++)
	pixel_comb[k] = input[k][l][m];          // Pixel from (k+1)th frame
      gsl_sort(pixel_comb, 1, num);              // Sort pixels
      output[l][m] = gsl_stats_median_from_sorted_data(pixel_comb, 1, num);
    }
  }
  return output;
}

/* Function to do arithmetic on two images */
double **image_arith(double **input1, double **input2, long *size, int op){
  
  /* Variable Declarations & Allocations */
  int i,l,m;
  double **output = alloc_2darray(size);
  
  /* Use switch statement to choose operation */ 
  switch(op){
    
  case IMADD:
    for(l=0;l<size[1];l++)
      for(m=0;m<size[0];m++)
	output[l][m] = input1[l][m] + input2[l][m];
    break;
    
  case IMSUB:
    for(l=0;l<size[1];l++)
      for(m=0;m<size[0];m++)
	output[l][m] = input1[l][m] - input2[l][m];
    break;
    
  case IMMUL:
    for(l=0;l<size[1];l++)
      for(m=0;m<size[0];m++)
	output[l][m] = input1[l][m] * input2[l][m];
    break;
    
  case IMDIV:
    for(l=0;l<size[1];l++)
      for(m=0;m<size[0];m++)
	output[l][m] = input1[l][m] / input2[l][m];
    break;
    
  default:
    fprintf(stderr,"Error: improper image operation specified -- bomb out.\n");
  }    
  
  return output;
}
