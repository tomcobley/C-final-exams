#include "binaryheap.h"
#include <assert.h>
#include <math.h>


/*allocate memory in the heap for a node_heap type element and returns a pointer to the new node_heap*/
node_heap *allocate_node_heap(void) {
  node_heap *heap = malloc(sizeof(node_heap));
  if (!heap) {
    perror("allocate_node_heap, memory allocation failed");
    exit(EXIT_FAILURE);
  }
  return heap;
}

/*initialise the heap array elements*/
void initial_heap(node_heap **heap, char *sequence) {
  for (int i = 0; sequence[i]; i++) {
    node_heap *node = allocate_node_heap();
    node->key = sequence + i;
    node->position = i + 1;
    heap[i + 1] = node;
  }
}

/*print every element of the heap array*/
void print_elem_heap(node_heap **heap, int length) {
  for (int i = 1; i <= length; printf("%c ", *heap[i++]->key));
  printf("\n");
  for (int i = 1; i <= length; printf("%d ", heap[i++]->position));
  printf("\n");
}


/* returns the index in the heap array where the parent is allocated for the index passed as argument*/

int parent(int index) {
  return floor(((double) index) / 2.0);
}

/* returns the index in the heap array where the left child is allocated for the index passed as argument*/

int left_child(int index) {
  return 2 * index;
}

/* returns the index in the heap array where the right child is allocated for the index passed as argument*/

int right_child(int index) {
  return 2 * index + 1;
}

/* exchange node_heap node1 to node_heap node2*/

void swap(node_heap *node1, node_heap *node2) {
  char *temp_char = node1->key;
  int temp_position = node1->position;
  node1->key = node2->key;
  node1->position = node2->position;
  node2->key = temp_char;
  node2->position = temp_position;
}


/*Moves down the value of the heap[current] so the subtree rooted at index "current" satisfies with the max-heap property*/

void max_heapify(node_heap **heap, int current, int heap_size) {
  //print_elem_heap(heap, heap_size);
  if (left_child(current) <= heap_size) {
    // current has a left child, so some action may be required
    int larger_subheap_index = left_child(current);

    if (right_child(current) <= heap_size) {
      // current also has a right child
      larger_subheap_index = *heap[right_child(current)]->key >
                             *heap[left_child(current)]->key
                             ? right_child(current)
                             : larger_subheap_index;
    }

    if (*heap[larger_subheap_index]->key > *heap[current]->key) {
      swap(heap[larger_subheap_index], heap[current]);
      max_heapify(heap, larger_subheap_index, heap_size);
    }
  }
}

/*it orders the heap so the ordered heap complies the max-heap property*/

void build_max_heap(node_heap **heap, int heap_size) {
  for (int i = heap_size; i >= 1; i--) {
    // max_heapify called on leaf nodes will have no effect
    max_heapify(heap, i, heap_size);
  }
}

/*starting from a max-heap ordered array, it moves the largest item from the heap and it into the array position left as the heap shrinks*/

void heapsort(node_heap **heap, int length) {
  build_max_heap(heap, length);
  while (length > 1) {
    swap(heap[1], heap[length]);
    length--;
    max_heapify(heap, 1, length);
  }
}

/*free the memory allocated by a node_heap type element in the heap*/

void free_node(node_heap *node) {
  free(node->key);
  free(node);
}

/*free the memory allocated in the heap array*/

void free_heap(node_heap **heap, int length) {
  for (int i = 1; i <= length; i++) {
    free_node(heap[i]);
    free(heap);
  }
}
