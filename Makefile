# complexfractals/Makefile/v3 handwritten by Pegasus Epsilon <pegasus@pimpninjas.org>

# FIX THIS PATH TO MAKE PNG FILES DIRECTLY
CONVERT=/usr/src/ImageMagick-6.8.6-2/utilities/convert
# FIX THIS PATH TO MAKE PNG FILES DIRECTLY

# final image size
HEIGHT=1080
WIDTH=1920

# palette shift and divider (play with them, you'll see)
SHIFT=0
MDIVIDE=.3	# logarithm massively changes the scale of this.
JDIVIDE=64	# julia just looks better without the logarithm
# julia uses CENTER_R and CENTER_I below to determine
# seed and centers on 0+0i instead

# full set
#CENTER_R=-.75
#CENTER_I=0
#RADIUS_R=2.4

# nautilus
CENTER_R=-0.7766729
CENTER_I=0.13661091
RADIUS_R=0.00016

# jellyfish thingy
#CENTER_R=-1.246696994556754700000000000000
#CENTER_I=-0.075544637307713675000000000000
#RADIUS_R=0.000000000009639400389005459100

# pinwheel thingy
#CENTER_R=-1.746868633203969300000000000000
#CENTER_I=0.003374101722025281200000000000
#RADIUS_R=0.00000000000035

# "Bird of Paradise" stress test
#CENTER_R=0.3750001200618655
#CENTER_I=-0.2166393884377127
#RADIUS_R=0.000000000002

# You should never have to mess with anything below here, but feel free.

# square pixels always kthx
RADIUS_I=$(shell echo "scale=40;$(RADIUS_R)*$(HEIGHT)/$(WIDTH)" | bc)

CC=cc
LIBS=-lm
CFLAGS=-Ofast -Wall -Wconversion -Wtraditional-conversion -pedantic -ansi -std=c99
LINK=$(CC) $(CFLAGS) $(LIBS) $^ -o $@
JRENDER=$^ $(SHIFT) $(JDIVIDE) $@
PNGIFY=$(CONVERT) -size $$(($(WIDTH)*2))x$$(($(HEIGHT)*2)) -resize $(WIDTH)x$(HEIGHT) -depth 8 $? $@

all: mandelbrot julia palette render

examples: palette.png mandelbrot.png julia.png

mandelbrot.png: mandelbrot.rgb
	$(PNGIFY) || true

mandelbrot.rgb: render mandelbrot.map palette.bin
	$< -l mandelbrot.map palette.bin $(SHIFT) $(MDIVIDE) $@

mandelbrot.map: mandelbrot
	$^ $$(($(WIDTH)*2)) $$(($(HEIGHT)*2)) $(CENTER_R) $(CENTER_I) $(RADIUS_R) $(RADIUS_I) 0 $@

mandelbrot: mandelbrot.o iterator.o mapper.o
	$(LINK)

julia.png: julia.rgb
	$(PNGIFY) || true

julia.rgb: render julia.map palette.bin
	$(JRENDER)

julia.map: julia
	$^ $$(($(WIDTH)*2)) $$(($(HEIGHT)*2)) 0 0 2 1.125 $(CENTER_R) $(CENTER_I) 0 $@

julia: julia.o iterator.o mapper.o
	$(LINK)

palette.png: palette.bin
	$(CONVERT) -size 45x34 -depth 8 rgb:$? $@

palette.bin: palette palette.txt
	$^ $@

palette: palette.c
	$(LINK)

render:	render.o
	$(LINK)

map:
	rm mandelbrot.map julia.map || true

clean: map
	rm mandelbrot.rgb mandelbrot.png julia.rgb julia.png \
	palette.bin palette.png mandelbrot julia palette render \
	mandelbrot.o julia.o iterator.o mapper.o render.o 2>/dev/null || true
