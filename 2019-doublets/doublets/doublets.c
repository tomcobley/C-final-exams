#include "doublets.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ------------------ YOUR CODE HERE ------------------- */
#define ALPHABET_SIZE (26)


bool
valid_step(dictionary_t *dict, const char *curr_word, const char *next_word) {
  size_t len = strlen(curr_word);
  if (len != strlen(next_word)) { return false; }

  int changes = 0;
  for (int i = 0; i < len; i++) {
    if (curr_word[i] != next_word[i]) {
      changes++;
    }
  }
  return changes == 1 && find(dict, curr_word) && find(dict, next_word);
}

static void print_upper(const char *str) {
  for (; *str; printf("%c", toupper(*(str++))));
  printf("\n");
}

static void print_lower(const char *str) {
  for (; *str; printf("%c", tolower(*(str++))));
  printf("\n");
}

void print_chain(const char **chain) {
  print_upper(chain[0]);
  int i;
  for (i = 1; chain[i + 1]; i++) {
    print_lower(chain[i]);
  }
  print_upper(chain[i]);
}

bool valid_chain(dictionary_t *dict, const char **chain) {
  dictionary_t *used_words = create_dict();
  insert(used_words, chain[0]);

  for (int i = 1; chain[i]; i++) {
    if (!valid_step(dict, chain[i - 1], chain[i]) ||
        find(used_words, chain[i])) {
      free_dict(used_words);
      return false;
    }
  }
  free_dict(used_words);
  return true;
}


bool in_chain(const char **chain, char *str) {
  for (int i = 0; chain[i]; i++) {
    if (!strcmp(chain[i], str)) { return true; }
  }
  return false;
}

bool find_chain_dfs(dictionary_t *dict, const char *curr_word,
                    const char *target_word, const char **chain, int max_words,
                    int step, size_t word_length) {

  if (step == max_words) { return false; }

  chain[step] = curr_word;
  if (!strcmp(curr_word, target_word)) { return true; }

  for (int i = 0; i < word_length; i++) {
    // vary one character in word at a time
    char *new_word = malloc(MAX_WORD_SIZE + 1);
    strcpy(new_word, curr_word);

    for (int j = 0; j < ALPHABET_SIZE; j++) {
      new_word[i] = (char) (((int) 'A') + j);
      if (find(dict, new_word) && !in_chain(chain, new_word)) {
        // new word is a valid word which hasn't been used before
        if (find_chain_dfs(dict, new_word, target_word, chain, max_words,
                           step + 1, word_length)) {
          return true;
        }
      }
    }
    free(new_word);
  }

  chain[step] = NULL;
  return false;
}

bool find_chain(dictionary_t *dict, const char *start_word,
                const char *target_word, const char **chain, int max_words) {

  // Attempt at brute-force recursive search strategy
  char *new_word = malloc(MAX_WORD_SIZE + 1);
  strcpy(new_word, start_word);
  return (find_chain_dfs(dict, new_word, target_word,
                     chain, max_words, 0, strlen(start_word)));
}

#ifdef DOUBLETS_MAIN

#define MAX_WORDS (7)

// TODO: DO NOT MODIFY THIS FUNCTION!!! Remember to run:
//  valgrind --leak-check=full --show-leak-kinds=all ./doublets

int main(void) {
  dictionary_t *dict = create_dict();
  load_from_file(dict, "words.txt");

  const char **chain = calloc(MAX_WORDS + 1, sizeof(char *));

  bool success = find_chain(dict, "HARD", "EASY", chain, MAX_WORDS);
  printf("%s a Chain of %d words!\n",
         success ? "Found" : "Couldn't find", MAX_WORDS);
  if (success) {
    print_chain(chain);
    for (int i = 0; i < MAX_WORDS; i++)
      free((void *) chain[i]);
  }

  free(chain);
  free_dict(dict);

  return EXIT_SUCCESS;
}

#endif
