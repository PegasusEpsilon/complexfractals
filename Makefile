# FIX THIS PATH TO MAKE PNG FILES DIRECTLY
CONVERT=/usr/src/ImageMagick-6.8.6-2/utilities/convert
# FIX THIS PATH TO MAKE PNG FILES DIRECTLY

HEIGHT=450
WIDTH=800
# julia uses CENTER_R and CENTER_I below to
# determine seed and centers on 0+0i instead
CENTER_R=-0.7766729
CENTER_I=0.13661091
RADIUS_R=0.00016
RADIUS_I=0.00009
# anything that can't compile clean with these
# compiler flags is just plain bad code.
CCOPTS=-Wall -pedantic -ansi -std=c99
HEADERS=iterator.h types.h

all: mandelbrot julia render palette

mandelbrot:	mandelbrot.c $(HEADERS)
	cc $(CCOPTS) -lm mandelbrot.c -o mandelbrot

julia:	julia.c $(HEADERS)
	cc $(CCOPTS) -lm julia.c -o julia

palette: palette.c $(HEADERS)
	cc $(CCOPTS) -lm palette.c -o palette

render: render.c $(HEADERS)
	cc $(CCOPTS) render.c -o render

mandelbrot.png: mandelbrot.rgb
	$(CONVERT) -size $(WIDTH)x$(HEIGHT) -depth 8 mandelbrot.rgb mandelbrot.png

mandelbrot.rgb: render mandelbrot.map palette.bin
	$^ 0 32 mandelbrot.rgb

mandelbrot.map: mandelbrot
	$^ $(WIDTH) $(HEIGHT) $(CENTER_R) $(CENTER_I) $(RADIUS_R) $(RADIUS_I) 0 mandelbrot.map

julia.png: julia.rgb
	$(CONVERT) -size $(WIDTH)x$(HEIGHT) -depth 8 julia.rgb julia.png

julia.rgb: render julia.map palette.bin
	$^ 0 32 julia.rgb

julia.map: julia
	$^ $(WIDTH) $(HEIGHT) 0 0 2 1.125 $(CENTER_R) $(CENTER_I) 0 julia.map

paltest.png: palette.bin
	$(CONVERT) -size 45x34 -depth 8 rgb:palette.bin paltest.png

palette.bin: palette palette.txt
	$^ palette.bin

clean:
	rm mandelbrot julia palette render || true
	rm mandelbrot.map mandelbrot.rgb mandelbrot.png || true
	rm julia.map julia.rgb julia.png || true
	rm palette.bin paltest.png || true
