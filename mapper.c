/* mapper.c/v1.0 - pixelmapper for complexfractals
** by Pegasus Epsilon <pegasus@pimpninjas.org>
** Distribute Unmodified -- http://pegasus.pimpninjas.org/license
**
**  Changelog:
**  v1.0 -- pulled into seperate module
*/

#include <math.h>
#include "types.h"

complex pixelsize (const pixel *const img, const region *const window) {
	complex out;
	out.R = 2 * window->radius.R / (img->X - 1);
	out.I = 2 * window->radius.I / (img->Y - 1);
	return out;
}

complex pixel2vector (
	const pixel *const in, const complex *const size,
	const region *const window, const double theta
) {
	complex out;

	out.R = window->center.R
		+ (in->X * size->R - window->radius.R) * cos(theta)
		- (in->Y * size->I - window->radius.I) * sin(theta);
	out.I = window->center.I
		+ (in->X * size->R - window->radius.R) * sin(theta)
		+ (in->Y * size->I - window->radius.I) * cos(theta);

	return out;
}
