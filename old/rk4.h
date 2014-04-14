/******** rk4.h ********/
/* Header function for the rk4 algorithm that defines 'type' of
   derivative function, and declares rk4 function.

   Derivative function need to be of form:

   int deriv(double x, int ny, double *y, double *dydx, void *pass){
     (<code>);
   }

*/

#include <stdlib.h>


typedef int (*derivf)(double, int, double *, double *, void *);


/* Runge-Kutta integrator declaration */


int rk4(derivf, double, double, int, double *, double *, void *);


/* Subroutine: 4th order Runga-Kutta integrator

  One step of 4th order Runge-Kutta integrator.

  Integrates a set of ny equations

  dy
  -- = f[x, y]
  dx

  from x0 to x1.


*/   


int rk4(derivf deriv, double x0, double x1, int ny, double *y0, double *y1, void *pass){

  double dx = x1 - x0;
  double *f0, *f1, *f2, *f3;
  double *p2, *p3, *p4;
  int i;
  
  /* Allocate space for various n-dimensional derivatives */

  f0 = (double *) malloc(ny * sizeof(double));
  f1 = (double *) malloc(ny * sizeof(double));
  f2 = (double *) malloc(ny * sizeof(double));
  f3 = (double *) malloc(ny * sizeof(double));
  
  /* Allocate space for intermediate n-D y-points in R-K algorithm */

  p2 = (double *) malloc(ny * sizeof(double));
  p3 = (double *) malloc(ny * sizeof(double));
  p4 = (double *) malloc(ny * sizeof(double));
  

  /* Loop through y vector */
  
  
  /* Calculate f0 */
  deriv(x0,        ny, y0, f0, 0x0);
  
  /* Calculate f1 */
  for(i=0;i<ny;i++){
    p2[i] = y0[i] + 0.5 * f0[i] * dx;
  }    
  deriv(x0+0.5*dx, ny, p2, f1, 0x0);
  
  /* Calculate f2 */
  for(i=0;i<ny;i++){
    p3[i] = y0[i] + 0.5 * f1[i] * dx;
  }
  deriv(x0+0.5*dx, ny, p3, f2, 0x0);
  
  /* Calculate f3 */
  for(i=0;i<ny;i++){
    p4[i] = y0[i]+f2[i]*dx;
  }    
  deriv(x0+dx,     ny, p4, f3, 0x0);
    
    /* Calculate y1 using 4th order R-K algorithm */ 
  for(i=0;i<ny;i++){
    y1[i] = y0[i] + (f0[i]/6. + f1[i]/3. + f2[i]/3. + f3[i]/6.)*dx;
  }
  
  
  /* Free allocated memory */
  
  free(f0);
  free(f1);
  free(f2);
  free(f3);

  free(p2);
  free(p3);
  free(p4);

  return;
}
