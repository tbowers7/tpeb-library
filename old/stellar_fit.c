/******** stellar_fit.c ********/
/* Timothy Ellsworth Bowers
   3 April 2009
   
   Routine based on program for ASTR 5550, HW#8, Prob #2.  This routine
   fits a 5-parameter Gaussian to a stellar image using a Chi-squared
   minimization scheme.  The routine returns the fit parameters, along with
   uncertainties in a structure defined in the accompanying header file.

   This routine accepts the full 2-D image, the star location, the size of the
   subsection to be taken, and a pointer to the structure of fit parameters.

   Calling sequence:
  
   stellar_fit(image, size, star_center, subsize, &fit_parameters);

*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gsl/gsl_multimin.h>       // Contains GSL's multi-dim minimization
#include <gsl/gsl_vector.h>         // Contains GSL's vector definitions
#include <gsl/gsl_matrix_long_double.h>  // Contains GSL's matrix definitions
#include <gsl/gsl_permutation.h>    // Contains GSL's permutation defs
#include <gsl/gsl_linalg.h>         // Contains GSL's linear algebra defs
#include <mylib/image_statistics.h> // Contains image stats
#include <mylib/image_utils.h>      // Contains image utilities
#include <mylib/stellar_fit.h>      // Stellar Fitting Defs
#include <mylib/fileio.h>

int stellar_fit(double **image, long *size, long *star_cent,
		long *subsize, sf_params *star, sf_noise *noise){
  
  /* Variable Declarations */
  int iter=0,gsl_status,ii,jj,cent_x,cent_y;
  long corner[2];
  struct func_par data;

  /* Variables needed for GSL multi-dimensional minimization */
  const gsl_multimin_fdfminimizer_type *T = // Broyden-Fletcher-Goldfarb-Shanno
    gsl_multimin_fdfminimizer_vector_bfgs2; // a quasi-Newton method
  gsl_multimin_fdfminimizer *s = NULL;
  gsl_multimin_function_fdf chisq_func;
  gsl_vector *theta;

  /* Variables needed for curvature matrix calculation */
  curvature curve;
  
  /* Get subsection of image centered on the stellar position given */
  corner[0] = star_cent[0] - (long)(0.5*subsize[0]);
  corner[1] = star_cent[1] - (long)(0.5*subsize[1]);
  
  data.image = get_subsection(image, size, subsize, corner);

  /* Put subsize into paramter structure for passing to multimin function */
  data.size[0] = subsize[0];
  data.size[1] = subsize[1];

  /* Allocate space for noise array */
  data.noise = alloc_2darray(subsize);
  
  /* Calculate noise in each pixel */
  for(ii=0; ii<subsize[1]; ii++)
    for(jj=0; jj<subsize[0]; jj++)
      data.noise[ii][jj] = sqrt((data.image[ii][jj]) 
				/ noise->gain + 
				noise->ron * noise->ron +
				noise->dc  * noise->dc  );
  /* Set up function for multidimensional minimization*/
  chisq_func.n = 5;
  chisq_func.f = &chisq_f;
  chisq_func.df = &chisq_df;
  chisq_func.fdf = &chisq_fdf;
  chisq_func.params = (void *)&data;
  
  /* Initialize theta -- starting point for multimin */
  /* A   = center pixel value
     B   = passed-though sky value
     x0  = center pixel #
     y0  = center pixel #
     sig = 1/4 average width of subsection */
  theta = gsl_vector_alloc(5);                                   // 5-D
  
  cent_x = (int)(subsize[0] / 2.);
  cent_y = (int)(subsize[1] / 2.);

  gsl_vector_set(theta, 0, data.image[cent_x][cent_y]);          // A
  gsl_vector_set(theta, 1, noise->sky_val);                      // B
  gsl_vector_set(theta, 2, 0.5 * subsize[0]);                    // x0
  gsl_vector_set(theta, 3, 0.5 * subsize[1]);                    // y0
  gsl_vector_set(theta, 4, (subsize[0] + subsize[1])/8.);        // sig
  
  /*
    printf("Starting values: %.2f  %.2f  %.3f  %.3f  %.4f\n",
    gsl_vector_get(theta, 2) + corner[0],
    gsl_vector_get(theta, 3) + corner[1],
    gsl_vector_get(theta, 0),
    gsl_vector_get(theta, 1),
    gsl_vector_get(theta, 4));
  */
  
  /* Do the multidimensional minimization! */
  s = gsl_multimin_fdfminimizer_alloc(T, 5);  // Allocate minimizer
  gsl_multimin_fdfminimizer_set(s, &chisq_func, theta, 0.01, 1e-4);
  
  /* Do iteration of minimizer until the thing converges */
  do{
    iter++;
    gsl_status = gsl_multimin_fdfminimizer_iterate(s);
    
    if(gsl_status)
      break;
    
    gsl_status = gsl_multimin_test_gradient(s->gradient, 1e-3);
    
  }while (gsl_status == GSL_CONTINUE && iter < 100000);
  
  /*  
      printf ("Minimum found at:\n");
      printf ("%5d  %.2f  %.2f  %.3f  %.3f  %.4f  %10.5f\n", iter,
      gsl_vector_get(s->x, 2) + corner[0], 
      gsl_vector_get(s->x, 3) + corner[1], 
      gsl_vector_get(s->x, 0), 
      gsl_vector_get(s->x, 1), 
      fabs(gsl_vector_get(s->x, 4)), 
      s->f);
  */
  
  /* Put minimized values into sf_params structure for returning */
  star->fit.A   = gsl_vector_get(s->x, 0);
  star->fit.B   = gsl_vector_get(s->x, 1);
  star->fit.x0  = gsl_vector_get(s->x, 2) + corner[0];
  star->fit.y0  = gsl_vector_get(s->x, 3) + corner[1];
  star->fit.sig = fabs(gsl_vector_get(s->x, 4));
  
  /* NOTE: Need uncertainty of A for problem #4 */
  /* Find the estimated covariance matrix.  First get the curvature matrix */
  get_curvature_matrix(s->x, (void *)&data, &curve);

  double fisher[] = {curve.a, curve.b, curve.c, curve.d, curve.e,
			  curve.b, curve.f, curve.g, curve.h, curve.i,
			  curve.c, curve.g, curve.j, curve.k, curve.l,
			  curve.d, curve.h, curve.k, curve.m, curve.n,
			  curve.e, curve.i, curve.l, curve.n, curve.o};
  
  //  gsl_matrix *fisher = gsl_matrix_calloc(5,5);
  gsl_matrix_view m = gsl_matrix_view_array(fisher,5,5);
  gsl_matrix *inv    = gsl_matrix_calloc(5,5);
  gsl_permutation *p = gsl_permutation_alloc(5);
  int signum;

  /* Manually put curvature elements into fisher 5x5 matrix */
  


  
  
  /*
    printf("Fisher Diagonals: %.3e  %.3e   %.3e   %.3e   %.3e\n",
    gsl_matrix_get(&m.matrix, 0, 0),
    gsl_matrix_get(&m.matrix, 1, 1),
    gsl_matrix_get(&m.matrix, 2, 2),
    gsl_matrix_get(&m.matrix, 3, 3),
    gsl_matrix_get(&m.matrix, 4, 4));
  */
  
  /* Build the covariance matrix, as the inverse of the curvature matrix */  
  gsl_linalg_LU_decomp(&m.matrix, p, &signum);
  gsl_linalg_LU_invert(&m.matrix, p, inv);

  gsl_permutation_free(p);


  /* Get the diagonal elements of the covariance matrix -- put in sf_params */
  star->sigma.A   = gsl_matrix_get(inv, 0, 0);
  star->sigma.B   = gsl_matrix_get(inv, 1, 1);
  star->sigma.x0  = gsl_matrix_get(inv, 2, 2);
  star->sigma.y0  = gsl_matrix_get(inv, 3, 3);
  star->sigma.sig = gsl_matrix_get(inv, 4, 4);

  /* 
     printf("Covariance Diagonals: %.3e  %.3e   %.3e   %.3e   %.3e\n",
     star->sigma.A, star->sigma.B, 
     star->sigma.x0, star->sigma.y0, star->sigma.sig);
  */
  
  
  /* Clean up */
  free_2darray(data.image, subsize);
  free_2darray(data.noise, subsize);
  
  return 0;
}



/* Function for use with GSL's multidimensional minimization */
double chisq_f(const gsl_vector *v, void *params){
  
  /* Variable Declarations */
  int i,l,m;
  double **model,**junk,chisq=0.;
  struct func_par *p = (struct func_par *)params;

  /* Allocate space for arrays */
  model = alloc_2darray(p->size);
  junk  = alloc_2darray(p->size);

  /* Get model from function */
  build_model(v, model, junk);
  
  /* Calculate chi-sq */
  for(l=0; l<10; l++)
    for(m=0; m<10; m++)
      chisq += pow( (p->image[l][m] - model[l][m]) / p->noise[l][m] , 2.);
  
  return chisq;
}


/* Gradient Calculator for use with GSL's multidimensional minimization */
void chisq_df(const gsl_vector *v, void *params, gsl_vector *df){

  /* Variable Declarations */
  int i,l,m;
  double **model,**expon;
  double A,x0,y0,sig,x,y;
  /* Derivatives */
  double dchi_dA=0.,dchi_dB=0.,dchi_dx0=0.,dchi_dy0=0.,dchi_dsig=0.;
  struct func_par *p = (struct func_par *)params;

  /* Allocate space for arrays */
  model = alloc_2darray(p->size);
  expon = alloc_2darray(p->size);

  /* Get model from function */
  build_model(v, model, expon);

  /* Extract model parameters needed for gradient */
  A   = gsl_vector_get(v, 0);
  x0  = gsl_vector_get(v, 2);
  y0  = gsl_vector_get(v, 3);
  sig = gsl_vector_get(v, 4);
  
  /* Calculate d(chi-sq)/dA */
  for(l=0; l<10; l++)
    for(m=0; m<10; m++)
      dchi_dA += ((p->image[l][m] - model[l][m]) * -2. * expon[l][m]) /
	(p->noise[l][m] * p->noise[l][m]);   // sum(2*exp*(img-model)/noise^2)
  
  /* Calculate d(chi-sq)/dB */
  for(l=0; l<10; l++)
    for(m=0; m<10; m++)
      dchi_dB += ((p->image[l][m] - model[l][m]) * -2.) /
	(p->noise[l][m] * p->noise[l][m]);   // sum(2*(img-model)/noise^2)
  
  /* Calculate d(chi-sq)/dx0 */
  for(l=0; l<10; l++){
    for(m=0; m<10; m++){
      x = (double)l;       
      dchi_dx0 += -2.*A*expon[l][m] * (p->image[l][m] - model[l][m]) *
	(x - x0) / (p->noise[l][m] * p->noise[l][m] * sig * sig);
    }
  }
  
  /* Calculate d(chi-sq)/dy0 */
  for(l=0; l<10; l++){
    for(m=0; m<10; m++){
      y = (double)m;            
      dchi_dy0 += -2.*A*expon[l][m] * (p->image[l][m] - model[l][m]) *
	(y - y0) / (p->noise[l][m] * p->noise[l][m] * sig * sig);
    }
  }
  
  /* Calculate d(chi-sq)/dsig */
  for(l=0; l<10; l++){
    for(m=0; m<10; m++){
      x = (double)l;   
      y = (double)m;   
      dchi_dsig += 2.*A*expon[l][m] * (p->image[l][m] - model[l][m]) *
	-((y-y0)*(y-y0) + (x-x0)*(x-x0)) / (p->noise[l][m] * p->noise[l][m] * 
					    sig * sig * sig);
    }
  }
  
  /* Set these values into the gsl_vector df */
  gsl_vector_set(df, 0, dchi_dA);
  gsl_vector_set(df, 1, dchi_dB);
  gsl_vector_set(df, 2, dchi_dx0);
  gsl_vector_set(df, 3, dchi_dy0);
  gsl_vector_set(df, 4, dchi_dsig);
  
  return;
}


/* Calculates f & df together -- required for GSL multimin */
void chisq_fdf(const gsl_vector *x, void *params, double *f, gsl_vector *df){
  
  *f = chisq_f(x, params); 
  chisq_df(x, params, df);
}


/* Function to build model 2-D Gaussian based on 5 model parameters */
void build_model(const gsl_vector *th, double **m_arr, double **e_arr){

  int l,m;
  double A,B,x0,y0,sig,x,y;
  
  /* Extract model parameters from gsl_vector */
  A   = gsl_vector_get(th, 0);
  B   = gsl_vector_get(th, 1);
  x0  = gsl_vector_get(th, 2);
  y0  = gsl_vector_get(th, 3);
  sig = gsl_vector_get(th, 4);
  
  /* Calculate model image array with given thetas */
  for(l=0; l<10; l++){
    for(m=0; m<10; m++){
      x = (double)l;
      y = (double)m;
      /* Model value */
      m_arr[l][m] = B + A*exp(-((x-x0)*(x-x0) + (y-y0)*(y-y0))/(2.*sig*sig));
      /* Just exponential value */
      e_arr[l][m] = exp(-((x-x0)*(x-x0) + (y-y0)*(y-y0))/(2.*sig*sig));
    }
  }
  
  return;
}

/* Function to find the curvature matrix */
void get_curvature_matrix(const gsl_vector *v, void *params, curvature *fisher){
  
  /* Variable Declarations */
  int i,l,m;
  double **model,**expon,**noise2,**im_mod;
;
  double A,x0,y0,sig,x,y;
  double sig2,sig3,sig4,sumsq;
  /* Derivatives */
  struct func_par *p = (struct func_par *)params;

  /* Allocate space for arrays */
  model  = alloc_2darray(p->size);
  expon  = alloc_2darray(p->size);
  noise2 = alloc_2darray(p->size);
  im_mod = alloc_2darray(p->size);
  
  /* Get model from function */
  build_model(v, model, expon);
  
  /* Extract model parameters needed for curvature */
  A   = gsl_vector_get(v, 0);
  x0  = gsl_vector_get(v, 2);
  y0  = gsl_vector_get(v, 3);
  sig = gsl_vector_get(v, 4);

  /* Repeated values */
  sig2 = sig*sig;
  sig3 = sig2*sig;
  sig4 = sig2*sig2;
  for(l=0; l<p->size[1]; l++){
    for(m=0; m<p->size[0]; m++){
      noise2[l][m] = p->noise[l][m] * p->noise[l][m]; // noise^2
      im_mod[l][m] = p->image[l][m] - model[l][m];    // image - model
    }
  }

  /* Set all 15 variables to zero before continuing... */
  fisher->a = 0.;
  fisher->b = 0.;
  fisher->c = 0.;
  fisher->d = 0.;
  fisher->e = 0.;
  fisher->f = 0.;
  fisher->g = 0.;
  fisher->h = 0.;
  fisher->i = 0.;
  fisher->j = 0.;
  fisher->k = 0.;
  fisher->l = 0.;
  fisher->m = 0.;
  fisher->n = 0.;
  fisher->o = 0.;

  /* Calculate all the second derivatives in one fell swoop! */
  for(l=0; l<p->size[1]; l++){
    for(m=0; m<p->size[0]; m++){
      x     = (double)l;
      y     = (double)m;
      sumsq = ((x-x0)*(x-x0) + (y-y0)*(y-y0));
      
      /* a = dA^2 */
      fisher->a += 2 * expon[l][m] * expon[l][m] / noise2[l][m];
      
      /* b = dAdB */
      fisher->b += 2 * expon[l][m] / noise2[l][m];
      
      /* c = dAdx0 */
      fisher->c += 2 * expon[l][m] * (x-x0) / (noise2[l][m] * sig2) *
	(expon[l][m] - im_mod[l][m]);
      
      /* d = dAdy0 */
      fisher->d += 2 * expon[l][m] * (y-y0) / (noise2[l][m] * sig2) *
	(expon[l][m] - im_mod[l][m]);
      
      /* e = dAdsig */
      fisher->e += 2 * expon[l][m] * sumsq / (noise2[l][m] * sig3) *
	(A * expon[l][m] - im_mod[l][m]);
      
      /* f = dB^2 */
      fisher->f += 2 / noise2[l][m];
      
      /* g = dBdx0 */
      fisher->g += 2 * A * expon[l][m] * (x-x0) / (noise2[l][m] * sig2);
      
      /* h = dBdy0 */
      fisher->h += 2 * A * expon[l][m] * (y-y0) / (noise2[l][m] * sig2);
      
      /* i = dBdsig */
      fisher->i += 2 * A * expon[l][m] * sumsq / (noise2[l][m] * sig3);

      /* j = dx0^2 */
      fisher->j += A * expon[l][m] / (noise2[l][m] * sig2) *
	(  im_mod[l][m] + (x-x0)*(x-x0)/sig2 *
	   (A * expon[l][m] - im_mod[l][m]));
      
      /* k = dx0dy0 */
      fisher->k += A * expon[l][m] / (noise2[l][m] * sig4) *
	(x-x0)*(y-y0) * (A * expon[l][m] - im_mod[l][m]);
      
      /* l = dx0dsig */
      fisher->l += 2 * A * expon[l][m] * (x-x0) / (noise2[l][m] * sig3) *
	(2 * im_mod[l][m] + sumsq / sig2 * (A * expon[l][m] - im_mod[l][m]));

      /* m = dy0^2 */
      fisher->m += A * expon[l][m] / (noise2[l][m] * sig2) *
	(  im_mod[l][m] + (y-y0)*(y-y0)/sig2 *
	   (A * expon[l][m] - im_mod[l][m]));
      
      /* n = dy0dsig */
      fisher->n += 2 * A * expon[l][m] * (y-y0) / (noise2[l][m] * sig3) *
	(2 * im_mod[l][m] + sumsq / sig2 * (A * expon[l][m] - im_mod[l][m]));
      
	
      /* o = dsig^2 */
      fisher->o += 2 * A * expon[l][m] * sumsq / (noise2[l][m] * sig4) *
	(3 * im_mod[l][m] + sumsq / sig2 * (A * expon[l][m] - im_mod[l][m]));

    }
  }
  
  /* Clean Up */
  free_2darray(model,  p->size);
  free_2darray(expon,  p->size);
  free_2darray(noise2, p->size);
  free_2darray(im_mod, p->size);
  

  return;
}
