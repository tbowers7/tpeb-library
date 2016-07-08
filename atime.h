/******** a_time.h ********/
/* Timothy Ellsworth Bowers
   11 October 2009

   Header file for time-related fuctions needed for various
   astronomical calculations.  All algorithms are from
   Astronomical Algorithms by Jean Meeus, 2nd edition --
   unless otherwise specified.

*/

#if !defined(A_TIME_H)

#define A_TIME_H 1

#define A_TIME_LOCAL 45
#define A_TIME_GMT   46

/* Structure Definitions */
typedef struct{
  int year;
  int doy;
  int month;
  int day;
  int hour;
  int min;
  double sec;
} a_time_time;


/* Function Declarations */
a_time_time *a_time_get_date_now(int clock_type);
a_time_time *a_time_get_date(int clock_type, int prevday);
double a_time_jd(int year, int month, double day);
double a_time_jd_today();
double a_time_jd_now();
double a_time_get_lst(double longitude);
double a_time_lst(double longitude, double jd);
char  *a_time_datestring();

#endif
