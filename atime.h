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
