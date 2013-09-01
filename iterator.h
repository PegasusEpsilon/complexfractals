/* iterator.h/v0 by Pegasus Epsilon <pegasus@pimpninjas.org>
 *
 * Too short to reasonably copyright, so whatevs
 */
#ifndef ITERATOR_H
#define ITERATOR_H
#include "types.h"

long long unsigned maxiter;
extern double (*iterate) (const COMPLEX*, const COMPLEX*, void*);
TRAP trapcheck(int*, char***);
int inset (const COMPLEX* const);
#endif /* ITERATOR_H */
