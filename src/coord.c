/******** coord_utils.c ********/
/* Timothy Ellsworth Bowers
   10 June 2009

   Series of library utility routines to deal with coordinates in various
   manners -- usually to weave back and forth between ASCII and double
   representations of the coordinates.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <tpeb.h>


/* Function to parse out RA & Dec from string input
   Form of algorithm from Phil Pinto, UA, 1999) 
   --Modified 10/22/09 to return astrom_coord structure
   containing the ddd.ddddddd versions of the parsed
   coordinates  */
astrom_coords coord_parserd(char line[128]){
  
  char *ratok, *dectok, line2[128];
  double rdout[2][3];
  astrom_coords object;

  strcpy(line2,line);
  
  /* get RA and DEC */
  ratok = strtok(line2," ");
  dectok = strtok( (char *) NULL, " ");
  
  /* parse RA */
  rdout[0][0] = atoi(strtok(ratok,":"));
  rdout[0][1] = atoi(strtok((char *) NULL,":"));
  rdout[0][2] = atof(strtok((char *) NULL," "));
  
  /* parse DEC */
  rdout[1][0] = atoi(strtok(dectok,":"));
  rdout[1][1] = atoi(strtok((char *) NULL,":"));
  rdout[1][2] = atof(strtok((char *) NULL," "));
  
  /* Check to ensure proper values for coordinates */
  if(rdout[0][0] > 23 || rdout[0][0] < 0)
    printf("\n** Input Error:(RA hours)  Please reenter input **\n");
  else if(rdout[0][1] > 59 || rdout[0][1] < 0)
    printf("\n** Input Error:(RA minutes)  Please reenter input **\n");
  else if(rdout[0][2] >= 60. || rdout[0][2] < 0.)
    printf("\n** Input Error:(RA seconds)  Please reenter input **\n");
  else if(rdout[1][0] > 90 || rdout[1][0] < -90)
    printf("\n** Input Error:(Dec degrees)  Please reenter input **\n");
  else if(rdout[1][1] > 59 || rdout[1][1] < 0)
    printf("\n** Input Error:(Dec minutes)  Please reenter input **\n");
  else if(rdout[1][2] >= 60. || rdout[1][2] < 0.)
    printf("\n** Input Error:(Dec seconds)  Please reenter input **\n");
  
  /* Now that we're parsed and checked, convert to ddd.ddddddd format */
  object.ra  = coord_dmstodeg(rdout[0], COORD_RA);
  object.dec = coord_dmstodeg(rdout[1], COORD_DEC);
  
  return object;
}


/* Function to convert dms coordinates to decimal degrees 
   NOTE: Outgoing RA coordinates are in ddd.dddddd format!  */
double coord_dmstodeg(double dms[3], int c_type){

  /* Input 3-element array, and convert it to a single double */
  double degout;
  int sign;
  
  /* Taking precautions if value is less than zero */
  if(dms[0] < 0.){
    sign = -1;
    dms[0] = -dms[0];
  }
  else sign = 1;

  if(c_type == COORD_DEC)
    degout = sign*(dms[0]+(dms[1]/60.)+(dms[2]/3600.));
  else
    degout = sign*(dms[0]+(dms[1]/60.)+(dms[2]/3600.)) * 15.;
    
  return degout;
}


/* Function to convert decimal degrees to dms coordinates
   in the form of a colon-delimited string
   NOTE: RA coordinates must be in ddd.dddddd format before coming to
   this routine! */
void coord_degtodms(double deg, char *dms_str, int c_type){

  /* Variable Declarations */
  int neg = 1;
  float  dms[3];
  double temp[3];

  if(c_type == COORD_RA || c_type == COORD_LST)
    deg /= 15.;
  
  /* Dealing with negative signs */
  if(deg < 0){
    deg = -deg;
    neg = -1;
  }
  
  temp[0] = deg;
  dms[0] = neg * floor(temp[0]);
  temp[1] = (temp[0] - fabs(dms[0]))*60.;
  dms[1] = floor(temp[1]);
  temp[2] = (temp[1] - dms[1])*60.;
  dms[2] = (float)(temp[2]);
  
  if(c_type == COORD_DEC)
    sprintf(dms_str,"%+03.0f:%02.0f:%04.1f",dms[0],dms[1],dms[2]);
  else if(c_type == COORD_RA)
    sprintf(dms_str,"%02.0f:%02.0f:%05.2f",dms[0],dms[1],dms[2]);
  else if(c_type == COORD_LST)
    sprintf(dms_str,"%02.0f:%02.0f:%04.1f",dms[0],dms[1],dms[2]);
  
  
  return;
}
