/* mapper.h/v1.0 - pixelmapper for complexfractals
** by Pegasus Epsilon <pegasus@pimpninjas.org>
** Distribute Unmodified -- http://pegasus.pimpninjas.org/license
**
**  Changelog:
**  v1.0 -- pulled into seperate module
*/

complex pixelsize (const pixel *const img, const region *const window);

complex pixel2vector (
	const pixel *const in, const complex *const size,
	const region *const window, const double theta
);
