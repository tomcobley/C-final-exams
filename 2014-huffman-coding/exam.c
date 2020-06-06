#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "exam.h"
#include "map.h"

/*
 * Private function prototypes.
 */

static void _print_huffman_tree(const huffman_tree_t *, int);

static void _print_huffman_tree_codes(const huffman_tree_t *, char *, char *);

/*
 * Prints the given Huffman tree.
 */
void print_huffman_tree(const huffman_tree_t *t) {
  printf("Huffman tree:\n");
  _print_huffman_tree(t, 0);
}

/*
 * Private helper function for print_huffman_tree.
 */
static void _print_huffman_tree(const huffman_tree_t *t, int level) {
  int i;
  for (i = 0; i <= level; i++) {
    printf("  ");
  }

  if (t->left == NULL && t->right == NULL) {
    printf("Leaf: '%c' with count %d\n", t->letter, t->count);
  } else {
    printf("Node: accumulated count %d\n", t->count);

    if (t->left != NULL) {
      _print_huffman_tree(t->left, level + 1);
    }

    if (t->right != NULL) {
      _print_huffman_tree(t->right, level + 1);
    }
  }
}

/*
 * Prints the codes contained in the given Huffman tree.
 */
void print_huffman_tree_codes(const huffman_tree_t *t) {
  printf("Huffman tree codes:\n");

  char *code = calloc(MAX_CODE_LENGTH, sizeof(char)), *code_position = code;
  if (code == NULL) {
    perror("calloc");
    exit(EXIT_FAILURE);
  }

  _print_huffman_tree_codes(t, code, code_position);
  free(code);
}

/*
 * Private helper function for print_huffman_tree_codes.
 */
static void _print_huffman_tree_codes(const huffman_tree_t *t,
                                      char *code, char *code_position) {

  if (t->left == NULL && t->right == NULL) {
    printf("'%c' has code \"%s\"\n", t->letter, code);
  } else if (t->left != NULL) {
    *code_position++ = 'L';
    *code_position = '\0';

    _print_huffman_tree_codes(t->left, code, code_position--);
  }

  if (t->right != NULL) {
    *code_position++ = 'R';
    *code_position = '\0';

    _print_huffman_tree_codes(t->right, code, code_position--);
  }
}

/*
 * Prints a list of Huffman trees.
 */
void print_huffman_tree_list(const huffman_tree_list_t *l) {
  printf("Huffman tree list:\n");

  for (; l != NULL; l = l->next) {
    print_huffman_tree(l->tree);
  }
}

/*
 * Frees a Huffman tree.
 */
void huffman_tree_free(huffman_tree_t *t) {
  if (t != NULL) {
    huffman_tree_free(t->left);
    huffman_tree_free(t->right);
    free(t);
  }
}

/*
 * Frees a list of Huffman trees.
 */
void huffman_tree_list_free(huffman_tree_list_t *l) {
  if (l != NULL) {
    huffman_tree_list_free(l->next);
    huffman_tree_free(l->tree);
    free(l);
  }
}

/*
 * Returns 1 if the string s contains the character c and 0 if it does not.
 */
int contains(char *s, char c) {
  for (; *s; s++) {
    if (*s == c) {
      return 1;
    }
  }
  return 0;
}

/*
 * Returns the number of occurrences of c in s.
 */
int frequency(char *s, char c) {
  int count;
  for (count = 0; *s; s++) {
    if (*s == c) {
      count++;
    }
  }
  return count;
}

/*
 * Takes a string s and returns a new heap-allocated string containing only the
 * unique characters of s.
 *
 * Pre: all strings will have fewer than or equal to MAX_STRING_LENGTH - 1
 *      characters.
 */
char *nub(char *s) {
  int len = strlen(s);
  char chars[len];
  int charindex = 0;
  for (int i = 0; i < len; i++) {
    if (!contains(chars, s[i])) {
      chars[charindex] = s[i];
      charindex++;
    }
  }
  char *chars_heap = malloc(charindex + 2);
  strcpy(chars_heap, chars);
  return chars_heap;
}

/*
 * Adds the Huffman tree t to the list l, returning a pointer to the new list.
 *
 * Pre:   The list l is sorted according to the frequency counts of the trees
 *        it contains.
 *
 * Post:  The list l is sorted according to the frequency counts of the trees
 *        it contains.
 */
huffman_tree_list_t *huffman_tree_list_add(huffman_tree_list_t *l,
                                           huffman_tree_t *t) {

  huffman_tree_list_t *node = malloc(sizeof(huffman_tree_list_t));
  node->tree = t;

  if (l == NULL || t->count <= l->tree->count) {
    // insert at start of list
    node->next = l;
    return node;
  }

  huffman_tree_list_t *head = l;
  for (; l->next; l = l->next) {
    if (t->count <= l->next->tree->count) {
      break;
    }
  }

  // insert node after l
  node->next = l->next;
  l->next = node;
  return head;
}

/*
 * Takes a string s and a lookup table and builds a list of Huffman trees
 * containing leaf nodes for the characters contained in the lookup table. The
 * leaf nodes' frequency counts are derived from the string s.
 *
 * Pre:   t is a duplicate-free version of s.
 *
 * Post:  The resulting list is sorted according to the frequency counts of the
 *        trees it contains.
 */
huffman_tree_list_t *huffman_tree_list_build(char *s, char *t) {

  huffman_tree_list_t *l = NULL;
  for (int i = 0; t[i]; i++) {
    huffman_tree_t *tree = calloc(1, sizeof(huffman_tree_t));
    tree->count = frequency(s, t[i]);
    tree->letter = t[i];
    // Note: left and right subtrees of h are NULL (by use of calloc)
    l = huffman_tree_list_add(l, tree);
  }

  return l;
}


/* 
 * Removes the first two elements from a list of Huffman trees
 *
 * Pre: l has at least two elements
 */
static huffman_tree_list_t *
huffman_tree_list_remove_two(huffman_tree_list_t *l) {
  huffman_tree_list_t *new_head = l->next->next;
  free(l->next);
  free(l);
  return new_head;
}


/*
 * Reduces a sorted list of Huffman trees to a single element.
 *
 * Pre:   The list l is non-empty and sorted according to the frequency counts
 *        of the trees it contains.
 *
 * Post:  The resuling list contains a single, correctly-formed Huffman tree.
 */
huffman_tree_list_t *huffman_tree_list_reduce(huffman_tree_list_t *l) {

  while (l->next) {
    // l->next is not NULL, so the list has at least 2 elements
    huffman_tree_t *node = malloc(sizeof(huffman_tree_t));
    huffman_tree_t *left = l->tree;
    huffman_tree_t *right = l->next->tree;

    node->count = left->count + right->count;
    node->letter = '\0';
    node->left = left;
    node->right = right;

    l = huffman_tree_list_remove_two(l);
    l = huffman_tree_list_add(l, node);
  }

  // l now has exactly one element, the final huffman tree
  return l;
}


void populate_code_map(huffman_tree_t *t, map_t *map,
                       char code[MAX_CODE_LENGTH + 1]) {

  if (t->letter) {
    char *code_ = malloc(strlen(code) + 1);
    strcpy(code_, code);
    insert(map, t->letter, code_);
    return;
  }

  // use stack allocated strings to simplify freeing
  char lcode[MAX_CODE_LENGTH + 1], rcode[MAX_CODE_LENGTH + 1];
  strcpy(lcode, code);
  strcpy(rcode, code);
  size_t i = strlen(code);
  lcode[i] = 'L';
  lcode[i + 1] = '\0';
  rcode[i] = 'R';
  rcode[i + 1] = '\0';

  populate_code_map(t->left, map, lcode);
  populate_code_map(t->right, map, rcode);

}

/*
 * Accepts a Huffman tree t and a string s and returns a new heap-allocated
 * string containing the encoding of s as per the tree t.
 *
 * Pre: s only contains characters present in the tree t.
 */
char *huffman_tree_encode(huffman_tree_t *t, char *s) {

  map_t *code_map = alloc_map();
  populate_code_map(t, code_map, "");
  char *code = malloc(MAX_CODE_LENGTH + 1);
  for (int i = 0; s[i]; i++) {
    strcat(code, find(code_map, s[i]));
  }
  free_map(code_map);
  return code;
}

/*
 * Accepts a Huffman tree t and an encoded string and returns a new
 * heap-allocated string contained the decoding of the code as per the tree t.
 *
 * Pre: the code given is decodable using the supplied tree t.
 */
char *huffman_tree_decode(huffman_tree_t *t, char *code) {
  char *str = malloc(MAX_CODE_LENGTH);
  int i = 0;
  huffman_tree_t *node = t;
  char c;

  while ((c = *code)) {
    assert(c == 'L' || c == 'R');
    node = (c == 'L') ? node->left : node->right;
    if (node->letter) {
      // leaf of tree reached
      str[i++] = node->letter;
      // reset node pointer to root of tree
      node = t;
    }
    code++;
  }

  // terminate string
  str[i] = '\0';
  return str;
}



