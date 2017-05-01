# Parallel Computing Final, Spring 2017

## Converting an image to grayscale

For this project, we choose a relatively simple task with a large dataset.

We want to convert a color bitmap image into a grayscale bitmap image

For simplicity, we selected the PGM/PPM formats.  PPM is an image format meant to be as simple as possible.  It's an ascii file with a header describing the image resolution and bit-depth, followed by a list of values.

In our case, we have a three-channel (Red, Green, Blue) image with one byte for each value.  So we parse three values at a time to get a pixel's values.

In the output, we have one grayscale value per pixel.

Our algorithm: gray_pix = (max(red, green, blue) + min(red, green, blue))/2

To run:
Ensure you have downloaded or converted an appropriate image.
This can be done with imagemagick, a popular free and open source image tool. Example:
convert SOURCEIMAGE -compress none in.ppm

Compilation on Linux:
mpicc -openmp gray.c -o a.out
Run ./a.out in.ppm

The program reads whatever .ppm image is specified in the argument, and outputs out.pgm, which is a grayscale bitmap.

