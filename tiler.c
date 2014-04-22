#include <stdio.h>  	/* printf(), puts(), sprintf(), fflush() */
#include <stdlib.h> 	/* exit(), strtoul(), malloc() */

#include <sys/types.h>	/* open() */
#include <sys/stat.h>	/* open() */
#include <fcntl.h>  	/* open() */

#include <unistd.h> 	/* read(), write(), close() */

#include <string.h> 	/* strlen() */

#include <errno.h>  	/* EIO */

#ifdef _WIN32
#	define PRIuSIZET "Iu"
#	define PRIuOFFT "Id"
#else
#	define PRIuSIZET "zu"
#	define PRIuOFFT "zd"
#endif

__attribute__((noreturn))
void usage (char *argv0) {
	printf("Usage: %s <input> <width> <height> <columns> <directory>\n\n",
		argv0);
	puts("\tinput   \t...filename");
	puts("\twidth   \t...of a single tile");
	puts("\theight  \t...of a single tile");
	puts("\tcolumns \tHow many tiles wide the input image is");
	puts("\tdirectory\tAn output directory that will be filled with tiles");
	puts("\t\t\tThis directory MUST ALREADY EXIST");
	exit(0);
}

__attribute__((noreturn))
void fail (const char *msg) {   /* report function failures */
	perror(msg);
	exit(1);
}

__attribute__((noreturn))
void die (char *msg, int err) { puts(msg); exit(-err); }

int main (int argc, char **argv) {
	size_t width, height, columns, rows, rowlen, eof;
	int inputfd, *outputfd;
	char *outputfile, *buf;

	if (4 > argc) { usage(argv[0]); }

	inputfd = open(argv[1], O_RDONLY);
	width = 3 * strtoul(argv[2], NULL, 10); /* 3 == sizeof(pixel) */
	height = strtoul(argv[3], NULL, 10);

	columns = strtoul(argv[4], NULL, 10);
	outputfd = (int *)malloc(columns * sizeof(int));

	outputfile = (char *)malloc(strlen(argv[5])
		+ strlen("/row000col000.png\n"));

	rowlen = width * columns;
	buf = malloc(rowlen);

	if (rowlen != read(inputfd, buf, rowlen))
		die("read error when starting", EIO);

	eof = rows = 0;
	do {
		for (int c = 0; c < columns; c++) {
			sprintf(outputfile, "%s/row%03" PRIuSIZET "col%03d.rgb",
				argv[5], rows, c);
			outputfd[c] = open(outputfile, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		}

		for (int y = 0; y < height; y++) {
			for (size_t c = 0; c < columns; c++)
				if (width != write(outputfd[c], buf + c * width, width))
					die("write error", EIO);
			eof = (rowlen != read(inputfd, buf, rowlen));
		}

		for (int c = 0; c < columns; c++) close(outputfd[c]);

		rows++;

	} while (!eof);
	printf("tiled %" PRIuSIZET " rows\n", rows);

	return 0;
}
