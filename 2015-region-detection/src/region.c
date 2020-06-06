#include "region.h"
#include "image.h"
#include "typedefs.h"
#include "list.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

/////ALL THESE FUNCTIONS ARE PROVIDED FOR YOU/////////////////////
/////DO NOT MODIFY THEM///////////////////////////////////////////

// Our colour function of choice
uint8_t region_colour(const region_t *region) {
  const unsigned depth = region->depth;
  return (depth * 1879u) % 255;
}

// Resets a point to (x,y)
void init_point(point_t *point, int x, int y) {
  point->x = x;
  point->y = y;
}

// Resets an extent to (width, height)
void init_extent(extent_t *extent, int width, int height) {
  extent->width = width;
  extent->height = height;
}


// Allocates a region. You may assume that the return value is non-NULL.
region_t *region_allocate(void) {
  region_t *new_region = malloc(sizeof(region_t));

  if (new_region == NULL) {
    perror("region_allocate");
    exit(EXIT_FAILURE);
  }

  return new_region;
}

// Prints a textual description of a region to the supplied FILE*.
void print_region(FILE *out, const region_t *region) {
  assert(out != NULL);
  assert(region != NULL);

  fprintf(out, "Region of depth %i at (%i, %i) of extent (%i, %i)\n",
          region->depth, region->position.x, region->position.y,
          region->extent.width, region->extent.height);
}

// Finds all regions located in "image" and adds them to "regions".
// Regions are added so that ordering according to the
// comparison function region_compare() is preserved.
void find_regions(list_t *regions, image_t *image) {
  region_t *image_region = region_allocate();

  image_region->depth = 0;
  init_point(&image_region->position, 0, 0);
  init_extent(&image_region->extent, image->width, image->height);

  list_insert_ascending(regions, image_region);
  find_sub_regions(regions, image, image_region);
}

///////////////////////////////////////////////////////////////////

/////////////////////////USEFUL FUNCTIONS///////////////////////////
///////////////////////////////////////////////////////////////////

// Compares two points lexicographically.
// Returns 1 if first is less than second, otherwise 0.
// Ordering of comparison is [y, x].
//
// You are NOT required to implement this function. However you may find
// it helpful to do so for Part I, Q1.
int point_compare_less(const point_t *first, const point_t *second) {
  return (first->y < second->y ||
          (first->y == second->y && first->x < second->x));
}

// Deallocates a region.
//
// You are NOT required to implement this function. However, you may
// find it helpful to do so for Part I, Q3.
void region_destroy(region_t *region) {
  free(region);
}
///////////////////////////////////////////////////////////////////

////////////////TO BE IMPLEMENTED///////////////////////////////////
////////////////////////////////////////////////////////////////////

// Compares two regions.
// Returns 1 if position of first is lexicographically less than that
// of the second, otherwise returns 0.
// Ordering of the position comparison is [y, x].
int region_compare(const region_t *r1, const region_t *r2) {
  assert(r1 && r2);
  return point_compare_less(&r1->position, &r2->position);
}

// Prints all regions in "regions" to "out".
// print_region (above) prints a textual description of a region
// to the supplied FILE*
void print_regions(FILE *out, list_t *regions) {
  for (list_elem_t *e = list_begin(regions);
       e != list_end(regions); e = e->next) {
    print_region(out, e->region);
  }
}

//
// Sets the specified region of image "image" to the intensity value "value".
//
void image_fill_region(image_t *image, const region_t *region, uint8_t value) {
  point_t pos = region->position;
  // traverse region left-to-right and top-to-bottom
  for (int y = pos.y; y < pos.y + region->extent.height; y++) {
    for (int x = pos.x; x < pos.x + region->extent.width; x++) {
      set_pixel(image, x, y, value);
    }
  }
}

// Determines the extent of a region.
// position: the location of the top-left-hand corner of the region.
// image: the image to be searched.
// extent: this will be populated with the width and height of a region.
void find_extent(extent_t *extent, image_t *image, const point_t *pos) {
  uint8_t shade = get_pixel(image, pos->x, pos->y);
  int x = pos->x, y = pos->y;

  for (; get_pixel(image, x, pos->y) == shade; x++);
  int width = x - pos->x;

  for (; get_pixel(image, pos->x, y) == shade; y++);
  int height = y - pos->y;

  init_extent(extent, width, height);
}

// Finds all regions located in the region "current" of "image" and adds them
// to "regions".  Regions are added so that ordering according to the
// comparison function region_compare() is preserved.
void
find_sub_regions(list_t *regions, image_t *image, const region_t *current) {

  point_t pos = current->position;
  uint8_t region_shade = get_pixel(image, pos.x, pos.y);
  int max_y = pos.y + current->extent.height;
  int max_x = pos.x + current->extent.width;

  // traverse current region left-to-right and top-to-bottom
  for (int y = pos.y; y < max_y; y++) {
    for (int x = pos.x; x < max_x; x++) {
      if (get_pixel(image, x, y) != region_shade) {
        // new region detected
        region_t *sub_region = region_allocate();
        sub_region->depth = current->depth + 1;
        init_point(&sub_region->position, x, y);
        find_extent(&sub_region->extent, image, &sub_region->position);
        // add to regions and recursively find_sub_regions
        list_insert_ascending(regions, sub_region);
        find_sub_regions(regions, image, sub_region);
        // erase detected region and continue search
        image_fill_region(image, sub_region, region_shade);
      }
    }
  }
}

// Renders all regions to an image using the supplied colour_function_t
// (declared in typedefs.h) to select pixel intensity.
void render_regions(image_t *image, list_t *regions,
                    colour_function_t get_colour) {
  for (list_elem_t *e = list_begin(regions); e != list_end(regions); e = e->next) {
    image_fill_region(image, e->region, get_colour(e->region));
  }
}
///////////////////////////////////////////////////////////////////////////////
