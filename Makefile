CCOPTS=-Wall -pedantic -ansi -std=c99
HEADERS=iterator.h complex.h

all: mandelbrot julia render palette

mandelbrot:	mandelbrot.c $(HEADERS)
	cc $(CCOPTS) -lm mandelbrot.c -o mandelbrot

julia:	julia.c $(HEADERS)
	cc $(CCOPTS) -lm julia.c -o julia

palette: palette.c $(HEADERS)
	cc $(CCOPTS) -lm palette.c -o palette

render: render.c $(HEADERS)
	cc $(CCOPTS) render.c -o render

clean:
	rm mandelbrot julia palette render || true
