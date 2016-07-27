/******** atime.h ********/
/* Timothy Ellsworth Bowers
   11 October 2009

   Header file for time-related fuctions needed for various
   astronomical calculations.  All algorithms are from
   Astronomical Algorithms by Jean Meeus, 2nd edition --
   unless otherwise specified.

*/

#if !defined(ATIME_H)

#define ATIME_H 1

#define ATIME_LOCAL 45
#define ATIME_GMT   46

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


/* Function Declarations */
atime_time *atime_get_date_now(int clock_type);
atime_time *atime_get_date(int clock_type, int prevday);
double atime_jd(int year, int month, double day);
double atime_jd_today();
double atime_jd_now();
double atime_get_lst(double longitude);
double atime_lst(double longitude, double jd);
char  *atime_datestring();

#endif
/******** photom.h ********/
/* Timothy Ellsworth Bowers
   9 July 2009

   Header file for photometry-related funtions needed for various
   astronomical calculations.

*/

#if !defined(PHOTOM_H)

#define PHOTOM_H 1


/* Function declarations */
double photom_spect_countrate(double, double, double, double, double);

#endif
/******** strings.h ********/
/* Timothy Ellsworth Bowers
   9 October 2009

   Header file to accompany the strings library routines.

*/

#define STRINGS_LEN 256

int strings_getline(FILE *, char *, size_t *);
/******** catalog.h ********/
/* Timothy Ellsworth Bowers
   11 June 2009

   Header file for catalog-related funtions needed for various
   astronomical calculations.

*/

#if !defined(CATALOG_H)

#define CATALOG_H 1

/* If 'Parallactic Angle'  */
#define CATALOG_PA -500.

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


/* Function declarations */
catalog_lib *catalog_read_lib(char *filename, int *n);
catalog_mmt *catalog_read_mmt(char *filename, int *n);
catalog_tui *catalog_read_tui(char *filename, int *n);

#endif
/******** read_dat_files.h ********/
/* Timothy Ellsworth Bowers
   28 February 2009

   Header file for the read_ncolumn routines, plus countlines().
*/

#if !defined(READ_DAT_FILES_H)

#define READ_DAT_FILES_H 1

int countlines(FILE *fp);
double *read_ncolumn(char *filename, int *N, int m);
double *parse_array(double *total_array, int n_lines, int n_dim);

#endif
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
/******** window.h ********/
/* Timothy Ellsworth Bowers
   20 January 2010

   Header definitions for various apodization window functions.
*/

#if !defined(WINDOW_H)

#define WINDOW_H 1

void window_andrew(double *array, int length, int n);
void window_hann(double *array, int length);
void window_hamming();

#endif
/******** imutil.h ********/
/* Timothy Ellsworth Bowers
   20 January 2010

   Header file for the imutil.c source code.  Image utility routines.

*/

#if !defined(IMUTIL_H)

#define IMUTIL_H 1

double **imutil_alloc_2darray(long *);
void     imutil_free_2darray(double **, long *);
double  *imutil_2d_to_1d(double **, long *);
double **imutil_get_subsection(double **, long *, long *, long *);
void     imutil_transpose(double **, double **, int, int);

#endif
/******** fileio.h ********/
/* Timothy Ellsworth Bowers
   18 September 2008

   Header file that contains the various permutations of file opening,
   and contains the proper error-handling, etc.  This is purely shorthand
   because I am lazy.

*/

#if !defined(FILEIO_H)

#define FILEIO_H 1
FILE *fileopenr(char *);
FILE *fileopenw(char *);
FILE *fileopenrb(char *);
FILE *fileopenwb(char *);
FILE *fileopenwa(char *);

/* Create filename from existing root */
void make_filename(char *,char *,char *,char *);

#endif
/******** astrom.h ********/
/* Timothy Ellsworth Bowers
   11 June 2009

   Header file for astrometry-related functions needed for various
   astronomical calculations.  All algorithms are from
   Astronomical Algorithms by Jean Meeus, 2nd edition --
   unless otherwise specified.

*/

#if !defined(ASTROM_H)

#define ASTROM_H 1

#include <math.h>

/* Radian --> Deg & Deg --> Radian conversions */
#define DEG2RAD M_PI/180.
#define RAD2DEG 180./M_PI
#define TWOPI   (M_PI*2.)
#define OBSFILE "/d1/observing/Library/observatories.dat"

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


/* Function declarations */
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

#endif
