/* mapper.c/v1.0 - pixelmapper for complexfractals
 * by Pegasus Epsilon <pegasus@pimpninjas.org>
 * Distribute Unmodified -- http://pegasus.pimpninjas.org/license
 *
 *  Changelog:
 *  v1.0 -- pulled into seperate module
 */

#include <math.h>
#include "types.h"

COMPLEX pixelsize (const PIXEL *const img, const REGION *const window) {
	COMPLEX out;
	out.R = 2 * window->radius.R / (img->X - 1);
	out.I = 2 * window->radius.I / (img->Y - 1);
	return out;
}

COMPLEX pixel2vector (
	const PIXEL *const in, const COMPLEX *const size,
	const REGION *const window, const double theta
) {
	COMPLEX out;

	out.R = window->center.R
		+ (in->X * size->R - window->radius.R) * cos(theta)
		- (in->Y * size->I - window->radius.I) * sin(theta);
	out.I = -window->center.I
		+ (in->X * size->R - window->radius.R) * sin(theta)
		+ (in->Y * size->I - window->radius.I) * cos(theta);

	return out;
}
