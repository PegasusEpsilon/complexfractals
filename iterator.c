/* iterator.c/v0.3 - generic iterator for complexfractals
 * by Pegasus Epsilon <pegasus@pimpninjas.org>
 * Distribute Unmodified -- http://pegasus.pimpninjas.org/license
 *
 *  Changelog:
 *  0.1 -- First version that works for both mandelbrot and julia.
 *  0.2 -- Rewrote renormalization formula, reduced execution time 56%.
 *  0.3 -- Removed apparently vestigial "last" variable.
 */
#include <math.h>   	/* log(), log2(), sin(), cos(), sqrt() */
#include <stdlib.h> 	/* atof() */
#include <string.h> 	/* strcmp() */
#include "types.h"  	/* struct COMPLEX */
#include "constants.h"

#define ESCAPE 16*16	/* */

unsigned long long maxiter = 0;
double renormalized (COMPLEX *, COMPLEX *, void *);
double crosstrap (COMPLEX *, COMPLEX *, void *);
double (*iterate)(COMPLEX *const z, COMPLEX *const c, void *data) = &renormalized;

int inset (const COMPLEX *const c) {
	/* quick check to see if the point is easily found in the set */
	double tmp;

	/* skip chaos line */
	if (0 == c->I && -1 > c->R && -2 < c->R) return TRUE;

	/* skip first and second period regions */
	tmp = (c->R - .25) * (c->R - .25) + c->I * c->I;

	if (
		4 * tmp * (tmp + (c->R - .25)) / c->I / c->I < 1 ||
		16 * ((c->R + 1) * (c->R + 1) + c->I * c->I) < 1
	) return TRUE;

	return FALSE;
}

/* check if the user requested a cross trap */
/* this feels so wrong... */
TRAP trapcheck (int *argc, char ***argv) {
	TRAP trap;
	if (3 < *argc) {
		if (!strcmp("--crosstrap", (*argv)[1])) {
			trap.angle = atof((*argv)[2]) * M_PI / 90;
			trap.dist = atof((*argv)[3]);
			trap.start = (uint32_t)atoi((*argv)[4]);
			trap.sin = sin(trap.angle);
			trap.cos = cos(trap.angle);
			trap.hyp = sqrt(trap.sin * trap.sin + trap.cos * trap.cos);
			(*argv)[4] = (*argv)[0]; *argc -= 4; *argv += 4;
			iterate = &crosstrap;
		} else trap.dist = 0;
	}
	return trap;
}

/* iterator for the cross trap */
double crosstrap (COMPLEX *const z, COMPLEX *const c, void *data) {
	unsigned long long i, deadline;
	double d = (double)((unsigned long long)-1);
	COMPLEX z2, oz;
	TRAP *trap;

	trap = (TRAP *)data;

	for (
		i = 0, (double)(deadline = 1),
		oz.R = oz.I = 100; /* far outside the escape radius */
		i != (unsigned long long)-1; i++
	) {
		z2.R = z->R * z->R;
		z2.I = z->I * z->I;

		/* if point escaped, break out */
		if (z2.R + z2.I > ESCAPE) break;

		z->I = 2 * z->R * z->I + c->I;
		z->R = z2.R - z2.I + c->R;

		if (i > trap->start) {
			double v, h, n;
			v = fabs(trap->sin * z->R + trap->cos * z->I);
			h = fabs(trap->cos * z->R - trap->sin * z->I);
			n = (h < v ? h : v) / trap->hyp;
			if (n < d) d = n;
		}
		if (oz.R == z->R && oz.I == z->I) return d / 2;
		if (i > deadline) { oz.R = z->R; oz.I = z->I; deadline <<= 1; }
	}

	/* track observed maximum iteration (for curiosity) */
	maxiter = maxiter < i ? i : maxiter;

	return trap->dist < d ? -1 : trap->dist - d;
}

/* iterator for renormalized iteration count */
double renormalized (COMPLEX *const z, COMPLEX *const c, void *data) {
	/* returns -1 when in set, renormalized escape time otherwise */
	unsigned long long i, deadline;
	COMPLEX z2, oz;

	for (
		i = 0, (double)(deadline = 1),
		oz.R = oz.I = 100; /* far outside the escape radius */
		i != (unsigned long long)-1; i++
	) {
		z2.R = z->R * z->R;
		z2.I = z->I * z->I;

		/* if point escaped, break out */
		if (z2.R + z2.I > ESCAPE) break;

		z->I = 2 * z->R * z->I + c->I;
		z->R = z2.R - z2.I + c->R;

		/* periodic orbits are in the set, detect them and bail out. */
		if (oz.R == z->R && oz.I == z->I) return -1;
		if (i > deadline) { oz.R = z->R; oz.I = z->I; deadline <<= 1; }
	}

	/* track observed maximum iteration (for curiosity) */
	maxiter = maxiter < i ? i : maxiter;

	/* return normalized iteration count */
	return (double)i - log2(log(z2.R + z2.I) / 2);
}
