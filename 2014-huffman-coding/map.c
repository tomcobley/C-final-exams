#include "map.h"
#include <stdlib.h>
#include <stdio.h>

map_t *alloc_map() {
  map_t *map = calloc(1, sizeof(*map));
  if (map == NULL) {
    perror("alloc_map");
    exit(EXIT_FAILURE);
  }
  return map;
}

static map_elem_t *alloc_map_elem() {
  map_elem_t *elem = calloc(1, sizeof(*elem));
  if (elem == NULL) {
    perror("alloc_map");
    exit(EXIT_FAILURE);
  }
  return elem;
}

// Pre: an element with key 'key' does not already exist in the map
void insert(map_t *map, char key, char *value) {
  map_elem_t *new_elem = alloc_map_elem();
  new_elem->key = key;
  new_elem->value = value;

  if (!map->head) {
    // create new head (which is also the tail)
    map->tail = map->head = new_elem;
    return;
  }

  // note: due to the precondition that 'find' will only be called for an
  //    element that *is* in the list, storing the keys in order
  //    provides no performance benefit, so instead all elements are appended
  //    to the end of the list
  map->tail->next = new_elem;
  map->tail = map->tail->next;

}

// Pre: an element with key 'key' exists in the map. Its value will be returned
char *find(map_t *map, char key) {
  map_elem_t *node = map->head;
  while (node) {
    if (node->key == key) { return node->value; }
    node = node->next;
  }
  perror("Element not found\n");
  printf("   Element key: %c\n", key);
  exit(EXIT_FAILURE);
}

void free_map(map_t *map) {
  map_elem_t *node = map->head;
  while (node) {
    map_elem_t *next = node->next;
    free(node->value);
    free(node);
    node = next;
  }
  free(map);
}
