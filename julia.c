/* julia.c/v3.1 - sampling half of the julia renderer for complexfractals.
** by Pegasus Epsilon <pegasus@pimpninjas.org>
** Distribute Unmodified -- http://pegasus.pimpninjas.org/license
**
**  Changelog:
**  3.1 -- forked from mandelbrot sampler
*/

#include <stdio.h>  	/* printf(), FILE, fopen(), fwrite(), fclose() */
#include <stdlib.h> 	/* exit(), atoi(), atof() */
#include <math.h>   	/* log() */
#include "types.h"
#include "iterator.h"

#define PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421
#define FALSE 0
#define TRUE !FALSE

void usage (const char *myself) {
	printf("%s width height centerX centerY radiusX radiusY seedReal seedImaginary angle outfile\n", myself);
	exit(1);
}

complex pixelsize (pixel img, region window) {
	complex out;
	out.R = 2 * window.radius.R / (img.X - 1);
	out.I = 2 * window.radius.I / (img.Y - 1);
	return out;
}

complex pixel2vector (pixel in, complex size, region window, double theta) {
	complex out;

	out.R = window.center.R
		+ (in.X * size.R - window.radius.R) * cos(theta)
		- (in.Y * size.I - window.radius.I) * sin(theta);
	out.I = window.center.I
		+ (in.X * size.R - window.radius.R) * sin(theta)
		+ (in.Y * size.I - window.radius.I) * cos(theta);

	return out;
}

int main (int argc, char **argv) {
	double angle, sample;
	pixel img, i;
	region window;
	complex size, z, c;
	FILE *outfile;

	if (11 != argc) { usage(argv[0]); }

	img.X = atoi(argv[1]);
	img.Y = atoi(argv[2]);

	window.center.R = atof(argv[3]);
	window.center.I = atof(argv[4]);
	window.radius.R = atof(argv[5]);
	window.radius.I = atof(argv[6]);
	c.R = atof(argv[7]);
	c.I = atof(argv[8]);

	angle = atof(argv[9]) * PI / 180;

	if (!(outfile = fopen(argv[10], "w"))) {
		perror(argv[10]);
		exit(3);
	}

	size = pixelsize(img, window);

	printf("Rendering %dx%d to %s...\n", img.X, img.Y, argv[10]);
  printf("Image center: %f%+fi\n", window.center.R, window.center.I);
  printf("Image radius: %f%+fi\n", window.radius.R, window.radius.I);
  printf("Mandelbrot center: %f%+fi\n", c.R, c.I);
	for (i.Y = 0; i.Y < img.Y; i.Y++) {
		printf("\rline %d/%d, %0.2f%%...", i.Y, img.Y, (float)i.Y*100/(float)img.Y);
		fflush(stdout);
		for (i.X = 0; i.X < img.X; i.X++) {
			z = pixel2vector(i, size, window, angle);
			sample = iterate(&z, &c);
			fwrite(&sample, sizeof(double), (size_t)1, outfile);
		}
	}
	printf("\rline %d/%d, %0.2f%%...\n", i.Y, img.Y, (float)i.Y*100/(float)img.Y);
	fclose(outfile);
	printf("Complete. Maximum iteration: %llu\n", maxiter);

	return 0;
}
