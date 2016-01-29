/*
 * Drive program to test a doubly linked list.
 *
 * Xiannong Meng
 * 2015-12-08
 *
 * To compile
 *    make
 * Sample run
 *   ./test_dlist < /usr/share/dict/words
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "dlist.h"

#define NUM_WORD 20      // number of words to test
#define WORD_LEN 64      // word length

int main(int argc, char* argv[]) {

  struct dlist * a_list = dlist_create();
  struct dnode * a_node;
  int i;
  int r;
  char word[NUM_WORD][WORD_LEN];
  
  srandom((int)time(NULL));     // initalize random number generator

  for (i = 0; i < NUM_WORD; i ++) {
    scanf("%s", word[i]);
    a_node = make_node(word[i], i);
    dlist_insert(a_node, a_list);
  }

  printf("after inserting %d words...\n", NUM_WORD);
  dlist_traverse(a_list);

  r = random() % NUM_WORD;
  printf("removing  node %d ...\n", r);
  dlist_remove(word[r], a_list);

  printf("removing node 0 - %dst node ...\n", 1);
  
  dlist_remove(word[0], a_list);
   

  printf("removing last node - the %dth node ...\n", NUM_WORD -1);
  dlist_remove(word[NUM_WORD-1], a_list);

  printf("after the removal...\n");
  dlist_traverse(a_list);
  
  //free(a_node);
  freeSpace(a_list);

  return 0;
}
