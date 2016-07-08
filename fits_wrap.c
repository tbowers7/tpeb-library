/******** fits_wrap.c ********/
/* Timothy Ellsworth Bowers
   24 March 2009

   
   CFITSIO is a low-level C library for performing operations on FITS files.
   These library routines are designed to provide a high-level 'wrapper'
   functionality for common FITS I/O tasks.  

   NOTE:  CFITSIO uses a 'human' pixel numbering scheme (i.e. 1-1024) as
   opposed to the more common 'array' numbering scheme (i.e. 0-1023).  These
   library routines assume the calling function is using the 'array' scheme,
   and makes the shift to CFITSIO notation internally.

   This source file contains the following routines:

   fits_wrap_open_read();            Open FITS file for reading ONLY
   fits_wrap_open_readwrite();       Open FITS file for reading and writing
   fits_wrap_read2array();           Reads FITS (subsection) into an array
   fits_wrap_write2file();           Writes array to FITS file

*/

#include <stdlib.h>
#include <unistd.h>
#include <time.h>               // To update FITS headers
#include <fitsio.h>             // Contains CFITSIO routine defs
#include "imutil.h"             // Contains Image Utility routine defs

/* Routine for opening FITS file READONLY with error checking */
fitsfile *fits_wrap_open_read(char *filename, int *status){
  
  fitsfile *fitsfp;
  
  if(fits_open_file(&fitsfp, filename, READONLY, status)){
    fits_report_error(stderr,*status);
    exit(1);
  }
  
  return fitsfp;
}


/* Routine for opening FITS file READONLY with error checking */
fitsfile *fits_wrap_open_readwrite(char *filename, int *status){
  
  fitsfile *fitsfp;
  
  if(fits_open_file(&fitsfp, filename, READWRITE, status)){
    fits_report_error(stderr,*status);
    exit(1);
  }
  
  return fitsfp;
}


/* Routine for reading FITS into array, starting at point, and w/ size */
/* This version assumes an open FITS file, and accepts the fitsfile pointer */
double **fits_wrap_read2array(fitsfile *fitsfp, long xystart[2], long xysize[2],
			      int data_type, int *status){
  
  /* Variable declarations & Initilaztion */
  int i,naxis,bitpix;
  long fpixel[2],naxes[2];
  *status=0;
  
  /* Allocate space for array */
  double **array = imutil_alloc_2darray(xysize);
  
  
  /* Read in FITS file using CFITSIO library routines - w/ error checking */
  /* Note: xystart[2] SHOULD be in 'array' notation (i.e. 0-1023), and this
     routine needs to convert these into 'human' notation (i.e. 1-1024). */
  
  /* ERROR CHECKING & CONVERTING NOTATION */
    /* Get image file parameters */
    if(fits_get_img_param(fitsfp, 2, &bitpix, &naxis, naxes, status)){
      fits_report_error(stderr,*status);
      exit(1);
    }    
    /* Error catching -- number of axes */
    if(naxis != 2){
      fprintf(stderr,"Error: only 2D images are supported.\n\n");
      fits_close_file(fitsfp,status);
      exit(1);
    }

    /* Notation Shift */
    long xstart = xystart[0] + 1;
    long ystart = xystart[1] + 1;
    

  /* Check to see if subsection goes beyond image bounds */
  // First Check x
  if( xstart < 1 || (xstart + xysize[0] - 1) > naxes[0]){
    fprintf(stderr,"Error: subsection is out of bounds\n");
    fits_close_file(fitsfp,status);
    exit(1);
  }
  // Then Check y
  if( ystart < 1 || (ystart + xysize[1] - 1) > naxes[1]){
    fprintf(stderr,"Error: subsection is out of bounds\n");
    fits_close_file(fitsfp,status);
    exit(1);
  }
  
  
  /* Read in the FITS file */  
  fpixel[0] = xstart;              // Set initial coordinate in the x-axis.
  for(i=0; i < xysize[1]; i++){    // Loop over xysize[1] (i.e. y) rows
    fpixel[1] = ystart + i;        // Set initial y-coordinate here...
    
    if(fits_read_pix(fitsfp, data_type, fpixel, xysize[0], NULL,
		     array[i], NULL, status)){
      fits_report_error(stderr,*status);
      break;
    }
  }
  
  return array;
}


/* Function to write array to FITS file, and copy header info from other file */
void fits_wrap_write2file(char *fileout, char *copyhdr, double **array, 
			  long write_size[2], int *status){
  
  /* Variable Declarations & Initializations */
  int k,naxis,bitpix;
  long fpixel[2],naxes[2],bzero;
  char buf_date[FLEN_VALUE],buf_time[FLEN_VALUE],*mod_comm;
  fitsfile *fitsfp,*hdrfp;
  time_t now;
  struct tm *ptr;
  *status = 0;

  /* Open file from which header will be copied */
  hdrfp = fits_wrap_open_read(copyhdr, status);

  /* Check for output file -- create & open for write */
  if(access(fileout,F_OK) == 0)          // Check if output file exists
    remove(fileout);                     // If yes, remove it
  if(fits_create_file(&fitsfp, fileout, status)){
    fits_report_error(stderr,*status);
    exit(1);
  }

  mod_comm = "Modified by fits_wrap routine, TPEB.";
  /* Copy header from copyhdr FITS file. */
  fits_copy_header(hdrfp, fitsfp, status);  // Copy header from input FITS
  fits_get_img_param(hdrfp, 2 ,&bitpix, &naxis, naxes, status);
  fits_read_key(hdrfp, TLONG, "BZERO", &bzero, NULL, status);

  /* Check for needed updates */
    // Writing new FITS as double
    if(bitpix != DOUBLE_IMG){
      bitpix = DOUBLE_IMG;
      fits_update_key(fitsfp, TSHORT, "BITPIX", &bitpix, mod_comm, status);
    }
    
    // check for 'zero level' (needed for type long FITS headers)
    // NOTE: If input header does not contain this keyword (status = 202), 
    // skip update & reset CFITSIO status to zero.
    
    if(bzero != 0 || *status != 202){  
      bzero = 0;
      fits_update_key(fitsfp, TSHORT, "BZERO", &bzero, mod_comm, status);
    }
    *status = 0;                       // Reset Status
    
    // Check new axis lengths are correct
    if(naxes[0] != write_size[0])
      fits_update_key(fitsfp, TULONG,"NAXIS1",&write_size[0],mod_comm,status);
    if(naxes[1] != write_size[1])
      fits_update_key(fitsfp, TULONG,"NAXIS2",&write_size[1],mod_comm,status);
    
    
  /* Write array to file */
  fpixel[0] = 1;
  for(k=0; k < write_size[1]; k++){       // Loop over size[1] rows (i.e. y)
    fpixel[1] = k + 1; 
    if(fits_write_pix(fitsfp, TDOUBLE, fpixel, write_size[0], array[k], 
  		      status)){
      fits_report_error(stderr,*status);
      break;
    }
  }
  
  /* Add/modify keyword for date modified 'DATE-MOD' & 'TIME-MOD' */
  time(&now);
  ptr = localtime(&now);
  strftime(buf_date, FLEN_VALUE, "%Y-%m-%d", ptr);
  strftime(buf_time, FLEN_VALUE, "%H:%M:%S", ptr);
  fits_update_key(fitsfp, TSTRING, "DATE-MOD", buf_date, mod_comm, status);
  fits_update_key(fitsfp, TSTRING, "TIME-MOD", buf_time, NULL, status);
  
  /* Clean up */
  fits_close_file(fitsfp, status);
  fits_close_file(hdrfp,  status);
  
  /* Report any CFITSIO errors to stderr */
  if(!*status)
    fits_report_error(stderr,*status);
  
  return;
}
