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
