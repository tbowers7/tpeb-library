/******** image_statistics.c ********/
/* Timothy Ellsworth Bowers
   26 March 2009

   Library routines for computing image statistics.  These routines take in
   2-D arrays and perform various statistical thingies to them.  Routines
   return various and sundry things, depending on purpose of routine.

   This source file contains the following routines:

   image_mean_2d();            Calculates the mean of a 2-D image
   image_stddev_2d();          Calculates the standard deviation of a 2-D image
   image_variance_2d();        Calculates the variance of a 2-D image
   image_mode_2d();            Finds the mode value of a 2-D image
   normalize_image();          Normalizes image by given value
   build_histogram();          Builds a histogram from input image

   NOTE:  When using this source code, always also compile with image_utils.

*/

#include <stdlib.h>
#include <stdio.h>
#include <gsl/gsl_math.h>           // Contains GSL's basic math routines
#include <gsl/gsl_statistics.h>     // Contains GSL's statistics routine defs
#include <gsl/gsl_histogram.h>      // Contains GSL's histogram routine defs
#include <mylib/image_statistics.h> // Contains symbolic contants
#include <mylib/image_utils.h>      // Contains image utility routine defs

/* Function to caluclate the mean of an image */
double image_mean_2d(double **image, long *size){
  
  /* Variable Declarations */
  double mean,*one_d;
  
  /* Read image into single array */
  one_d = twod_to_oned(image, size);
  
  mean = gsl_stats_mean(one_d, 1, size[0]*size[1]);
  
  free(one_d);
  return mean;
}

/* Function to caluclate the standard deviation of an image */
double image_stddev_2d(double **image, long *size){
  
  /* Variable Declarations */
  double stddev,*one_d;
  
  /* Read image into single array */
  one_d = twod_to_oned(image, size);
  
  stddev = gsl_stats_sd(one_d, 1, size[0]*size[1]);
  
  free(one_d);
  return stddev;
}

/* Function to caluclate the standard deviation of an image */
double image_variance_2d(double **image, long *size){
  
  /* Variable Declarations */
  double variance,*one_d;
  
  /* Read image into single array */
  one_d = twod_to_oned(image, size);
  
  variance = gsl_stats_variance(one_d, 1, size[0]*size[1]);
  
  free(one_d);
  return variance;
}

/* Function to find mode of an image */
double image_mode_2d(double **image, long *size, int typ){
  
  /* Variable Declarations */
  size_t index;
  double lower,upper;
  gsl_histogram *h;
  
  /* Build the histogram */
  h = build_histogram(image, size, typ);

  /* Get peak value */
  index = gsl_histogram_max_bin(h);
  gsl_histogram_get_range(h, index, &lower, &upper);

  /* Clean up */
  gsl_histogram_free(h);
  
  return (lower + upper) / 2.;
}

/* Function normalizes image array by given input value */
void normalize_image(double **norm, double in_value, long *size){
  
  /* Variable Declarations */
  int l,m;

  /* Normalize */
  for(l=0;l<size[1];l++)
    for(m=0;m<size[0];m++)
      norm[l][m] /= in_value;

  return;
}

/* Function to define histogram range & build the histogram */
/* IMPORTANT:  histogram allocated here MUST be freed by calling function */
gsl_histogram *build_histogram(double **image, long *size, int typ){
  
  int m;
  long oned_size = size[0]*size[1];
  double max_tot, min_tot;
  double *oned_arr;                 // 1-D array for ease of statistics
  double range;
  gsl_histogram *hist;

  /* Read 2-D image into 1-D array for ease of statistics */
  oned_arr = twod_to_oned(image, size);
  
  /* Find global maximum & minimum */
  max_tot = gsl_stats_max(oned_arr, 1, oned_size);
  min_tot = gsl_stats_min(oned_arr, 1, oned_size);
  
  /* Allocate space for the histogram -- range = max - min */
  /* With extra bin's worth to allow flux values to center in the bins */
  
  /* Use 'typ' to determine type of range to build */
  switch(typ){
    
  case INT_STEP:      // Build bins based on integer bin values
    range = max_tot - min_tot + 1.;
    hist = gsl_histogram_alloc((size_t)range);
    gsl_histogram_set_ranges_uniform(hist, min_tot-0.5, max_tot+0.5);
    break;
    
  case CONTINUOUS:    // Build bins based on continuous bin value set at
    range = GSL_MAX(max_tot - min_tot, 100.);  // integer values, but make
    hist = gsl_histogram_alloc((size_t)range); // sure >= 100 bins.
    gsl_histogram_set_ranges_uniform(hist, min_tot, max_tot);
    break;
    
  default:
    fprintf(stderr,"Error: improper binning type specified -- bomb out.\n");
  }
  
  /* Loop through image, and add pixel values to the histogram */
  for(m=0; m < oned_size; m++)
    gsl_histogram_increment(hist, oned_arr[m]);

  free(oned_arr);
  
  return hist;
}

