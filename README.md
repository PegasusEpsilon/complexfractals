complexfractals
===============

Render fractals based on complex number arithmetic, in C99.

Output examples:

Julia set: https://secure.flickr.com/photos/pegasusepsilon/sets/72157635276921082/

Mandelbrot set: https://secure.flickr.com/photos/pegasusepsilon/sets/72157638597810636/

Requires C99 for long long. No other reason. Can easily be switched to
C89/C90 by just removing that extra long. You'll lose precision, though.

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

"resample" takes a multi-sampled RGB file and averages pixels to produce
	a final RGB colormap.

"pngify" compresses the final RGB colormap and turns it into a .png.
	Obviously.

Maximum resolution is effectively limited by your filesystem's maximum file
size. Most people won't be able to render to files larger than 2GB. To
circumvent filesystem limits, or just save drive space, check the mkfifo(1)
manpage, and/or read the huge(mandelbrot|julia).sh scripts.

All programs spit out a list of required parameters when you run them without
said parameters.

Some programs accept a verbose flag, or various other optional flags to
change their behavior in some way.

Type "make julia.png" or "make mandelbrot.png" or "make palette.png" to see
sample output from everything.

TODO:
* add method to specify render color for in-set samples
