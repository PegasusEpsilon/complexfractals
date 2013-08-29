#!/bin/sh

make palette mandelbrot render resample pngify

palette blueglow.txt palette.bin

# PIPE ALL THE THINGS
rm mandelbrot.map mandelbrot.msaa mandelbrot.rgb 2>/dev/null
mkfifo mandelbrot.map mandelbrot.msaa mandelbrot.rgb

mandelbrot --crosstrap 0 .005 1 $((1920*4*16)) $((1080*4*16)) 0 0 2 1.25 -0.7766729 0.13661091 0 mandelbrot.map &
render mandelbrot.map palette.bin 0 .015 mandelbrot.msaa &
resample mandelbrot.msaa $((1920*4)) 16 mandelbrot.rgb &
pngify mandelbrot.rgb $((1920*4)) mandelbrot.png

rm mandelbrot.map mandelbrot.msaa mandelbrot.rgb
