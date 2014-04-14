/******** fits_wrap.h ********/
/* Timothy Ellsworth Bowers
   21 January 2009

   Header file for the fits_wrap.c source code.  CFITSIO is a low-level C
   library for performing operations on FITS files.  The functions declared
   here provide a high-level 'wrapper' functionality for common FITS I/O
   tasks.  

*/

#if !defined(FITS_WRAP_H)

#define FITS_WRAP_H 1

fitsfile *fits_wrap_open_read(char *filename, int *status);
fitsfile *fits_wrap_open_readwrite(char *filename, int *status);
double **fits_wrap_read2array(fitsfile *fitsfp, long xystart[2], long xysize[2],
			      int data_type, int *status);
void fits_wrap_write2file(char *fileout, char *copyhdr, double **array, 
			  long subsize[2], int *status);


#endif
