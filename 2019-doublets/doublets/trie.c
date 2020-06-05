#include "trie.h"

#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>


dictionary_t *create_dict(void) {
  return create_node();
}

void free_dict(dictionary_t *root) {
  free_node(root);
}

/* ------------ YOUR CODE BELOW -------------- */

#define ALPHABET_SIZE (26)

static int char_to_int(char c) {
  assert('A' <= c && c <= 'Z');
  return c - 'A';
}

bool find(dictionary_t *root, const char *word) {
  if (!root) { return false;}
  char c = word[0];
  if (!c) {
    // sentinel character reached (end of string), return end_of_word
    return root->end_of_word;
  }
  if (!isalpha(c)) {
    return false;
  }
  return find(root->children[char_to_int(c)], word + 1);
}

bool insert(dictionary_t *root, const char *word) {
  char c = word[0];
  if (!c) {
    // sentinel character reached (end of string), mark end of word, return true
    return root->end_of_word = true;
  }
  if (!isalpha(c)) {
    return false;
  }
  int char_index = char_to_int(c);
  if (!root->children[char_index]) {
    // no node exists for c, so create one
    root->children[char_index] = create_node();
  }
  return insert(root->children[char_index], word + 1);
}

dictionary_t *create_node(void) {
  dictionary_t *node = malloc(sizeof(dictionary_t));
  if (!node) {
    perror("create_node: memory allocation failed");
    exit(EXIT_FAILURE);
  }
  node->children = calloc(ALPHABET_SIZE, sizeof(dictionary_t *));
  if (!node->children) {
    perror("create_node: children memory allocation failed");
    exit(EXIT_FAILURE);
  }
  node->end_of_word = false;
  return node;
}

void free_node(dictionary_t *root) {
  if (!root) {return;}

  for (int i = 0; i < ALPHABET_SIZE; i++) {
    free_node(root->children[i]);
  }
  free(root->children);
  free(root);
}


bool load_from_file(dictionary_t *root, const char *filename) {
  FILE *fp = fopen(filename, "r");
  if (!fp) {return false;}

  char *word = malloc(MAX_WORD_SIZE + 1);
  fscanf(fp, "%s", word);
  while (!feof(fp)) {
    insert(root, word);
    fscanf(fp, "%s", word);
  }

  free(word);
  fclose(fp);
  return true;
}

#ifdef DICTIONARY_MAIN

// TODO: DO NOT MODIFY THIS FUNCTION!!! Remember to run:
//  valgrind --leak-check=full --show-leak-kinds=all ./dictionary

int main (void) {
  dictionary_t *d = create_dict();
  load_from_file(d, "words.txt");
  char *wordsnotin[] = { "ALGORITHMICALLY", "COMPUTER", "TRIE", "PIXEL", "ZEROED" };
  for (int i = 0; i < 5; i++) {
    if (find(d, wordsnotin[i])) {
      printf("Should not be able to find: %s\n", wordsnotin[i]);
    }
  }
  
  char *wordsin[] = { "BRAKES", "AVOWABLE", "AMOLE", "ALBUMEN", "BUZZER" };
  for (int i = 0; i < 5; i++) {
    if (!find(d, wordsin[i])) {
      printf("Should be able to find: %s\n", wordsin[i]);
    }
  }
  
  free_dict(d);
  return EXIT_SUCCESS;
}

#endif
