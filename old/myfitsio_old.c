/******** myfitsio.c ********/
/* Timothy Ellsworth Bowers
   8 March 2009

   These library routines are designed to be a high-level interface between
   the user (me!) and the CFITSIO routines.  They, like some of the other
   routines in this library, are shortcuts because I am lazy, and want I/O
   routines more reminiscent of IDL routines, or something like that.

   This source file contains the following routines:

   open_fits_read();                 Open FITS file for reading
   read_fits_to_array();             Reads FITS (subsection) into an array
   write_array_to_fits();            Writes array to FITS file

*/

#include <stdlib.h>
#include <unistd.h>
#include <fitsio.h>             // Contains CFITSIO routine defs

/* Routine for opening FITS file with error checking */
fitsfile *open_fits_read(char *filename, int *status){
  
  fitsfile *fitsfp;
  
  if(fits_open_file(&fitsfp, filename, READONLY, status)){
    fits_report_error(stderr,*status);
    exit(1);
  }
    
  return fitsfp;
}


/* Routine for reading FITS into array, starting at point, and w/ size */
double **read_fits_to_array(char *filename, long xystart[2], long xysize[2]){
  
  /* Variable declarations */
  int status=0,i,naxis,bitpix;
  long fpixel[2],naxes[2];
  fitsfile *fitsfp;
  
  /* Allocate space for array */
  double **array = (double **)malloc(xysize[0] * sizeof(double *));
  for(i=0; i<xysize[0]; i++)
    array[i] = (double *)calloc(xysize[1], sizeof(double));

  /* Read in FITS file using CFITSIO library routines - w/ error checking */

  /* Open file */
  if(fits_open_file(&fitsfp, filename, READONLY, &status)){
    fits_report_error(stderr,status);
    exit(1);
  }
  /* Get image file parameters */
  if(fits_get_img_param(fitsfp, 2, &bitpix, &naxis, naxes, &status)){
    fits_report_error(stderr,status);
    exit(1);
  }    
  /* Error catching -- number of axes */
  if(naxis != 2){
    fprintf(stderr,"Error: only 2D images are supported.\n\n");
    fits_close_file(fitsfp,&status);
    exit(1);
  }
  /* Check to see if subsection goes beyond image bounds */
  if( xystart[0] < 1 || (xystart[0] + xysize[0]) > naxes[0]){
    fprintf(stderr,"Error: subsection is out of bounds for %s\n",filename);
    fits_close_file(fitsfp,&status);
    exit(1);
  }
  if( xystart[1] < 1 || (xystart[1] + xysize[1]) > naxes[1]){
    fprintf(stderr,"Error: subsection is out of bounds for %s\n",filename);
    fits_close_file(fitsfp,&status);
    exit(1);
  }
  
  /* Read in */  
  fpixel[0] = xystart[0];          // Set initial coordinate in the x-axis.
  for(i=0; i<xysize[0] ;i++){      // Loop over xysize[0] rows
    fpixel[1] = xystart[1] + i;    // Set initial y-coordinate here...
    
    if(fits_read_pix(fitsfp, TDOUBLE, fpixel, xysize[1], NULL,
		     array[i], NULL, &status)){
      fits_report_error(stderr,status);
      break;
    }
  }
  
  fits_close_file(fitsfp,&status);
  return array;
}


/* Function to write array to FITS file, and copy header info from other file */
void write_array_to_fits(char *fileout, char *copyhdr, double **array, 
			 long subsize[2], char *comment){
  
  /* Variable Declarations */
  int status=0,k,naxis;
  long fpixel[2],naxes[2];
  fitsfile *fitsfp,*hdrfp;
  
  /* Open file from which header will be copied */
  if(fits_open_file(&hdrfp, copyhdr, READONLY, &status)){
    fits_report_error(stderr,status);
    exit(1);
  }
  if(access(fileout,F_OK) == 0)          // Check if output file exists
    remove(fileout);                     // If yes, remove it
  if(fits_create_file(&fitsfp, fileout, &status)){
    fits_report_error(stderr,status);
    exit(1);
  }

  /* Update header for proper number of pixles, add comment */
  fits_copy_header(hdrfp, fitsfp, &status);  // Copy header from input FITS
  fits_update_key(fitsfp, TULONG, "NAXIS1", &subsize[0], NULL, &status);
  fits_update_key(fitsfp, TULONG, "NAXIS2", &subsize[1], NULL, &status);
  fits_write_comment(fitsfp,comment,&status);

  /* Write array to file */
  fpixel[0] = 1;
  for(k=0; k<subsize[0]; k++){
    fpixel[1] = k+1; 
    if(fits_write_pix(fitsfp, TDOUBLE, fpixel, subsize[1], array[k], &status)){
      fits_report_error(stderr,status);
      break;
    }
  }
  
  /* Clean up */
  fits_close_file(fitsfp, &status);
  fits_close_file(hdrfp,  &status);
  
  return;
}
