/* v3.0a2/renderer half of the mandelbrot renderer.
** by Pegasus Epsilon <pegasus@pimpninjas.org>
** Distribute Unmodified -- http://pegasus.pimpninjas.org/license
**
**  CHANGELOG:
**  3.0a1 -- first version with a separated sampler/renderer.
**  3.0a2 -- fixed bug where "in-set" indicator was ignored.
**
**  TODO:
**  -- Add a way to specify the color for samples that are in-set.
*/

#define _POSIX_SOURCE	/* fileno() */
#include <stdio.h>    /* printf(), FILE, fopen(), fwrite(), fclose(), fileno() */
#include <stdlib.h> 	/* exit(), atoi(), atof() */
#include <sys/mman.h> 	/* mmap() */
#include <sys/types.h>	/* fstat(), open(), close() */
#include <sys/stat.h> 	/* fstat(), open(), close() */
#include <fcntl.h>    	/* open(), close() */
#include <unistd.h>   	/* fstat() */

typedef struct {
	int fd, size, shift;
	double divider;
	char *map;
} palette;

void usage(const char *myself) {
	printf("%s samplefile palfile shift divider outfile\n", myself);
	exit(1);
}

int main (int argc, char **argv) {
	FILE *infile, *outfile;
	palette map;

	if (6 != argc) usage(argv[0]);

	infile = fopen(argv[1], "r");
	map.fd = open(argv[2], O_RDONLY);
	{
		struct stat info;
		fstat(map.fd, &info);
		map.size = info.st_size;
	}
	map.map = mmap(NULL, map.size, PROT_READ, MAP_SHARED, map.fd, 0);
	map.shift = atoi(argv[3]);
	map.divider = atof(argv[4]);
	outfile = fopen(argv[5], "w");

	while (!feof(infile)) {
		static double sample;
		const unsigned char black[] = { 0, 0, 0 };
		fread(&sample, sizeof(double), 1, infile);
		if (0 <= sample)
			fwrite(map.map + 3 * (int)(map.size + (sample + map.shift) * map.size / map.divider) % map.size, 1, 3, outfile);
		else
			fwrite(black, 1, 3, outfile);
	}
	fclose(infile);
	close(map.fd);
	fclose(outfile);

	return 0;
}
