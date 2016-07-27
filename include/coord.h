/******** coord.h ********/
/* Timothy Ellsworth Bowers
   11 June 2009

   Header file for coordinate-related functions needed for various
   astronomical calculations.

*/

#if !defined(COORD_H)

#define COORD_H 1

/* c_type definitions for deg/dms routines */
#define COORD_RA  360
#define COORD_DEC 180
#define COORD_LST 24

#include "astrom.h"

/* Function declarations */
astrom_coords  coord_parserd(char *);
double         coord_dmstodeg(double dms[3], int c_type);
void           coord_degtodms(double, char *, int c_type);

#endif
