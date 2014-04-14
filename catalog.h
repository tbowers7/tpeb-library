/******** catalog.h ********/
/* Timothy Ellsworth Bowers
   11 June 2009

   Header file for catalog-related funtions needed for various
   astronomical calculations.

*/

#if !defined(CATALOG_H)

#define CATALOG_H 1

/* If 'Parallactic Angle'  */
#define CATALOG_PA -500.

/* Typedef catalog structures */

// Library Preferred catalog structure
typedef struct {
  char   id[50];
  double ra;
  double dec;
  float  ra_pm;
  float  dec_pm;
  float  mag;
  float  color;
  char   spectyp[50];
  double epoch;
  float  pa;
} catalog_lib;

// MMT-style catalog structure
typedef struct {
  char   id[50];
  double ra;
  double dec;
  float  ra_pm;
  float  dec_pm;
  float  mag;
  char   spectyp[50];
  double epoch;
} catalog_mmt;

// APO / TUI catalog structure
typedef struct {
  char   id[50];
  double ra;
  double dec;
  char   keywords[100];
} catalog_tui;

// Betsy Green pccb Catalog structure
typedef struct {
  char   id[50];
  double ra;
  double dec;
  double epoch;
  float  v_mag;
  float  b_v;
} catalog_bg;


/* Function declarations */
catalog_lib *catalog_read_lib(char *filename, int *n);
catalog_mmt *catalog_read_mmt(char *filename, int *n);
catalog_tui *catalog_read_tui(char *filename, int *n);

#endif
