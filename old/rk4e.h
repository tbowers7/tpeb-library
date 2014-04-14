/******** rk4e.h ********/







#ifndef	RK4S_H

#include "rk4.h"

/* error-controlled Runge-Kutta integrator */
int rk4s(derivf, double, double, int, double *, double *, void *);

#endif	/* RK4S_H */
