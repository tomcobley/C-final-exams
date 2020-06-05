#include "doublets.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ------------------ YOUR CODE HERE ------------------- */

// PRE: curr_word is a valid word (exists in the dictionary dict)
bool valid_step(dictionary_t *dict, const char *curr_word, const char *next_word) {
  size_t len = strlen(curr_word);
  if (len != strlen(next_word)) { return false; }

  int changes = 0;
  for (int i = 0; i < len; i++) {
    if (curr_word[i] != next_word[i]) {
      changes++;
    }
  }
  return changes == 1 && find(dict, next_word);
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
  for (i = 1; chain[i+1]; i++) {
    print_lower(chain[i]);
  }
  print_upper(chain[i]);
}

bool valid_chain(dictionary_t *dict, const char **chain) {
  dictionary_t *used_words = create_dict();
  insert(used_words, chain[0]);

  for (int i = 1; chain[i]; i++) {
    if (!valid_step(dict, chain[i-1], chain[i]) || find(used_words, chain[i])) {
      return false;
    }
  }
  return true;
}

//bool find_chain_internal(dictionary_t *dict, const char *word,
//                         const char *target_word, const char **chain, int max_words, int chain_length, int word_length) {
//
//
//  for (int i = 0; i < word_length; i++) {
//    // vary one character in word at a time
//  }
//
//}

bool find_chain(dictionary_t *dict, const char *start_word, 
                const char *target_word, const char **chain, int max_words) {

  return false;
  // Attempt at brute-force recursive search strategy
  //size_t length = strlen(word);

}

#ifdef DOUBLETS_MAIN

#define MAX_WORDS (7)

// TODO: DO NOT MODIFY THIS FUNCTION!!! Remember to run:
//  valgrind --leak-check=full --show-leak-kinds=all ./doublets

int main(void) {
  dictionary_t *dict = create_dict();
  load_from_file(dict, "words.txt");

  const char **chain = calloc(MAX_WORDS + 1, sizeof(char *));

//  const char **chain2 = calloc(MAX_WORDS + 1, sizeof(char *));
//  chain2[0] = "word";
//  chain2[1] = "WORS";
//  chain2[2] = "wars";
//  chain2[3] = "WART";
//  print_chain(chain2);

  bool success = find_chain(dict, "HARD", "EASY", chain, MAX_WORDS);
  printf("%s a Chain of %d words!\n",
         success ? "Found" : "Couldn't find", MAX_WORDS);
  if (success) {
    print_chain(chain);
    for(int i = 0; i < MAX_WORDS; i++)
      free((void *) chain[i]);
  }

  free(chain);
  free_dict(dict);

  return EXIT_SUCCESS;
}

#endif
