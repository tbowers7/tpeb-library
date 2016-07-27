/******** astrom.c ********/
/* Timothy Ellsworth Bowers
   3 July 2009

   Library routines for astrometry-related functions needed for various
   astronomical calculations.  All algorithms are from
   Astronomical Algorithms by Jean Meeus, 2nd edition --
   unless otherwise specified.

   Calling sequences:
     ang_sep_deg = angular_separation(&star1, &star2); -- in degrees
     rel_pos_ang = rel_position_angle(&star1, &star2); -- deg EofN  1 wrt 2

     

*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include <tpeb.h>

/* Function for calculating angular separation
   Algorithm from "Astronomical Algorithms, 2nd ed", Ch 17, by Jean Meeus */
double astrom_ang_sep(const astrom_coords *star1, 
			  const astrom_coords *star2){
  
  /* Variable Declarations */
  double sep;
  double A,B,C;
  astrom_coords starone,startwo,delta;

  /* Equation 17.5 from Meeus used here:
     hav d = hav del_dec + cos dec1 * cos dec2 * hav del_ra
     Where...
          hav x = (1 - cos x)/2 = sin^2 (x/2)
  */

  /* Conversion of coordinates to Radians for use with C's trig functions */
  starone.ra  = star1->ra  * DEG2RAD;
  starone.dec = star1->dec * DEG2RAD;
  startwo.ra  = star2->ra  * DEG2RAD;
  startwo.dec = star2->dec * DEG2RAD;

  /* Assignment of parts of equation 17.5 to variables A-C for ease */
  delta.ra  = starone.ra  - startwo.ra;
  delta.dec = starone.dec - startwo.dec;

  A = sin(delta.dec/2.) * sin(delta.dec/2.);
  B = cos(starone.dec) * cos(startwo.dec);
  C = sin(delta.ra/2.) * sin(delta.ra/2.);

  sep = 2 * asin(sqrt(A + B * C)) * RAD2DEG;
  
  return sep;
}


/* Alternate function for calculating angular separation
   Algorithm from "Astronomical Algorithms, 2nd ed", Ch 17, by Jean Meeus */
double astrom_ang_sep2(const astrom_coords *star1, 
			   const astrom_coords *star2){
  
  /* Variable Declarations */
  double sep;
  double x,y,z;
  astrom_coords starone,startwo,delta;

  /* Unnumbered equation, (p. 115) from Meeus used here:
     d = atan( sqrt(x^2 + y^2) / z)
     Where...
        x = cos dec1 * sin dec2 - sin dec1 * cos dec2 * cos (-del_ra)
	y = cos dec2 * sin (-del_ra)
	z = sin dec1 * sin dec2 + cos dec1 * cos dec2 * cos (-del_ra)
  */

  /* Conversion of coordinates to Radians for use with C's trig functions */
  starone.ra  = star1->ra  * DEG2RAD;
  starone.dec = star1->dec * DEG2RAD;
  startwo.ra  = star2->ra  * DEG2RAD;
  startwo.dec = star2->dec * DEG2RAD;

  /* Assignment of parts of equation 17.5 to variables x-z for ease */
  delta.ra  = starone.ra  - startwo.ra;
  delta.dec = starone.dec - startwo.dec;
    
  x = cos(starone.dec) * sin(startwo.dec) - sin(starone.dec) * 
    cos(startwo.dec) *  cos(-delta.ra);
  y = cos(startwo.dec) * sin(-delta.ra);
  z = sin(starone.dec) * sin(startwo.dec) + cos(starone.dec) * 
    cos(startwo.dec) *  cos(-delta.ra);
  
  sep = atan2( hypot(x,y), z) * RAD2DEG;
  
  return sep;
}


/* Function for calculating relative position angle (obj1 wrt obj 2)
   Algorithm from "Astronomical Algorithms, 2nd ed", Ch 17, by Jean Meeus */
double astrom_rel_pos_ang(const astrom_coords *star1, 
			  const astrom_coords *star2){
  
  /* Variable Declarations */
  double theta;
  double A,B,C;
  astrom_coords starone,startwo,delta;

  /* Equation (unnumbered), p.116 from Meeus used here:
     tan P = (sin del_ra) / (cos dec * tan dec1 - sin dec2 * cos del_ra
  */

  /* Conversion of coordinates to Radians for use with C's trig functions */
  starone.ra  = star1->ra  * DEG2RAD;
  starone.dec = star1->dec * DEG2RAD;
  startwo.ra  = star2->ra  * DEG2RAD;
  startwo.dec = star2->dec * DEG2RAD;

  /* Assignment of parts of this equation to variables A-C for ease */
  delta.ra = starone.ra  - startwo.ra;

  A = sin(delta.ra);
  B = cos(startwo.dec) * tan(starone.dec);
  C = sin(startwo.dec) * cos(delta.ra);
  
  theta = atan2(A , (B - C)) * RAD2DEG;

  if(theta < 0.)
    theta += 360.;      // 0 < theta < 360
  
  return theta;
}


/* Function for calculating the precession of coordinates 
   Algorithm from "Astronomical Algorithms, 2nd ed", Ch 21, by Jean Meeus */
void astrom_precess2(astrom_coords *position, double epoch_i, double epoch_f){

  double T,t,zeta,zed,theta,A,B,C,alpha,delta,alphaout;


  /* Input epochs in year format YYYY.YY */
  
  T = (epoch_i - 2000.0) / 100.;
  t = (epoch_f - epoch_i)/ 100.;

  /* Calculate angles in degrees*DEG2RAD (ending in radians) */
  
  zeta  = ((2306.2181+1.39656*T-0.000139*T*T)*t + 
	   (0.30188-0.000344*T)*t*t + 0.017998*t*t*t)/3600.*DEG2RAD;
  zed   = ((2306.2181+1.39656*T-0.000139*T*T)*t + 
	   (1.09468+0.000066*T)*t*t + 0.018203*t*t*t)/3600.*DEG2RAD;
  theta = ((2004.3109-0.85330*T-0.000217*T*T)*t - 
	   (0.42665+0.000217*T)*t*t - 0.041833*t*t*t)/3600.*DEG2RAD;
  
  alpha = position->ra  * DEG2RAD;
  delta = position->dec * DEG2RAD;

  A = cos(delta) * sin(alpha + zeta);
  B = cos(theta) * cos(delta) * cos(alpha + zeta) - sin(theta) * sin(delta);
  C = sin(theta) * cos(delta) * cos(alpha + zeta) + cos(theta) * sin(delta);


  /* Check output of R.A. to keep within bounds */
  
  alphaout = (atan2(A,B) + zed);

  if(alphaout < 0.)
    position->ra = (alphaout + TWOPI) * RAD2DEG;
  else if(alphaout > TWOPI)
    position->ra = (alphaout - TWOPI) * RAD2DEG;
  else
    position->ra = alphaout * RAD2DEG;

  /* If original coords near poles, alternate Dec calculation */
  
  if(position->dec > 80. || position->dec < -80.)
    position->dec = acos(hypot(A,B)) * RAD2DEG;
  else
    position->dec = (asin(C)) * RAD2DEG;
  
  return;
}


/* Function to calculate the LHA of an object 
   NOTE: LHA is measured in degrees */
double astrom_get_lha(double lst, double alpha){
  
  /* Calculate LST, then get LHA */
  return lst - alpha;
}


/* Function to calculate the altitude & azimuth of a sky position
      Equations 13.5 & 13.6, p. 93 (Meeus)  */
void astrom_get_altaz(astrom_coords *object, astrom_location *observer, 
		      astrom_altaz *position, double lst){
  
  /* Variable Declarations */
  double H,phi,delta;

  /* Get variables , converting to radians for use with C trig functions */
  H     = astrom_get_lha(lst, object->ra) * DEG2RAD;
  phi   = observer->lat * DEG2RAD;
  delta = object->dec * DEG2RAD;
  
  /* Calculate alt & az, converting back to degrees */
  position->az  = 180. + atan2( sin(H) , (cos(H)*sin(phi) - 
					  tan(delta)*cos(phi))) * RAD2DEG;
  position->alt = asin( sin(phi)*sin(delta) + 
			cos(phi)*cos(delta)*cos(H)) * RAD2DEG;
  
  return;
}


/* Function to calculate the transit time of an object on the JD */
astrom_rst *astrom_get_rst(astrom_coords *object, astrom_location *observer,
			   double JD, double alt, int *updown){
  
  /* Variable Declarations */
  astrom_rst *times;
  double cos_H0;
  double alpha = object->ra;
  double L     = observer->lon;
  double h     = DEG2RAD * alt;
  double phi   = DEG2RAD * observer->lat;
  double delta = DEG2RAD * object->dec;
  
  times = (astrom_rst *)malloc(sizeof(astrom_rst));
  
  /* Calculate Ho */
  cos_H0 = ( sin(h) - sin(phi)*sin(delta) ) / ( cos(phi)*cos(delta) );
  
  // Check validity of cos_H0
  if(cos_H0 < -1 || cos_H0 > 1){
    astrom_altaz circum;
    astrom_get_altaz(object, observer, &circum, 0.0);

    /* Check for always up circumpolar */
    if(circum.alt > alt){
      *updown = 1;
      //fprintf(stderr,"Object is always above altitude %.1f degrees\n",alt);
      times->m0 = (alpha - atime_lst(L,JD)) / 360.;
      // Check ranges on transit time for circumpolar (always up)
      if(times->m0 < 0.) times->m0 += 1.;
      if(times->m0 > 1.) times->m0 -= 1.;
    }

    /* Check for always down circumpolar */ 
    else if(circum.alt < alt){
      *updown = -1;
      //fprintf(stderr,"Object is never above altitude %.1f degrees\n",alt);
      times->m0 = 0.;
    }
    times->m1 = 0.;
    times->m2 = 0.;
    return times;
  }
  
  /* For objects that rise and set as usual */
  else
    *updown = 0;
  
  // Calculate times
  times->m0 = (alpha - atime_lst(L,JD)) / 360.;
  times->m1 = times->m0 - acos(cos_H0)*RAD2DEG / 360.;
  times->m2 = times->m0 + acos(cos_H0)*RAD2DEG / 360.;
  
  /* m's should be in range [0,1], so check each one and correct */
  if(times->m0 < 0.) times->m0 += 1.;
  if(times->m0 > 1.) times->m0 -= 1.;
  if(times->m1 < 0.) times->m1 += 1.;
  if(times->m1 > 1.) times->m1 -= 1.;
  if(times->m2 < 0.) times->m2 += 1.;
  if(times->m2 > 1.) times->m2 -= 1.;
  
  return times;
}

/* Function for reading in observatory location catalog files */
astrom_location *astrom_read_observatories(char *filename, int *n){
  
  /* Variable Declarations */
  int i;
  char line[211],lat_str[50],lon_str[50],tz_str[15],num_str[15];
  FILE *fp;
  astrom_location *locations,catline;
  
  /* Open catalog file & count # of entries */
  fp = fileopenr(filename);
  *n = countlines(fp);
  
  /* Allocate space for the structure array */
  locations = (astrom_location *)malloc(*n * sizeof(astrom_location)); 
  
  /* FOR loop reading in the lines of the catalog file */
  for(i=0; i<*n; i++){

    // Get the line first, check for '#'. then read in 8 fields
    fgets(line,210,fp);
    if(line[0] == '#')      // Ignore commented lines
      i--;
    else{                   // If not commented, read in line
      // Catalog is character-wise laid out

      // 5 characters for the location number
      strncpy(num_str,line,5);
      catline.num = atoi(num_str);

      // 40 characters for the location name
      strncpy(catline.name,line+5,40);
      catline.name[40] = '\0';          // Append null character

      // 15 characters for the location latitude
      strncpy(lat_str,line+45,15);
      catline.lat = atof(lat_str);

      // 15 characters for the location longitude
      strncpy(lon_str,line+60,15);
      catline.lon = atof(lon_str);

      // 5 characters for the location time zone
      strncpy(tz_str,line+75,5);
      catline.tz = atoi(tz_str);

      locations[i] = catline;
    }
  }
  
  fclose(fp);
  
  return locations;
}


/* Function to prompt user for location based on input observatories.dat file,
   returns the astrom_location structure. */
astrom_location astrom_get_location(char *filename){
  
  /* Variable Declarations */
  int locno,good_obj,j,num_loc;
  astrom_location *list;
  
  /* Read in telescope DAT file */
  list = astrom_read_observatories(filename, &num_loc);
  
  printf("\nTelescope locations:\n--------------------\n");
  for(j=0;j<num_loc;j++)
    printf("%d    %s\n",j+1,list[j].name);
  
  // Select site number, w/ error checking
  do{
    printf("\nSelect site: ");
    fflush(stdin);
    fscanf(stdin,"%d",&locno);
    
    // Error checking
    if(locno < 1 || locno > num_loc){
      printf("Improper location number.  Try again.\n");
      good_obj = 0;
    }else good_obj = 1;
  }while(!good_obj);
  
  return list[locno-1];
  
}
