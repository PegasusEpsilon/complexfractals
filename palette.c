/* palette.c/v4.1 - palette generator for complexfractals
** by Pegasus Epsilon <pegasus@pimpninjas.org>
** Distribute Unmodified - http://pegasus.pimpninjas.org/license
**
**  Changelog:
**  v4.0 - Decoupled channels
**  v4.1 - Added verbose flag, suppressed noisy debug info
*/

#include <stdio.h>	/* perror(), puts(), printf(), fopen(), fgets(), feof(), sscanf(), fclose(), fwrite() */
#include <stdlib.h>	/* exit(), realloc(), calloc(), free() */
#include <stdarg.h>	/* va_list, va_start(), vprintf(), va_end() */
#ifdef _WIN32
#	include <stdint.h>	/* uint8_t */
#else
#	include <inttypes.h>	/* uint8_t	*/
#endif
#include <string.h>	/* strcmp() */
#include <math.h> 	/* cos(), fmod() */
#include "constants.h"

#define CHANNELS 3
#define RED 0
#define GRN 1
#define BLU 2

char *channel[] = {"RED", "GRN", "BLU"};

typedef struct {
	uint8_t y[CHANNELS];
} RGB24;

typedef struct {
	RGB24 *x;
	size_t length;
} GRADIENT;

typedef struct {
	double x;
	double y;
} POINT;

typedef struct {
	POINT *p;
	size_t length;
} CHANNEL;

void fail (const char *msg) {	/* report function failures */
	perror(msg);
	exit(1);
}

void die (const char *fmt, ...) {	/* report errors */
	va_list args;
	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
	puts(".");
	exit(1);
}

GRADIENT *generate_palette (const CHANNEL *points, GRADIENT *gradient) {
	uint8_t c;	/* only three channels, after all */
	size_t i, a, b;

	for (c = 0; c < CHANNELS; c++) {
		a = points[c].p[0].x > 0 ? points[c].length - 1 : 0;
		b = (a + 1) % points[c].length;
		for (i = 0; i < gradient->length; i++) {
			double y, x = (double)i / (double)gradient->length;
			double dx = fmod(1 + points[c].p[b].x - points[c].p[a].x, 1.0);
			while (x > points[c].p[b].x) {
				b = ((a = (a + 1) % points[c].length) + 1) % points[c].length;
				if (points[c].p[b].x < points[c].p[a].x) points[c].p[b].x++;
			}
			x -= points[c].p[a].x;	/* shift X */
			if (dx) x /= dx;	/* scale X */
			y = (cos(M_PI * x) + 1) / 2;	/* interpolate! */
			y *= points[c].p[a].y - points[c].p[b].y;	/* scale Y */
			y += points[c].p[b].y;	/* shift Y */

			gradient->x[i].y[c] = (uint8_t)(y * 255 + .5); /* multiply and round */
		}
	}

	return gradient;
}

void usage (const char *myself) {
	printf("Usage: %s infile outfile\n\n", myself);
	puts("\t-v\tshow debug output");
	exit(1);
}

CHANNEL *add_point (CHANNEL *in, double x, double y) {
	POINT *new_p;
	new_p = realloc(in->p, (in->length + 1) * sizeof(POINT));
	if (!new_p) fail("failed to allocate memory");
	in->p = new_p;
	in->p[in->length].x = x;
	in->p[in->length].y = y;
	in->length++;
	return in;
}

/* do-nothing for when debug is disabled */
int nothing (const char *f, ...) { return 0; }

int main (int argc, char **argv) {
	size_t c, i;
	int (*debug)(const char *f, ...) = &nothing;
	CHANNEL points[CHANNELS];
	GRADIENT gradient;
	FILE *infile, *outfile;

	if (argc > 1 && !strcmp("-v", argv[1])) {
		debug = &printf;
		argc--; argv++;
	}

	if (argc < 2) usage(argv[0]);
	/* zero all lengths */
	gradient.length = 0;
	for (c = 0; c < CHANNELS; c++) points[c].length = 0;
	for (c = 0; c < CHANNELS; c++) points[c].p = NULL;

	/* read input file */
	if (NULL == (infile = fopen(argv[1], "r"))) fail(argv[1]);
	for (;;) {
		char line[256];
		double x, y;
		int o = 0;

		if (NULL == fgets(line, 256, infile)) {
			if (feof(infile)) break;
			fail("reading input file");
		}
		for (i = 0; ' ' == line[i] || '\t' == line[i]; i++);	/* skip whitespace */
		if ('#' == line[i] || '\n' == line[i]) continue;	/* skip comments */
		if (sscanf(line+i, "LEN%zu", &gradient.length)) {
			debug("gradient length: %llu\n", gradient.length);
			continue;
		} else if (sscanf(line+i, "RED%lf%lf", &x, &y)) {
			debug("read RED control point %lf/%lf\n", x, y);
			add_point(&points[RED], x, y);
		} else if (sscanf(line+i, "GRN%lf%lf", &x, &y)) {
			debug("read GRN control point %lf/%lf\n", x, y);
			add_point(&points[GRN], x, y);
		} else if (sscanf(line+i, "BLU%lf%lf", &x, &y)) {
			debug("read BLU control point %lf/%lf\n", x, y);
			add_point(&points[BLU], x, y);
		} else if (sscanf(line+i, "RGB%lf%n", &x, &o)) {
			/* not sure this works, need to test it */
			debug("read RGB control point %lf", x);
			for (c = 0; c < CHANNELS; c++) {
				i += (size_t)o;
				sscanf(line+i, "%lf%n", &y, &o);
				debug("%f,", y);
				add_point(&(points[c]), x, y);
			}
			if (debug == &printf) putchar('\n');
		} else debug("Warning: Unknown statement in config file: %s\n", line+i);
	}
	fclose(infile);

	for (c = 0; c < CHANNELS; c++)
		for (i = 0; i < points[c].length; i++)
			debug("%s: %f/%f\n", channel[c], points[c].p[i].x, points[c].p[i].y);

	if (!gradient.length) die("Config must specify a gradient length (LEN)");
	gradient.x = calloc(gradient.length, sizeof(RGB24));

	/* actually generate the palette */
	generate_palette(points, &gradient);
	for (c = 0; c < gradient.length; c++) debug("%lf: %d, %d, %d\n",
		(double)c / (double)gradient.length, gradient.x[c].y[RED],
		gradient.x[c].y[GRN], gradient.x[c].y[BLU]
	);

	/* write the palette to outfile */
	outfile = fopen(argv[2], "w");
	fwrite(gradient.x, sizeof(RGB24), gradient.length, outfile);
	fclose(outfile);

	free(gradient.x);
	for (c = 0; c < 3; c++) free(points[c].p);

	return 0;
}
