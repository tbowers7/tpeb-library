/******** neville.c ********/
/* Timothy Ellsworth Bowers
   9 December 2008
   
   Neville's Algorithm for calculating polynomial of nth degree using n+1
   points.  Function based on algorithm found in Wikipedia article:
   "Neville's algorithm" (accessed 12/6/08). Algorithm calculates the
   polynomial, and returns the value of the function evaluated as 'ex'.  The
   desired order is given by 'n', and the input values *x, *y are in n-element
   arrays.
*/

double neville(double *x, double *y, double ex, int n){
  
  int i,j,k;
  double p[n+1][n+1];
  
  /* Inital pii = yi */
  for(i=0;i<=n;i++)
    p[i][i] = y[i];
    
  /* Loop through the algorithm*/
  for(k=1;k<=n;k++){
    for(i=0;i<=(n-k);i++){
      j = i+k;
      p[i][j] = ((ex-x[j])*p[i][j-1] + (x[i]-ex)*p[i+1][j])/(x[i] - x[j]);
    }
  }    
  
  return p[0][n];
}
