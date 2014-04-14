/******** a_time.c ********/
/* Timothy Ellsworth Bowers
   10 June 2009

   Astronomical time-related fuctions needed for various
   astronomical calculations.  All algorithms are from
   Astronomical Algorithms by Jean Meeus, 2nd edition --
   unless otherwise specified.

*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <tpeb/a_time.h>

/* Function that returns the current date/time in an a_time_time structure */
a_time_time *a_time_get_date_now(int clock_type){
  
  /* Variable Declarations */
  a_time_time *date;
  char buf0[81],buf1[81],buf2[81],buf3[81],buf4[81],buf5[81],buf6[81];
  time_t now;
  struct tm *ptr;
  struct timeval tv;
  
  
  date = (a_time_time *)malloc(sizeof(a_time_time));
  
  /* Get time string */
  time(&now);
  if(clock_type == A_TIME_LOCAL)
    ptr = localtime(&now);  
  else if(clock_type == A_TIME_GMT)
    ptr = gmtime(&now);  
  gettimeofday(&tv, NULL); 
  
  /* Extract ascii information from the date */
  /* And convert it to numbers */
  
  strftime(buf0, 80, "%S", ptr);
  date->sec   = atof(buf0) + (float)tv.tv_usec/1000000.;
  strftime(buf1, 80, "%M", ptr);
  date->min   = atoi(buf1);
  strftime(buf2, 80, "%H", ptr);
  date->hour  = atoi(buf2);
  strftime(buf3, 80, "%d", ptr);
  date->day   = atoi(buf3);
  strftime(buf5, 80, "%m", ptr);
  date->month = atoi(buf5);
  strftime(buf4, 80, "%Y", ptr);
  date->year  = atoi(buf4);
  strftime(buf6, 80, "%j", ptr);
  date->doy   = atoi(buf6);
  
  return date;
}

/* Function that returns the current date/time in an a_time_time structure */
a_time_time *a_time_get_date(int clock_type, int prevday){
  
  /* Variable Declarations */
  a_time_time *date;
  char buf0[81],buf1[81],buf2[81],buf3[81],buf4[81],buf5[81],buf6[81];
  time_t now;
  struct tm *ptr;
  struct timeval tv;
  
  
  date = (a_time_time *)malloc(sizeof(a_time_time));
  
  /* Get # of seconds since Jan. 1, 1970 */
  time(&now);
  
  /* Remove the appropriate number of days from now */
  now -= (86400 * prevday);
  
  /* Get time string */
  if(clock_type == A_TIME_LOCAL)
    ptr = localtime(&now);  
  else if(clock_type == A_TIME_GMT)
    ptr = gmtime(&now);  
  gettimeofday(&tv, NULL); 
  
  /* Extract ascii information from the date */
  /* And convert it to numbers */
  
  strftime(buf0, 80, "%S", ptr);
  date->sec   = atof(buf0) + (float)tv.tv_usec/1000000.;
  strftime(buf1, 80, "%M", ptr);
  date->min   = atoi(buf1);
  strftime(buf2, 80, "%H", ptr);
  date->hour  = atoi(buf2);
  strftime(buf3, 80, "%d", ptr);
  date->day   = atoi(buf3);
  strftime(buf5, 80, "%m", ptr);
  date->month = atoi(buf5);
  strftime(buf4, 80, "%Y", ptr);
  date->year  = atoi(buf4);
  strftime(buf6, 80, "%j", ptr);
  date->doy   = atoi(buf6);
  
  return date;
}

/* Function that calculates JD based on year, month, day */
double a_time_jd(int year, int month, double day){
  
  int A,B;
  
  if(month<3){
    year--;
    month+=12;
  }
  
  A = floor(year/100.);
  if(year>1582)
    B = 2 - A + floor(A/4.);
  else
    B = 0;
  
  return (floor(365.25*(year+4716)) + floor(30.6001*(month+1)) + day + 
	  B - 1524.5);
  
}


/* Function that returns today's JD (at 0h GMT) */
double a_time_jd_today(){
  
  a_time_time *now;
  double jd;

  /* Get time structure */
  now = a_time_get_date_now(A_TIME_GMT);
  
  // Use a_time_jd() to calculate the day's JD
  jd = a_time_jd(now->year, now->month, now->day);
  
  // Fix memory leak by freeing a_time_time structure.
  free(now);
  
  return jd;
}


/* Function that returns the JD of the moment (er, nearest second */
double a_time_jd_now(){
  
  a_time_time *now;
  double daypart,jd_now;
  
  /* Get time structure */
  now = a_time_get_date_now(A_TIME_GMT);
  
  daypart = (now->hour / 24.) + (now->min / 1440.) + (now->sec / 86400.);
  
  jd_now = a_time_jd_today() + daypart;
  
  // Fix memory leak by freeing a_time_time structure.
  free(now);
  
  return jd_now;
}


/* Function to calculate the LST, based on current location and time
   The LST is in ddd.dddddd (i.e. degrees past midnight) */
double a_time_get_lst(double longitude){
  
  /* Variable Declarations */
  long double jd,jd_now,T,theta_0,lst;  

  /* Get today's JD via a_time_jd() */
  jd     = a_time_jd_today();
  jd_now = a_time_jd_now();
  
  /* Calculate T */
  T = (jd - 2451545.0) / 36525;
    
  /* Calculate theta_0 with the aid of a_time_jdnow() */
  theta_0 = 280.46061837 + (360.98564736629 * (jd_now - 2451545.0)) +
    (0.000387933 * T * T) - (T * T * T / 38710000.);
  
  /* Calculate LST via theta = theta_0 - longitude */
  lst = theta_0 - longitude;

  // IF lst is outside the range [0,360], adjust until it is
  if(lst > 360.){
    lst -= floor(lst / 360.) * 360.;
  }else if(lst < 0.)
    while(lst < 0.)
      lst += 360.;
  
  return (double)lst;
}


/* Function to calculate the LST, at a longitude for a given JD.
   The LST is in ddd.dddddd (i.e. degrees past midnight) */
double a_time_lst(double longitude, double jd){
  
  /* Variable Declarations */
  long double jd_0h,T,theta_0,lst;  

  /* Set jd_0h for the calculation of T --> JD @0h GMT */
  jd_0h = floor(jd) + 0.5;
  if(jd - jd_0h < 0.)
    jd_0h += 1.;
  
  /* Calculate T */
  T = (jd_0h - 2451545.0) / 36525;
    
  /* Calculate theta_0 with the aid of a_time_jdnow() */
  theta_0 = 280.46061837 + (360.98564736629 * (jd - 2451545.0)) +
    (0.000387933 * T * T) - (T * T * T / 38710000.);
  
  /* Calculate LST via theta = theta_0 - longitude */
  lst = theta_0 - longitude;
  
  // IF lst is outside the range [0,360], adjust until it is
  if(lst > 360.){
    lst -= floor(lst / 360.) * 360.;
  }else if(lst < 0.)
    while(lst < 0.)
      lst += 360.;
  
  return (double)lst;
}


char *a_time_datestring(){
  
  /* Variable Declarations */
  char *string;
  a_time_time *now;
  
  /* Get date structure */
  now = a_time_get_date_now(A_TIME_LOCAL);
  
  string = (char *)malloc(81 * sizeof(char));
  
  sprintf(string,"%0d-%02d-%02d %02d:%02d:%02.f",now->year,now->month,
	  now->day,now->hour,now->min,now->sec);
  
  // Fix memory leak by freeing a_time_time structure.
  free(now);
  
  return string;
}
