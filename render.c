/* render.c/v3.0a2 - final RGB renderer for complexfractals.
** by Pegasus Epsilon <pegasus@pimpninjas.org>
** Distribute Unmodified -- http://pegasus.pimpninjas.org/license
**
**  CHANGELOG:
**  3.0a1 -- first version with a separated sampler/renderer.
**  3.0a2 -- fixed bug where "in-set" indicator was ignored.
**
**  TODO:
**  -- Add a way to specify the color for samples that are in-set.
**  -- Put in-pipeline supersampling back.
*/

#define _POSIX_SOURCE	/* fileno() */
#include <stdio.h>    /* printf(), puts(), FILE, fopen(), fwrite(), fclose(), fileno() */
#include <stdlib.h> 	/* exit(), atoi(), atof() */
#include <sys/mman.h> 	/* mmap() */
#include <sys/types.h>	/* fstat(), open(), close() */
#include <sys/stat.h> 	/* fstat(), open(), close() */
#include <fcntl.h>    	/* open(), close() */
#include <unistd.h>   	/* fstat() */
#include <math.h>     	/* log() */
#include <string.h>			/* strcmp() */

typedef struct {
	size_t size;
	int fd, shift;
	double divider;
	char *map;
} palette;

void usage(const char *myself) {
	printf("%s [OPTION] samplefile palfile shift divider outfile\n\n", myself);
	puts("\t-l\tapply the natural logarithm to each sample before mapping");
	puts("\t\t(flatten more)");
	puts("\t-2\tapply the base-2 logarithm to each sample before mapping");
	puts("\t\t(flatten less)");
	puts("\t\tOnly one logarithm may be applied at a time (for now)");
	exit(1);
}

double nothing (double x) { return x; }

int main (int argc, char **argv) {
	FILE *infile, *outfile;
	palette map;
	double (*flatten)(double) = &nothing;

	if (argc > 1) {
		if (0 == strcmp("-l", argv[1])) flatten = &log;
		if (0 == strcmp("-2", argv[1])) flatten = &log2;
		if (flatten != &nothing) { argc--; argv++; }
	}

	if (6 != argc) usage(argv[0]);

	infile = fopen(argv[1], "r");
	map.fd = open(argv[2], O_RDONLY);
	{
		struct stat info;
		fstat(map.fd, &info);
		map.size = (size_t)info.st_size;
	}
	map.map = mmap(NULL, map.size, PROT_READ, MAP_SHARED, map.fd, (size_t)0);
	map.shift = atoi(argv[3]);
	map.divider = atof(argv[4]);
	outfile = fopen(argv[5], "w");

	while (!feof(infile)) {
		static double sample;
		const unsigned char black[] = { 0, 0, 0 };
		fread(&sample, sizeof(double), (size_t)1, infile);
		if (0 <= sample)
			fwrite(map.map + 3 * (unsigned int)((double)map.size + sample * (double)map.size / map.divider + map.shift) % map.size, (size_t)1, (size_t)3, outfile);
		else
			fwrite(black, (size_t)1, (size_t)3, outfile);
	}
	fclose(infile);
	close(map.fd);
	fclose(outfile);

	return 0;
}
