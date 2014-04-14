/******** image_combine.h ********/
/* Timothy Ellsworth Bowers
   24 March 2009

   Header file for the image_combine.c source code.  Image combination
   routines.

*/

#if !defined(IMAGE_COMBINE_H)

#define IMAGE_COMBINE_H 1

#include <gsl/gsl_histogram.h>

/* Symbolic constants for image_arith(); */
#define IMADD 10
#define IMSUB 11
#define IMMUL 12
#define IMDIV 13

/* Symbolic constants for histogram functions */
#define INT_STEP 20
#define CONTINUOUS 21

double **median_combine(double **input[], long *size, int num);
double **image_arith(double **input1, double **input2, long *size, int op);
double find_mode_2d(double **image, long *size, int typ);
void normalize_image(double **norm, double in_value, long *size);
gsl_histogram *build_histogram(double **image, long *size, int typ);


#endif
