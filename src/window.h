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
