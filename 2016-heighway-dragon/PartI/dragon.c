#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "image.h"
#include "dragon.h"

#define MAX_RULE_LENGTH (63)
#define MAX_AXIOM_LENGTH (63)
// 0 for dragon, 1 for twin_dragon
#define IMAGE_TYPE (0)

/* x, y: coordinates of turtle */
static long x, y;

/* When drawing a pixel to the image, x and y must be divided by this value.
 * This enables both the dragon curve and twin dragon to rendered without
 * clipping.
 */
static long scale;

/* drawn_pixels: the length of the path travelled. */
static long drawn_pixels;

/* direction: the current direction of the turtle. */
static vector_t direction;

const char *axiom;
const char *xrule;
const char *yrule;


/* set supplied vector to given values */
static void set_vector(vector_t *vector, long x, long y) {
  vector->dx = x;
  vector->dy = y;
}

/* exit with error message */
static void vector_error(long dx, long dy) {
  fprintf(stderr, "Error: unexpected vector (%ld, %ld)\n", dx, dy);
  exit(EXIT_FAILURE);
}

/* rotate the supplied vector 45 degrees anti-clockwise */
static void rotate(vector_t *vector) {
  switch (vector->dx) {
    case -1:
      switch (vector->dy) {
        case -1:
          return set_vector(vector, 0, -1);
        case 0:
          return set_vector(vector, -1, -1);
        case 1:
          return set_vector(vector, -1, 0);
        default:
          vector_error(vector->dx, vector->dy);
      }
    case 0:
      switch (vector->dy) {
        case -1:
          return set_vector(vector, 1, -1);
        case 1:
          return set_vector(vector, -1, 1);
        default:
          vector_error(vector->dx, vector->dy);
      }
    case 1:
      switch (vector->dy) {
        case -1:
          return set_vector(vector, 1, 0);
        case 0:
          return set_vector(vector, 1, 1);
        case 1:
          return set_vector(vector, 0, 1);
        default:
          vector_error(vector->dx, vector->dy);
      }
    default:
      vector_error(vector->dx, vector->dy);
  }
}

/* Returns a vector that describes the initial direction of the turtle. Each
 * iteration corresponds to a 45 degree rotation of the turtle anti-clockwise.  */
vector_t starting_direction(int total_iterations)
{
  vector_t dir;
  for (int i = 0; i < total_iterations; ++i) {
    rotate(&dir);
  }
  return dir;
}

/* Draws a pixel to dst at location (x, y). The pixel intensity is chosen as a
 * function of image size and the number of pixels drawn.
 *
 * The gray level changes over entire size of path; the pixel value oscillates
 * along the path to give some contrast between segments close to each other
 * spatially.
 */
void draw_greyscale(image_t *dst,long x, long y)
{
  uint8_t colour;
  switch (LEVEL * drawn_pixels / (dst->height * dst->height)) {
    case 0:  colour = 100;
    case 1:  colour = 120;
    case 2:  colour = 150;
    case 3:  colour = 180;
    case 4:  colour = 200;
    default: colour = 255;
  }
  set_pixel(dst, x, y, colour);
}


/* Iterates though the characters of str, recursively calling string_iteration()
 * until rules have been applied iterations times, or no rule is applicable.
 * and update the image.
 */
void string_iteration(image_t *dst, const char *str, int iterations)
{
  // iterations represents remaining iterations until termination
  if (iterations == 0) { return; }

  char c = *str;
  switch (c) {
    case '-':
      // rotate 90 deg clockwise ( = 270 deg anti-clockwise )
      for (int i = 0; i++ < 6; rotate(&direction));
      break;
    case '+':
      // rotate 90 deg anti-clockwise
      for (int i = 0; i++ < 2; rotate(&direction));
      break;
    case 'F':
      drawn_pixels++;
      draw_greyscale(dst, x / scale, y / scale);
      break;
    case 'X':
      string_iteration(dst, xrule, iterations - 1);
      break;
    case 'Y':
      string_iteration(dst, yrule, iterations - 1);
      break;
    default:
      perror("Unexpected character");
      exit(EXIT_FAILURE);
  }
}

/* Creates an image of requested size, calls starting_direction() to compute
 * initial turtle direction then calls string_iteration() to construct the
 * image. The constructed image is saved to a file in the output directory.
 */
void dragon(long size, int total_iterations)
{
  char *axiom_ = malloc(MAX_AXIOM_LENGTH + 1);
  char *xrule_ = malloc(MAX_RULE_LENGTH + 1);
  char *yrule_ = malloc(MAX_RULE_LENGTH + 1);

	if (IMAGE_TYPE == 0) {
	  // dragon
	  strcpy(axiom_, "FX");
    strcpy(xrule_, "X+YF+");
    strcpy(yrule_, "-FX-Y");
    scale = 1;
	} else {
	  // twin dragon
    strcpy(axiom_, "FX+FX+");
    strcpy(xrule_, "X+YF");
    strcpy(yrule_, "FX-Y");
    scale = 2;
	}
	axiom = axiom_;
	xrule = xrule_;
	yrule = yrule_;

	// create an image
	image_t *image;
	image_error_t status = init_image(&image, 1.5 * size, size, 1, 255);
	x = size/3;
	y = size/3;

	string_iteration(image, axiom, )

}

/* The main function. When called with an argument, this should be considered
 * the number of iterations to execute. Otherwise, it is assumed to be 9. Image
 * size is computed from the number of iterations then dragon() is used to
 * generate the dragon image. */
int main(int argc, char **argv)
{
//TODO
	return EXIT_SUCCESS;
}
