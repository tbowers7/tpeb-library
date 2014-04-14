/******** plot_wrap.h ********/
/* Timothy Ellsworth Bowers
   17 January 2010

   Header file for the plot_wrap.c source code.  PLOTUTIL is a low-level C
   library for making plots.  The functions declared here provide a high-level 
   'wrapper' functionality for common plotting tasks.  

*/

#if !defined(PLOT_WRAP_H)

#define PLOT_WRAP_H 1

plPlotter *plot_wrap_open_x(plPlotterParams *, int *);
void plot_wrap_close(plPlotter *, int *);
double *plot_wrap_build_axes(plPlotter *, double *, double *, int, int, int *);

#endif
