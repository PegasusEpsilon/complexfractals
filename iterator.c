/* v0.2/generic iterator for julia and mandelbrot fractals
** by Pegasus Epsilon <pegasus@pimpninjas.org>
** Distribute Unmodified -- http://pegasus.pimpninjas.org/license
**
**  Changelog:
**  v0.1 -- First version that works for both mandelbrot and julia.
**  v0.2 -- Rewrote renormalization formula, reduced execution time 56%.
*/
#include <math.h>   	/* log(), log2() */
#include "types.h"	/* struct complex */

#define ESCAPE 4*4	/* 4*4 = root twice for smooth shading */

unsigned long long maxiter = 0;
double iterate (complex *z, complex *c) {
	/* returns -1 when in set, normalized escape time otherwise */
	unsigned long long i, last, deadline;
	double magnitude;
	complex z2, oz;

	for (
		i = 0,
		oz.R = oz.I = 100, /* far outside the escape radius */
		last = (unsigned long long)-1, /* maximum possible iterations */
		(double)(deadline = 1);
		i != last; i++
	) {
/*
		if (i > 1 << 26 && 0 == i % 10000000) {
			printf(
				"\nvery high iteration count: %lld, "
				"magnitude: %le (real: %le, imaginary: %le)\x1b[K",
				i, magnitude, z->R, z->I
			);
			fflush(stdout);
		}
*/

		z2.R = z->R * z->R;
		z2.I = z->I * z->I;
		magnitude = z2.R - z2.I + c->R;
		z->I = 2 * z->R * z->I + c->I;
		z->R = magnitude;
		magnitude = z2.R + z2.I;

		/* if point escaped, break out */
		if ((unsigned long long)-1 == last && magnitude > ESCAPE) break;

		/* periodic orbits are in the set, detect them and bail out. */
		if (oz.R == z->R && oz.I == z->I) return -1;
		if (i > deadline) { oz.R = z->R; oz.I = z->I; deadline <<= 1; }
	}

	/* track observed maximum iteration (for curiosity) */
	maxiter = maxiter < i ? i : maxiter;

	/* return normalized iteration count */
	return i - log2(log(magnitude) / 2);
}
