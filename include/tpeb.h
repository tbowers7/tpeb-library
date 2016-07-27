/* TPEB Library
 * 
 * A library of routines for astronomical use or wrappers around lower-level
 * libraries for ease of use.
 * 
 * Timothy P. Ellsworth Bowers
 * 
 * FILE: tpeb.h
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */


#ifndef _TPEB_H
#define _TPEB_H

/* THIS FILE IS A COMPOSITE OF HEADERS FOR THE INDIVIDUAL SOURCE CODE FILES. */


/******** astrom.h ********/
/* Timothy Ellsworth Bowers
   11 June 2009

   Header file for astrometry-related functions needed for various
   astronomical calculations.  All algorithms are from
   Astronomical Algorithms by Jean Meeus, 2nd edition --
   unless otherwise specified.

*/
/******** atime.h ********/
/* Timothy Ellsworth Bowers
   11 October 2009

   Header file for time-related fuctions needed for various
   astronomical calculations.  All algorithms are from
   Astronomical Algorithms by Jean Meeus, 2nd edition --
   unless otherwise specified.

*/
/******** catalog.h ********/
/* Timothy Ellsworth Bowers
   11 June 2009

   Header file for catalog-related funtions needed for various
   astronomical calculations.

*/
/******** coord.h ********/
/* Timothy Ellsworth Bowers
   11 June 2009

   Header file for coordinate-related functions needed for various
   astronomical calculations.

*/
/******** fileio.h ********/
/* Timothy Ellsworth Bowers
   18 September 2008

   Header file that contains the various permutations of file opening,
   and contains the proper error-handling, etc.  This is purely shorthand
   because I am lazy.

*/
/******** fits_wrap.h ********/
/* Timothy Ellsworth Bowers
   21 January 2009

   Header file for the fits_wrap.c source code.  CFITSIO is a low-level C
   library for performing operations on FITS files.  The functions declared
   here provide a high-level 'wrapper' functionality for common FITS I/O
   tasks.  

*/
/******** imutil.h ********/
/* Timothy Ellsworth Bowers
   20 January 2010

   Header file for the imutil.c source code.  Image utility routines.

*/
/******** photom.h ********/
/* Timothy Ellsworth Bowers
   9 July 2009

   Header file for photometry-related funtions needed for various
   astronomical calculations.

*/
/******** read_dat_files.h ********/
/* Timothy Ellsworth Bowers
   28 February 2009

   Header file for the read_ncolumn routines, plus countlines().
*/
/******** strings.h ********/
/* Timothy Ellsworth Bowers
   9 October 2009

   Header file to accompany the strings library routines.

*/
/******** window.h ********/
/* Timothy Ellsworth Bowers
   20 January 2010

   Header definitions for various apodization window functions.
*/


/* Includes for external headers */
#ifndef HAVE_MATH_H
#define HAVE_MATH_H
#include <math.h>
#endif

#ifndef HAVE_FITSIO_H
#define HAVE_FITSIO_H
#include <fitsio.h>
#endif

/* Define Constants */
#define ATIME_LOCAL 45
#define ATIME_GMT   46
  /* c_type definitions for deg/dms routines */
#define COORD_RA  360
#define COORD_DEC 180
#define COORD_LST 24
  /* If 'Parallactic Angle'  */
#define CATALOG_PA -500.
#define STRINGS_LEN 256
  /* Radian --> Deg & Deg --> Radian conversions */
#define DEG2RAD M_PI/180.
#define RAD2DEG 180./M_PI
#define TWOPI   (M_PI*2.)
#define OBSFILE "/d1/observing/Library/observatories.dat"



/* Structure Definitions */
typedef struct{
  int year;
  int doy;
  int month;
  int day;
  int hour;
  int min;
  double sec;
} atime_time;

/* Typedef catalog structures */

// Library Preferred catalog structure
typedef struct {
  char   id[50];
  double ra;
  double dec;
  float  ra_pm;
  float  dec_pm;
  float  mag;
  float  color;
  char   spectyp[50];
  double epoch;
  float  pa;
} catalog_lib;

// MMT-style catalog structure
typedef struct {
  char   id[50];
  double ra;
  double dec;
  float  ra_pm;
  float  dec_pm;
  float  mag;
  char   spectyp[50];
  double epoch;
} catalog_mmt;

// APO / TUI catalog structure
typedef struct {
  char   id[50];
  double ra;
  double dec;
  char   keywords[100];
} catalog_tui;

// Betsy Green pccb Catalog structure
typedef struct {
  char   id[50];
  double ra;
  double dec;
  double epoch;
  float  v_mag;
  float  b_v;
} catalog_bg;

/* Typedef structures for coordinates */

// Celestial Coordinates
typedef struct {
  double ra;            // RA measured in DDD.DDDDDDD
  double dec;           // Dec measured in +-DD.DDDDDDDD
} astrom_coords;

// Alt-Az Coordinates
typedef struct {
  double alt;           // Position altitude
  double az;            // Position azimuth (E of N)
} astrom_altaz;

// Earth Locations
typedef struct {
  int    num;           // Catalog Number
  char   name[100];     // Location name
  double lat;           // Location latitude
  double lon;           // Location longitude -- measured +WEST
  int    tz;            // Location time zone (i.e. GMT-7)
} astrom_location;

// Rise/Transit/Set Day Fractions
typedef struct {
  double m0;
  double m1;
  double m2;
} astrom_rst;







/* Function Declarations */
atime_time *atime_get_date_now(int clock_type);
atime_time *atime_get_date(int clock_type, int prevday);
double atime_jd(int year, int month, double day);
double atime_jd_today();
double atime_jd_now();
double atime_get_lst(double longitude);
double atime_lst(double longitude, double jd);
char  *atime_datestring();

double photom_spect_countrate(double, double, double, double, double);

int strings_getline(FILE *, char *, size_t *);

catalog_lib *catalog_read_lib(char *filename, int *n);
catalog_mmt *catalog_read_mmt(char *filename, int *n);
catalog_tui *catalog_read_tui(char *filename, int *n);

int countlines(FILE *fp);
double *read_ncolumn(char *filename, int *N, int m);
double *parse_array(double *total_array, int n_lines, int n_dim);

fitsfile *fits_wrap_open_read(char *filename, int *status);
fitsfile *fits_wrap_open_readwrite(char *filename, int *status);
double **fits_wrap_read2array(fitsfile *fitsfp, long xystart[2], long xysize[2],
			      int data_type, int *status);
void fits_wrap_write2file(char *fileout, char *copyhdr, double **array, 
			  long subsize[2], int *status);

astrom_coords  coord_parserd(char *);
double         coord_dmstodeg(double dms[3], int c_type);
void           coord_degtodms(double, char *, int c_type);

void window_andrew(double *array, int length, int n);
void window_hann(double *array, int length);
void window_hamming();

double **imutil_alloc_2darray(long *);
void     imutil_free_2darray(double **, long *);
double  *imutil_2d_to_1d(double **, long *);
double **imutil_get_subsection(double **, long *, long *, long *);
void     imutil_transpose(double **, double **, int, int);

FILE *fileopenr(char *);
FILE *fileopenw(char *);
FILE *fileopenrb(char *);
FILE *fileopenwb(char *);
FILE *fileopenwa(char *);

void make_filename(char *,char *,char *,char *);

double astrom_ang_sep(const astrom_coords *, const astrom_coords *);
double astrom_ang_sep2(const astrom_coords *, const astrom_coords *);
double astrom_rel_pos_ang(const astrom_coords *, const astrom_coords *);
void   astrom_precess2(astrom_coords *, double, double);
double astrom_get_lha(double, double);
void   astrom_get_altaz(astrom_coords *, astrom_location *, astrom_altaz *,
			double);
astrom_rst      *astrom_get_rst(astrom_coords *,astrom_location *, 
				double, double, int *);
astrom_location *astrom_read_observatories(char *, int *);
astrom_location  astrom_get_location(char *);


#endif  /* _TPEB_H */
