/******** fileio.c ********/
/* Timothy Ellsworth Bowers
   18 September 2008

   Actual functions for the various permutations of file opening, and contains
   the proper error-handling, etc.  This is purely shorthand because I am lazy.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *fileopenr(char *filename){

  FILE *fp;

  if((fp=fopen(filename,"r")) == NULL){
    fprintf(stderr,"\nError opening file %s\n",filename);
    exit(1);
  }

  return fp;
}

FILE *fileopenw(char *filename){

  FILE *fp;

  if((fp=fopen(filename,"w")) == NULL){
    fprintf(stderr,"\nError opening file %s\n",filename);
    exit(1);
  }

  return fp;
}

FILE *fileopenrb(char *filename){

  FILE *fp;

  if((fp=fopen(filename,"rb")) == NULL){
    fprintf(stderr,"\nError opening file %s\n",filename);
    exit(1);
  }

  return fp;
}

FILE *fileopenwb(char *filename){

  FILE *fp;

  if((fp=fopen(filename,"wb")) == NULL){
    fprintf(stderr,"\nError opening file %s\n",filename);
    exit(1);
  }

  return fp;
}

FILE *fileopenwa(char *filename){

  FILE *fp;

  if((fp=fopen(filename,"a")) == NULL){
    fprintf(stderr,"\nError opening file %s\n",filename);
    exit(1);
  }

  return fp;
}

/* Create filename from existing root */
void make_filename(char *infile, char *outfile, char *ext, char *suffix){

  int cutpt;
  char *ptr,*root;

  ptr = strstr(infile,ext);
  cutpt = ptr - infile;

  root = (char *)calloc(cutpt + 1, sizeof(char));

  strncpy(root,infile,cutpt);
  sprintf(outfile,"%s%s",root,suffix);

  free(root);
  return;
}
