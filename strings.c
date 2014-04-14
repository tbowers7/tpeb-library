/******** strings.h ********/
/* Timothy Ellsworth Bowers
   9 October 2009

   Library routines related to string shortcuts.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tpeb/strings.h>


/* Function in the style of getline */
int strings_getline(FILE *f, char *buf, size_t *length){
  
  /* Variable Declarations */
  char *nl,*final,*inter;
  int i;
  
  /* Read in line up to the STRINGS_LEN limit */
  if (fgets(buf, STRINGS_LEN, f) != NULL){
    nl = strchr(buf, '\n');
    
    i = 1;
    final = (char *)malloc(i * STRINGS_LEN * sizeof(char));
    
    strcpy(final,buf);
    
    /* If newline not found, keep reading in from stream until newline found */
    if (nl == NULL){
      
      /* Define variables needed here */
      char dummy[STRINGS_LEN];
      char *r;
      
      do {
	
	/* Allocate space for 'final' that is large enough for i runs */
	free(inter);
	inter = (char *)malloc(i * STRINGS_LEN * sizeof(char));
	strcpy(inter,final);
	
	i++;
	free(final);
	final = (char *)malloc(i * STRINGS_LEN * sizeof(char));
	
	r = fgets(dummy, STRINGS_LEN, f);
	
	strcpy(final,inter);
	strcat(final,dummy);
	
      } while (r != NULL && strchr(dummy, '\n') == NULL);
      
      nl = strchr(final, '\n');
      *nl = 0;
      *length = strlen(final);
      
      /* If newline found, then remove '\n' and return */
    } else {
      *nl = 0;
      *length = strlen(buf);
    }
    
    
  } else {
    /* Error checking if fgets didn't get anything */
    fprintf(stderr,"Error -- no input detected.  Returning empty string.\n");
    strcpy(buf,"");
    return 0;
  }
  
  return 1;
  
}
