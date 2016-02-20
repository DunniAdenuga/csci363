#include "list.h"
/*
 * Insert a node to the list
 * Precondition: node is made, list itself is not null
 * Postcondition: node is inserted into the list
 */
void insert(struct list_t * list, struct node_t * node) {

  if (list->head == NULL) { // new list
    list->head = node;
  } else {   // list already there
    list->tail->next = node;
  }
  list->tail = node;
  list->len++;
}

/*
 * Check if the list is empty
 * Precondition: list itself is not null
 * Postcondition: none changed
 */
bool isEmpty(struct list_t list) {

  return (list.head == NULL);
}

/*
 * Make a list node with a quote and a count
 * Precondition: none
 * Postcondition: a list node is made and returned
 */
struct node_t * make_node(char * word, int len, double rate) {

  struct node_t * anode = (struct node_t *)malloc(sizeof(struct node_t));
  strcpy(anode->words, word);
  anode->len = len;
  anode->rate = rate;

  return anode;
}

/*
 * Print the content of the list
 * Precondition: a list has been made
 * Postcondition: the content of the list printed
 */
void print_list( struct list_t *thislist ) {

  struct node_t *current = thislist->head;
  while (current != NULL) {
    printf( "node ->\n%d\n%s\n===\n", current->len, current->words);
    current = current->next;
  }
}
