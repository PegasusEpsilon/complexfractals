#!/bin/sh

make palette julia render resample pngify

palette blueglow.txt palette.bin

# PIPE ALL THE THINGS
rm julia.map julia.msaa julia.rgb 2>/dev/null
mkfifo julia.map julia.msaa julia.rgb

julia --crosstrap 0 .005 1 $((1920*4*16)) $((1080*4*16)) 0 0 2 1.25 -0.7766729 0.13661091 0 julia.map &
render julia.map palette.bin 0 .015 julia.msaa &
resample julia.msaa $((1920*4)) 16 julia.rgb &
pngify julia.rgb $((1920*4)) $((1080*4)) julia.png

rm julia.map julia.msaa julia.rgb
