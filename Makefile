# complexfractals/Makefile/v3 handwritten by Pegasus Epsilon <pegasus@pimpninjas.org>

# FIX THIS PATH TO MAKE PNG FILES DIRECTLY
CONVERT=/usr/src/ImageMagick-6.8.6-2/utilities/convert
# FIX THIS PATH TO MAKE PNG FILES DIRECTLY

# palette divider (play with it, you'll see)
DIVIDE=64
# final image size
HEIGHT=1080
WIDTH=1920
# julia uses CENTER_R and CENTER_I below to determine
# seed and centers on 0+0i instead
CENTER_R=-0.7766729
CENTER_I=0.13661091
RADIUS_R=0.00016
RADIUS_I=0.00009

# You should never have to mess with anything below here, but feel free.
CC=cc
LIBS=-lm
CFLAGS=-Ofast -Wall -pedantic -ansi -std=c99
LINK=$(CC) $(CFLAGS) $(LIBS) $^ -o $@
SAMPLE=$^ $$(($(WIDTH)*2)) $$(($(HEIGHT)*2)) $(CENTER_R) $(CENTER_I) $(RADIUS_R) $(RADIUS_I) 0 $@
RENDER=	$^ 0 $(DIVIDE) $@
PNGIFY=$(CONVERT) -size $$(($(WIDTH)*2))x$$(($(HEIGHT)*2)) -resize $(WIDTH)x$(HEIGHT) -depth 8 $? $@

all: mandelbrot julia palette render

mandelbrot.png: mandelbrot.rgb
	$(PNGIFY)

mandelbrot.rgb: render mandelbrot.map palette.bin
	$(RENDER)

mandelbrot.map: mandelbrot
	$(SAMPLE)

mandelbrot: mandelbrot.o iterator.o
	$(LINK)

julia.png: julia.rgb
	$(PNGIFY) 

julia.rgb: render julia.map palette.bin
	$(RENDER)

julia.map: julia
	$(SAMPLE)

julia: julia.o iterator.o
	$(LINK)

paltest.png: palette.bin
	$(PNGIFY) -size 45x34 -depth 8 rgb:$? $@

palette.bin: palette palette.txt
	$^ $@

palette: palette.c
	$(LINK)

clean:
	rm mandelbrot.map mandelbrot.rgb mandelbrot.png julia.map julia.rgb \
	julia.png palette.bin palette.png mandelbrot julia palette render \
	mandelbrot.o julia.o iterator.o 2>/dev/null || true
