#include "binaryheap.h"
#include <string.h>

int main(int argc, char **argv){
  
//  node_heap **heaplist = malloc(sizeof(node_heap*) * 4);
//  char *sequence = malloc(4 + 1);
//  strcpy(sequence, "abcd");
//  initial_heap(heaplist, sequence);
//  print_elem_heap(heaplist, 4);

  char *str = argv[1];
  size_t length = strlen(str);
  node_heap **heap = malloc(sizeof(node_heap*) * strlen(str));
  initial_heap(heap, str);
  print_elem_heap(heap, length);

  build_max_heap(heap, length);
  print_elem_heap(heap, length);

  heapsort(heap, length);
  print_elem_heap(heap, length);

  return EXIT_SUCCESS;
}
