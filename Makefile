# complexfractals/Makefile/v4 handwritten by Pegasus Epsilon <pegasus@pimpninjas.org>

# final image size
HEIGHT=$$((1080))
WIDTH=$$((1920))

# multi-sampling anti-aliasing
# 1 = off, 2 = 4x, 3 = 9x, 4 = 16x, 5 = 25x, etc.
MSAA=2

# palette shift and divider (play with them, you'll see)
SHIFT=0
# logarithm massively changes the scale of this.
LOGDIVIDE=.3
# cross-trap shading changes things too.
TRAPANGLE=0
TRAPDISTANCE=.005
TRAPDIVIDE=$(shell echo "scale=40;$(TRAPDISTANCE)*3" | bc)
# iteration on which to start the trap
# higher numbers remove more foreground trap lines
TRAPSTART=1
# julia just looks better without the logarithm
DIVIDE=64

### COORDINATES ###

# julia uses CENTER_R and CENTER_I below to determine
# "seed" and centers on 0+0i instead

# full set
#CENTER_R=-.75
#CENTER_I=0
#RADIUS_R=2.4

# nautilus
CENTER_R=-0.7766729
CENTER_I=-0.13661091
RADIUS_R=0.00016

# jellyfish thingy
#CENTER_R=-1.246696994556754700000000000000
#CENTER_I=0.075544637307713675000000000000
#RADIUS_R=0.000000000009639400389005459100

# pinwheel thingy
#CENTER_R=-1.746868633203969300000000000000
#CENTER_I=-0.003374101722025281200000000000
#RADIUS_R=0.00000000000035

# "Bird of Paradise" stress test
#CENTER_R=0.3750001200618655
#CENTER_I=0.2166393884377127
#RADIUS_R=0.000000000002

# You should never have to mess with anything below here, but feel free.

# square pixels always kthx
RADIUS_I=$(shell echo "scale=40;$(RADIUS_R)*$(HEIGHT)/$(WIDTH)" | bc)

CC=cc
LIBS=
CFLAGS=-Ofast -Werror -Wall -Wconversion -Wtraditional-conversion -pedantic -ansi -std=c99
LINK=$(CC) $(CFLAGS) $(LIBS) $^ -o $@

png:	mandelbrot.png julia.png palete.png

all:	mandelbrot julia palette render resample pngify

examples:	palette.png mandelbrot.png julia.png

mandelbrot.png:	pngify mandelbrot.rgb
	./$^ $(WIDTH) $(HEIGHT) $@ || true

mandelbrot.rgb:	resample mandelbrot.msaa
	./$^ $(WIDTH) $(MSAA) $@

mandelbrot.msaa:	render mandelbrot.map palette.bin
	./$^ $(SHIFT) $(TRAPDIVIDE) $@
	#./$^ $(SHIFT) $(DIVIDE) $@

mandelbrot.map:	mandelbrot
	time ./$^ --crosstrap $(TRAPANGLE) $(TRAPDISTANCE) $(TRAPSTART) $$(($(WIDTH)*$(MSAA))) $$(($(HEIGHT)*$(MSAA))) $(CENTER_R) $(CENTER_I) $(RADIUS_R) $(RADIUS_I) 0 $@
	#time $^ $$(($(WIDTH)*$(MSAA))) $$(($(HEIGHT)*$(MSAA))) $(CENTER_R) $(CENTER_I) $(RADIUS_R) $(RADIUS_I) 0 $@

mandelbrot:	mandelbrot.o iterator.o mapper.o
	$(LINK) -lm

julia.png:	pngify julia.rgb
	./$^ $(WIDTH) $(HEIGHT) $@ || true

julia.rgb:	resample julia.msaa
	./$^ $(WIDTH) $(MSAA) $@

julia.msaa:	render julia.map palette.bin
	./$^ $(SHIFT) $(TRAPDIVIDE) $@
	#./$^ $(SHIFT) $(DIVIDE) $@

julia.map:	julia
	time ./$^ --crosstrap $(TRAPANGLE) $(TRAPDISTANCE) $(TRAPSTART) $$(($(WIDTH)*$(MSAA))) $$(($(HEIGHT)*$(MSAA))) 0 0 2 1.125 $(CENTER_R) $(CENTER_I) 0 $@
	#time ./$^ $$(($(WIDTH)*$(MSAA))) $$(($(HEIGHT)*$(MSAA))) 0 0 2 1.125 $(CENTER_R) $(CENTER_I) 0 $@

julia:	julia.o iterator.o mapper.o
	$(LINK) -lm

palette.png:	pngify palette.bin
	./$^ 45 34 $@

palette.bin:	palette blueglow.txt
	./$^ $@

palette:	palette.c
	$(LINK) -lm

render:	render.c
	$(LINK) -lm

pngify:	pngify.c
	$(LINK) -lz

pal:
	rm palette.bin 2>/dev/null || true

map:
	rm mandelbrot.map julia.map 2>/dev/null || true

clean:	pal map
	rm julia.png mandelbrot.png palette.png \
		julia.rgb mandelbrot.rgb \
		julia.msaa mandelbrot.msaa \
		julia.o mandelbrot.o iterator.o mapper.o render.o resample.o pngify.o \
		julia mandelbrot palette render resample pngify 2>/dev/null || true
