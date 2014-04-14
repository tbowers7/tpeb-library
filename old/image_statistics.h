/******** image_statistics.h ********/
/* Timothy Ellsworth Bowers
   26 March 2009

   Header file for the image_statistics.c source code.  Image statistics
   routines.

*/

#if !defined(IMAGE_STATISTICS_H)

#define IMAGE_STATISTICS_H 1

#include <gsl/gsl_histogram.h>

/* Symbolic constants for histogram functions */
#define INT_STEP 20
#define CONTINUOUS 21

double image_mean_2d(double **image, long *size);
double image_stddev_2d(double **image, long *size);
double image_variance_2d(double **image, long *size);
double image_mode_2d(double **image, long *size, int typ);
void normalize_image(double **norm, double in_value, long *size);
gsl_histogram *build_histogram(double **image, long *size, int typ);

#endif
