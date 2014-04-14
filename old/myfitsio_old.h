/******** myfitsio.h ********/
/* Timothy Ellsworth Bowers
   8 March 2009

   Header file for the myfitsio.c source code.  Basically, cheap shortcuts
   to make life easier.  See comments in source code.

*/

#if !defined(MYFITSIO_OLD_H)

#define MYFITSIO_OLD_H 1

#include <fitsio.h>

fitsfile *open_fits_read(char *filename, int *status);
double **read_fits_to_array(char *filename, long xystart[2], long xysize[2]);
void write_array_to_fits(char *fileout, char *copyhdr, double **array, 
			 long subsize[2], char *comment);


#endif
