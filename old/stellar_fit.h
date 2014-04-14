/******** stellar_fit.h ********/
/* Timothy Ellsworth Bowers
   3 April 2009

   Header file for the stellar_fit.c source code.  Stellar image fitting
   routine.

*/

#if !defined(STELLAR_FIT_H)

#define STELLAR_FIT_H 1

#include <gsl/gsl_vector.h>         // Contains GSL vector header types

/* Constants found in previous sections of ASTR5550 project #1 */

/* Various structure definitions for ease of programming */
/* Structure for passing parameters to multidim fitting functions */
struct func_par {
  double **image;                   // Image value per pixel
  double **noise;                   // Noise value per pixel
  long size[2];                     // Size of image aperature used
};

/* Structure containing the 5 parameters (theta) of the Gaussian fit */
struct chisq_fit {
  double A;
  double B;
  double sig;
  double x0;
  double y0;
};  

/* Typedef structure for passing fit parameter to/from calling function */
typedef struct {
  struct chisq_fit fit;
  struct chisq_fit sigma;
} sf_params;

/* Typedef structure for passing noise sources to function */
typedef struct {
  double gain;
  double ron;
  double dc;
  double sky;
  double sky_val;
} sf_noise;

/* Structure for curvature matrix return values */
typedef struct{
  double a;
  double b;
  double c;
  double d;
  double e;
  double f;
  double g;
  double h;
  double i;
  double j;
  double k;
  double l;
  double m;
  double n;
  double o;
} curvature;



/* Function prototypes */
int stellar_fit(double **image, long *size, long *star_cent, 
		long *subsize, sf_params *star, sf_noise *noise);

/* Function to be minimized */
double chisq_f(const gsl_vector *v, void *params);

/* Derivatives functions, needed by GSL's multimin routines */
void chisq_df(const gsl_vector *v, void *params, gsl_vector *df);
void chisq_fdf(const gsl_vector *v, void *params, double *f, gsl_vector *df);

/* Other routines for ease of programming */
void build_model(const gsl_vector *th, double **m_arr, double **e_arr);
void get_curvature_matrix(const gsl_vector *v, void *params, curvature *fisher);

#endif
