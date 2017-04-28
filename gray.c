/*
  Parallel Computing Final
  Initial draft by Kimberly McCarty
  Teammates:
  Frank Chiavetta <chiavettaf1@mail.montclair.edu>
  Ian Stypulkoski <stypulkoski1@mail.montclair.edu>,
  Donald Carter <carterd5@montclair.edu>


  Program loads in.ppm, and outputs a grayscale conversion as out.pgm


  to create input file:
  (using imagemagick)
  convert SOURCEFILE.JPG -compress none IN.PPM


  Compilation:
  gcc gray.c
  Running (on unix-y systems):
  ./a.out

  I suggest you copy and paste this code into a new file and edit it for parallel implementations

  I will merge it into the main method manually.
*/

//Standard includes
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//Method declarations
int *tograyscale(int* image, int width, int height);
int *loadimage(FILE *f);
void save_image(FILE *o, int* image, int width, int height);
int get_width(FILE *f);
int get_height(FILE *f);

int main(){
  //Create file pointer to input
  FILE *fp, *ofp;
  fp = fopen("in.ppm", "r");

  clock_t t;

  /* read image */
  int *image;
  image= loadimage(fp);
  int width = get_width(fp);
  int height = get_height(fp);
  fclose(fp);


  /* convert image */
  int* grayscale;

  //Around function call, we log the start
  printf("Starting the clock and converting image with serial algorithm\n");
  t = clock();
  grayscale = tograyscale(image, width, height);
  //Then we log the time when we finished
  //And we print the difference
  t = clock()-t;
  float secs = ((float)t)/CLOCKS_PER_SEC;
  printf("Serial conversion took %d cpu ticks, or %.3f seconds\n", (int) t, secs);

  //Now that we're off the clock, free up the memory we used for the unprocessed image, before we get to work saving the image
  free(image);


  /* save image to output file pointer */
  ofp = fopen("out.pgm", "w");
  save_image(ofp, grayscale, width, height);
  free(grayscale);
  fclose(ofp);

  //Done!
  return 0;
}

//function that loads the image as a one-dimensional list of subpixel values from the ppm file
int *loadimage(FILE *f){
  //simple buffer
  char buff[255];

  //canvas size
  int width;
  int height;
  int bit_depth;

  fscanf(f, "%s", buff);
  //below was for debug purposes
  //printf(buff);
  fscanf(f, "%d", &width);
  fscanf(f, "%d", &height);
  //FIXME: bit_depth not implemented, maybe should generalize program to process different bit_depths, time permitting
  fscanf(f, "%d", &bit_depth);
  int fields = width*height*3; //three channels of color (red green blue)
  //allocate a memory buffer for that many bytes...
  int * result =  malloc(fields * sizeof(int));
  for (int i = 0; i < fields; i++){
    //Read one string of decimals, store it as an integer, and move on
    fscanf(f, "%d", &result[i]);
  }
  return result;
}

//A simple function that checks what width is in the input file
int get_width(FILE *f){
  //simple buffer
  rewind(f);
  char buff[255];

  //canvas size
  int width;
  int height;
  int bit_depth;

  //integers for last pixel's colors
  fscanf(f, "%s", buff);
  fscanf(f, "%d", &width);
  fscanf(f, "%d", &height);

  return width;
}

//A simple function that checks what height is in the input file
int get_height(FILE *f){
  //simple buffer
  rewind(f);
  char buff[255];

  //canvas size
  int width;
  int height;
  int bit_depth;

  //integers for last pixel's colors
  fscanf(f, "%s", buff);
  fscanf(f, "%d", &width);
  fscanf(f, "%d", &height);

  return height;
}

//Take a one-dimensional array of subpixel values and the images resolution
//And output a grayscale array
//NOTE: Classmates, please edit this function!
int *tograyscale(int* image, int width, int height){
  //integers for last pixel's colors
  int colors[3];
  int grayscale;

  int fields = width*height; //three channels of color (red green blue)
  int color_fields = fields * 3;

  //allocate a memory buffer for that many bytes...
  int * grayscales = (int *) malloc(fields * sizeof(int));
  for (int i = 0; i < color_fields; i++){
    /* Got a tad in the c syntax here.
     fscanf: read a bit of text that matches the pattern %d (an integer)
     and write that to a field in colors, modulo 3 for if it's a red, green, or blue cell.
    */
    colors[i%3] = image[i];
    if (i % 3 == 2){
      int max = 0;
      int min = 255;
      for (int n = 0; n < 3; n++){
        if (max < colors[n]) max = colors[n];
        if (min < colors[n]) min = colors[n];
      }
      grayscale = (max + min) / 2; //desaturate by averaging brightest and darkest channel
      grayscales[i/3] =  grayscale;
    }
  }
  return grayscales;
}

//Take tograyscale's output, and make it into a valid pgm file
//Writing to the file at FILE *o.
void save_image(FILE *o, int* image, int width, int height){
  fprintf(o, "P2\n%d %d\n255\n", width, height);
  int length = width * height;
  for (int i = 0; i < length; i++){
    //add a new line if we're at the start of a row of pixels
    if (i % width == 0) fprintf(o, "\n");
    fprintf(o, "%d ", image[i]);
  }
}
