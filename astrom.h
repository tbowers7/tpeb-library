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
