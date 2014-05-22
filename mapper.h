/* mapper.h/v1.0 - pixelmapper for complexfractals
 * by Pegasus Epsilon <pegasus@pimpninjas.org>
 * Distribute Unmodified -- http://pegasus.pimpninjas.org/license
 *
 *  Changelog:
 *  v1.0 -- pulled into seperate module
 */

#include "types.h"

COMPLEX pixelsize (const PIXEL *const, const REGION *const);

extern COMPLEX (*pixel2vector) (
	const PIXEL *const, const COMPLEX *const,
	const REGION *const, const double
);
