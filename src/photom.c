/******** photom.c ********/
/* Timothy Ellsworth Bowers
   9 July 2009

   Series of library functions related to photometry and other observing
   related exposure-time calculation functions.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define GSL_CONST_CGSM_PLANCKS_CONSTANT_H (6.62606896e-27) /* g cm^2 / s */

/* Function to calculate the countrate (photons / sec) per pixel of a 
   spectrograph.
   This function does NOT take into account the efficiency of the spectrograph
   (typically ~10%) 
   NOTE:     lambda in um, D in cm
   Algorithm from conversation w/ John Bally (and condensed) on 4 July 2009
*/
double photom_spect_countrate(double mag, double lambda, double R, 
			      double pixres, double D){
  
  /* Variable Declarations */
  double h = GSL_CONST_CGSM_PLANCKS_CONSTANT_H;
  double vega_v = 3650.e-23;                     // erg/s/cm^2/Hz
  double v_lambda = 0.5480;                      // um
  
  return (vega_v / h) * (v_lambda * v_lambda * M_PI * D * D) /
    (4. * R * lambda * lambda * pow(2.5,mag) * pixres);
  
}
