/******** plot_wrap.c ********/
/* Timothy Ellsworth Bowers
   17 February 2010
   
   PLOTUTILS is a low-level C library for creating plots.  These library 
   routines are designed to provide a high-level 'wrapper' functionality for 
   common plotting tasks.

   This source file contains the following routines:

   plot_wrap_open_x();          // Opens an "X" plotting window
   plot_wrap_open_ps();         // Opens a PostScript plotting device
   plot_wrap_close();           // Closes an active plotter

   Each function also returns a status value.  0 = okay.
   
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <plot.h>                     // Contains PLOTUTIL routine defs
#include <gsl/gsl_statistics_double.h>// Contains GSL's statistics functions

/* Routine for opening an X11 plotter with error checking */
plPlotter *plot_wrap_open_x(plPlotterParams *plotter_params, int *status){
  
  /* Variable Declarations */
  plPlotter *plotter;
  
  /* set a Plotter parameter */
  plotter_params = pl_newplparams();
  pl_setplparam(plotter_params, "BITMAPSIZE", "840x525");
  
  /* Create an X drawable plotter, with error checking */
  if ((plotter = pl_newpl_r("X", stdin, stdout, stderr,plotter_params)) 
      == NULL){
    fprintf (stderr, "Couldn't create Plotter\n");
    *status = 1;
  }
  
  /* Open Plotter */
  if(*status == 0)
    if (pl_openpl_r(plotter) < 0){
      fprintf (stderr, "Couldn't open Plotter\n");
      *status = 1;
    }
  
  return plotter;
  
}


/* Routine for closing and deleting the plotter */
void plot_wrap_close(plPlotter *plotter, int *status){
  
  /* Close the Plotter */
  if (pl_closepl_r (plotter) < 0){
    fprintf (stderr, "Couldn't close Plotter\n");
    *status = 1;
  }

  /* Delete the Plotter */
  if (pl_deletepl_r (plotter) < 0){
    fprintf (stderr, "Couldn't delete Plotter\n");
    *status = 1;
  }
    
  return;
}

/* Routine for building plot axes */
/* Arrays x and y are the data to be plotted */
/* nx & ny are the sizes of the x & y arrays */
/* Return value is a 4-element array of [xmin,ymin,xmax,ymax] */
double *plot_wrap_build_axes(plPlotter *plotter, double *x, double *y, 
			     int nx, int ny, int *status){
  
  /* Variable declarations */
  double xmin, xmax, ymin, ymax, xrange, yrange;
  double margin = 0.10;
  double *ranges;

  ranges = (double *)calloc(4, sizeof(double));
  
  /* Get plot ranges */
  gsl_stats_minmax(&xmin, &xmax, x, 1, nx);
  gsl_stats_minmax(&ymin, &ymax, y, 1, ny);
  
  ranges[0] = xmin;
  ranges[1] = ymin;
  ranges[2] = xmax;
  ranges[3] = ymax;
    
  xrange = xmax - xmin;
  yrange = ymax - ymin;
  
  /* Set plot ranges with a margin of 5% of the plot range on either side
     of the coordinate axes */
  pl_fspace_r(plotter, xmin - margin * xrange, ymin - margin * yrange,
	      xmax + margin * xrange, ymax + margin * yrange);
  
  pl_fbox_r(plotter, xmin, ymin, xmax, ymax);
  
  pl_flinewidth_r(plotter, 0.25);
  
  *status = 0;
    
  return ranges;
}
