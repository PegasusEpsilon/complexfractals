complexfractals
===============

Render fractals based on complex number arithmetic, in C99.

Output examples:

Julia set: http://pegasusepsilon.minus.com/mNei5OeRbTx55

Mandelbrot set: http://pegasusepsilon.minus.com/mbjMKPspCR

Requires C99 for long long. No other reason. Can easily be switched to C89/C90
by just removing that extra long. You'll lose precision, though.

"mandelbrot" and "julia" sample the complex plane and save normalized 
	sample data to the outputted sample map.

"palette" reads a palette specification from the input file and generates a
	three-channel cosine interpolated palette map in binary format,
	suitable for feeding to...

"render" reads the sample map and the palette map to generate the final raw
	output image. Feed this file to ImageMagick's convert, or 
	photoshop, or something similar to get a nice high-quality .png 
	of your desired render. Resize appropriately for multisampled
	anti-aliasing (MSAA).

Maximum resolution is effectively limited by your filesystem's maximum file
size. Most people won't be able to render to files larger than 2GB. To
circumvent filesystem limits, check the mkfifo(1) manpage.

ImageMagick has a difficult time with files excessively large, but there are
command-line parameters available to help with that. See the documentation for
details.

I'm pretty sure photoshop (and similar programs) flat-out can't handle images
this large. I've managed to crash many programs with output from this,
including IrfanView.

I have tested renders up to approximately 130,000px square using ImageMagick,
though.

All programs spit out a list of required parameters when you run them without
said parameters.

Edit the Makefile and fix the path to Imagemagick's convert to make PNG files
directly with make(1).

Type "make julia.png" or "make mandelbrot.png" or "make paltest.png" to see
sample output from everything.

TODO:
* add method to specify render color for in-set samples
* reenable in-pipe supersampling in render.c
