CCOPTS=-Wall -pedantic -ansi -std=c99
HEADERS=iterator.h complex.h

all: sample render palette

render: render.c $(HEADERS)
	cc $(CCOPTS) $^ -o render

sample:	sample.c $(HEADERS)
	cc $(CCOPTS) -lm $^ -o sample

palette: palette.c $(HEADERS)
	cc $(CCOPTS) -lm $^ -o palette

clean:
	rm palette sample render || true
