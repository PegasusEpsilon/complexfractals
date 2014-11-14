#define _POSIX_SOURCE	/* SSIZE_MAX */
#include <limits.h> 	/* SSIZE_MAX */

#include <stdio.h>  	/* printf(), puts(), sprintf(), fflush() */
#include <stdlib.h> 	/* exit(), strtoul(), malloc() */

#include <sys/types.h>	/* open() */
#include <sys/stat.h>	/* open() */
#include <fcntl.h>  	/* open() */

#include <unistd.h> 	/* read(), write(), close() */

#include <string.h> 	/* strlen() */

#include <errno.h>  	/* EIO, errno */

#ifdef _WIN32
#	define PRIuSIZET "Iu"
#	define PRIdSSIZET "Id"
#else
#	define PRIuSIZET "zu"
#	define PRIdSSIZET "zd"
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
	exit(errno);
}

__attribute__((noreturn))
void die (char *msg, int err) { puts(msg); exit(-err); }

ssize_t fullread (int fd, void *buf, size_t count) {
	if (count > SSIZE_MAX) die("row length too large, sorry", EIO);
	ssize_t got = 0, total = 0;
	for (;
		total != count
		&& 0 <= (got = read(fd, (char *)buf + total, count - (size_t)total));
		total += got
	) if (0 == got && EAGAIN != errno) { return total; }
	if (0 > got) fail("read error");
	if (total != count) fail("short read");
	return total;
}

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
	if (SSIZE_MAX < rowlen) die("row length too large, sorry", EIO);
	buf = malloc(rowlen);

	fullread(inputfd, buf, rowlen);
	eof = rows = 0;
	do {
		for (int c = 0; c < columns; c++) {
			sprintf(outputfile, "%s/row%02" PRIuSIZET "col%02d.rgb",
				argv[5], rows, c);
			outputfd[c] = open(outputfile, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		}

		for (int y = 0; y < height; y++) {
			for (size_t c = 0; c < columns; c++)
				if (width != write(outputfd[c], buf + c * width, width))
					die("write error", EIO);
			eof = (rowlen != fullread(inputfd, buf, rowlen));
		}

		for (int c = 0; c < columns; c++) close(outputfd[c]);

		rows++;

	} while (!eof);
	printf("tiled %" PRIuSIZET " rows\n", rows);

	return 0;
}
