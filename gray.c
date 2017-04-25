#include <stdio.h>
#include <stdlib.h>

int *tograyscale(int* image, int width, int height);
int *loadimage(FILE *f);
void save_image(FILE *o, int* image, int width, int height);
int get_width(FILE *f);
int get_height(FILE *f);

int main(){
  FILE *fp, *ofp;
  fp = fopen("in.ppm", "r");
  /* read image */
  int *image;
  image= loadimage(fp);
  int width = get_width(fp);
  int height = get_height(fp);
  fclose(fp);
  /* convert image */
  int* grayscale;
  grayscale = tograyscale(image, width, height);
  /* save image */
  ofp = fopen("out.pgm", "w");
  save_image(ofp, grayscale, width, height);
  fclose(ofp);
  return 0;
}

int *loadimage(FILE *f){
  //simple buffer
  char buff[255];

  //canvas size
  int width;
  int height;
  int bit_depth;

  //integers for last pixel's colors
  fscanf(f, "%s", buff);
  printf(buff);
  fscanf(f, "%d", &width);
  fscanf(f, "%d", &height);
  //bit_depth not implemented, should change program to accomodate different bit_depths
  fscanf(f, "%d", &bit_depth);
  int fields = width*height*3; //three channels of color (red green blue)
  //allocate a memory buffer for that many bytes...
  int * result =  malloc(fields * sizeof(int));
  for (int i = 0; i < fields; i++){
    fscanf(f, "%d", &result[i]);
  }
  return result;
}

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
  printf(buff);
  fscanf(f, "%d", &width);
  fscanf(f, "%d", &height);

  return width;
}

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
  printf(buff);
  fscanf(f, "%d", &width);
  fscanf(f, "%d", &height);

  return height;
}

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

void save_image(FILE *o, int* image, int width, int height){
  fprintf(o, "P2\n%d %d\n255\n", width, height);
  int length = width * height;
  for (int i = 0; i < length; i++){
    //add a new line if we're at the start of a row of pixels
    if (i % width == 0) fprintf(o, "\n");
    fprintf(o, "%d ", image[i]);
  }
}
