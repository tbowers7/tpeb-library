/******** window.c ********/
/* Timothy Ellsworth Bowers
   20 January 2010

   Apodization windowing functions for use with FFT implimentations.

   Included functions:
        window_andrew()  -- Andrew Hamilton's favorite window function
	window_hann()    -- Hann window function
	window_hamming() -- Hamming window function

*/

#include <math.h>

/* Andrew Hamilton's favorite (scalable) windowing function */
void window_andrew(double *array, int length, int n){
  
  int i;
  
  for(i=0; i<length; i++){
    /* Set window for 0 --> length 
       (1 - ((2x-L)/L)^2)^n           */
    array[i] *= pow(1.-pow((2*(double)i-(double)length)/(double)length,2.),n);
  }
  return;
}


void window_hann(double *array, int length){

  int i;
  double hann_len = (double)length / 2.;

  for(i=0; i<length; i++){
    /* t_0 = length / 2. for Hanning window -- build cosine period */
    array[i] *= 0.5 - 0.5 * cos(M_PI * (double)i / hann_len);
  }
  return;
}

void window_hamming(){}
