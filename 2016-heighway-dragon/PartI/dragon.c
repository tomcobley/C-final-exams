#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <assert.h>
#include "image.h"
#include "dragon.h"

#define MAX_RULE_LENGTH (63)
#define MAX_AXIOM_LENGTH (63)

// 0 for dragon, 1 for twin_dragon
#define IMAGE_TYPE (1)

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

char *axiom;
char *xrule;
char *yrule;


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
          set_vector(vector, 0, -1); return;
        case 0:
          set_vector(vector, -1, -1); return;
        case 1:
          set_vector(vector, -1, 0); return;
        default:
          vector_error(vector->dx, vector->dy);
      }
    case 0:
      switch (vector->dy) {
        case -1:
          set_vector(vector, 1, -1); return;
        case 1:
          set_vector(vector, -1, 1); return;
        default:
          vector_error(vector->dx, vector->dy);
      }
    case 1:
      switch (vector->dy) {
        case -1:
          set_vector(vector, 1, 0); return;
        case 0:
          set_vector(vector, 1, 1); return;
        case 1:
          set_vector(vector, 0, 1); return;
        default:
          vector_error(vector->dx, vector->dy);
      }
    default:
      vector_error(vector->dx, vector->dy);
  }
}

/* Returns a vector that describes the initial direction of the turtle. Each
 * iteration corresponds to a 45 degree rotation of the turtle anti-clockwise.  */
vector_t starting_direction(int total_iterations) {
  vector_t dir = {1, 0};
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
void draw_greyscale(image_t *dst, long x, long y) {
  uint8_t colour;
  switch (LEVEL * drawn_pixels / (dst->height * dst->height)) {
    case 0:
      colour = 100; break;
    case 1:
      colour = 120; break;
    case 2:
      colour = 150; break;
    case 3:
      colour = 180; break;
    case 4:
      colour = 200; break;
    default:
      colour = 255; break;
  }
  set_pixel(dst, x, y, colour);
}


/* Iterates though the characters of str, recursively calling string_iteration()
 * until rules have been applied iterations times, or no rule is applicable.
 * and update the image.
 */
void string_iteration(image_t *dst, const char *str, int iterations) {
  // iterations represents remaining iterations until termination
  if (iterations == 0) { return; }

  //printf("String = %s, iterations = %d\n", str, iterations);
  char c;

  while ((c = *(str++))) {
    switch (c) {
      // NOTE: first two cases switched, the spec appears to be wrong
      case '+':
        // rotate 90 deg clockwise ( = 270 deg anti-clockwise )
        for (int i = 0; i++ < 6; rotate(&direction));
        break;
      case '-':
        // rotate 90 deg anti-clockwise
        for (int i = 0; i++ < 2; rotate(&direction));
        break;
      case 'F':
        drawn_pixels++;
        draw_greyscale(dst, x / scale, y / scale);
        x += direction.dx;
        y += direction.dy;
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
}

/* Creates an image of requested size, calls starting_direction() to compute
 * initial turtle direction then calls string_iteration() to construct the
 * image. The constructed image is saved to a file in the output directory.
 */
void dragon(long size, int total_iterations) {
  axiom = malloc(MAX_AXIOM_LENGTH + 1);
  xrule = malloc(MAX_RULE_LENGTH + 1);
  yrule = malloc(MAX_RULE_LENGTH + 1);
  char *out = malloc(64);

  // initialisation settings vary depending on image type
  if (IMAGE_TYPE == 0) {
    // dragon
    strcpy(axiom, "FX");
    strcpy(xrule, "X+YF+");
    strcpy(yrule, "-FX-Y");
    scale = 1;
    strcpy(out, "../output/jurassicdragon.pgm");
    x = size / 3;
    y = size / 3;
  } else {
    // twin dragon
    strcpy(axiom, "FX+FX+");
    strcpy(xrule, "X+YF");
    strcpy(yrule, "FX-Y");
    scale = 2;
    strcpy(out, "../output/twindragon.pgm");
    x = size;
    y = size;
  }

  // create an image
  image_t *image;
  image_error_t status = init_image(&image, 1.5 * size, size, 1, 255);
  if (status != IMG_OK) {
    fprintf(stderr, "Error initialising image. \n");
    image_print_error(status);
    exit(EXIT_FAILURE);
  }

  string_iteration(image, axiom, total_iterations);

  // save image
  status = image_write(out, image, PGM_FORMAT);
  if (status != IMG_OK) {
    fprintf(stderr, "Error saving image. \n");
    image_print_error(status);
    exit(EXIT_FAILURE);
  }
}

/* The main function. When called with an argument, this should be considered
 * the number of iterations to execute. Otherwise, it is assumed to be 9. Image
 * size is computed from the number of iterations then dragon() is used to
 * generate the dragon image. */
int main(int argc, char **argv) {

  int iterations = 9;
  if (argc > 1) {
    iterations = atoi(argv[1]);
    assert(iterations > 0);
  }
  printf("Iterations: %u\n", iterations);
  long size = pow(2, iterations);
  direction = starting_direction(iterations);

  // To keep the correct resolution of the curves at rendering time
  //    the total number of iterations, total_iterations, should be twice the
  //    number of iterations
  dragon(size, 2 * iterations);

  printf("Exit success\n");

  return EXIT_SUCCESS;
}
