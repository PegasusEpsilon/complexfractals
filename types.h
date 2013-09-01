/* types.h/v0 by Pegasus Epsilon <pegasus@pimpninjas.org>
 *
 * Too short to reasonably copyright, so whatevs.
 */
#ifndef TYPES_H
#define TYPES_H

#include <inttypes.h>

typedef struct {
	double R;
	double I;
} COMPLEX;

typedef struct {
	COMPLEX center;
	COMPLEX radius;
} REGION;

typedef struct {
	int X;
	int Y;
} PIXEL;

typedef struct {
	double angle, dist, sin, cos, hyp;
	uint32_t start;
} TRAP;

#endif
