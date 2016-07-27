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
