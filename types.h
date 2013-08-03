/* types.h/v0 by Pegasus Epsilon <pegasus@pimpninjas.org>
**
** Too short to reasonably copyright, so whatevs.
*/
#ifndef TYPES_H
#define TYPES_H

typedef struct {
	double R;
	double I;
} complex;

typedef struct {
	complex center;
	complex radius;
} region;

typedef struct {
	int X;
	int Y;
} pixel;

#endif
