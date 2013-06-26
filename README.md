complexfractals
===============

Render fractals based on complex number arithmetic, in C99.

Examples:
Julia set: http://pegasusepsilon.minus.com/mNei5OeRbTx55
Mandelbrot set: http://pegasusepsilon.minus.com/mbjMKPspCR

Requires C99 for long long. No other reason. Can easily be switched to C89/C90
by just removing that extra long. You'll lose precision, though.

"sample" samples the complex plane and saves normalized sample data to the
  outputted sample map.

"palette" reads a palette specification from the input file and generates a
  cosine interpolated palette map in binary format, suitable for feeding to...

"render" reads the sample map and the palette map to generate the final raw
  output image. Feed this file to ImageMagick's convert, or photoshop, or
  something similar to get a nice high-quality .png of your desired render.

All programs spit out a list of required parameters when you run them for the
first time. Better documentation and examples will be added when I'm bored.
