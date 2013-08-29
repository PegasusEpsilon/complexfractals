/* mandelbrot.c/v3.2 - sampling half of the mandelbrot renderer for complexfractals.
** by Pegasus Epsilon <pegasus@pimpninjas.org>
** Distribute Unmodified -- http://pegasus.pimpninjas.org/license
**
**  Changelog:
**  3.0a1 -- first 3.0 version
**  3.0a2 -- removed unneccessary "magnitude" variable
**  3.0a3 -- removed unneccessary "tmp" variable in pixel2vector
**  3.1   -- refactored to share an iterator with a julia renderer.
**  3.2   -- moved pixel2vector and pixelsize to mapper.o
*/

#include <stdio.h>  	/* printf(), FILE, fopen(), fwrite(), fclose() */
#include <stdlib.h> 	/* exit(), atoi(), atof() */
#include <math.h>   	/* log() */
#include <string.h> 	/* strcmp() */
#include "types.h"
#include "iterator.h"
#include "mapper.h"
#include "constants.h"

void usage (const char *const myself) {
	printf("Usage: %s [--crosstrap angle distance start] width height centerX centerY radiusX radiusY angle outfile\n", myself);
	exit(1);
}

int main (int argc, char **argv) {
	double angle, sample;
	float pct = 0;
	int oldpct;
	TRAP trap;
	PIXEL img, i;
	REGION window;
	COMPLEX size, z, c;
	FILE *outfile;

	trap = trapcheck(&argc, &argv);

	if (9 != argc) { usage(argv[0]); }

	img.X = atoi(argv[1]);
	img.Y = atoi(argv[2]);

	window.center.R = atof(argv[3]);
	window.center.I = atof(argv[4]);
	window.radius.R = atof(argv[5]);
	window.radius.I = atof(argv[6]);

	angle = atof(argv[7]) * M_PI / 180;

	if (!(outfile = fopen(argv[8], "w"))) {
		perror(argv[8]);
		exit(3);
	}

	size = pixelsize(&img, &window);

	printf("Rendering %dx%d to %s...\n", img.X, img.Y, argv[8]);
	printf("Image center: %f%+fi\n", window.center.R, window.center.I);
	printf("Image radius: %f%+fi\n", window.radius.R, window.radius.I);
	for (i.Y = 0; i.Y < img.Y; i.Y++) {
		for (i.X = 0; i.X < img.X; i.X++) {
			z = c = pixel2vector(&i, &size, &window, angle);
			sample = iterate(&z, &c, (void *)&trap);
			fwrite(&sample, sizeof(double), (size_t)1, outfile);
		}

		oldpct = (int)(pct * 100);
		pct = (float)i.Y*100/(float)img.Y;
		if (oldpct != (int)(pct * 100)) {
			printf("\rline %d/%d, %0.2f%%...", i.Y, img.Y, pct);
			fflush(stdout);
		}
	}
	printf("\rline %d/%d, %0.2f%%...\n", i.Y, img.Y, (float)i.Y*100/(float)img.Y);
	fclose(outfile);
	printf("Complete. Maximum iteration: %llu\n", maxiter);

	return 0;
}
