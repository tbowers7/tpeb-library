/******** read_dat_files.h ********/
/* Timothy Ellsworth Bowers
   28 February 2009

   Header file for the read_ncolumn routines, plus countlines().
*/

#if !defined(READ_DAT_FILES_H)

#define READ_DAT_FILES_H 1

int countlines(FILE *fp);
double *read_ncolumn(char *filename, int *N, int m);
double *parse_array(double *total_array, int n_lines, int n_dim);

#endif
