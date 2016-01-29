/*
 * Implementation of 'dlist,' a simple doubly linked list.
 * This version of the program has memory leak issues
 * and segmentation fault issues. Students are supposed 
 * to fix these problems with the tools of 'gdb' and
 * 'Valgrind'.
 * 
 * Xiannong Meng
 * 2015-12-07
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "dlist.h"

/*
 * Creates an empty list.
 *
 * Parameters: none.
 * Return: created list with head == tail == null.
 */
struct dlist * dlist_create() {

  struct dlist * new_list;

  new_list = (struct dlist *)malloc(sizeof(struct dlist));
  new_list->head = NULL;
  new_list->tail = NULL;

  return new_list;
}

/*
 * Insert a node into the list.
 * The node and the list have been created.
 *
 * Parameters:
 *   a_node:   the node to be inserted.
 *   the_list: the doubly linked to which the new node will be added.
 */
void dlist_insert(struct dnode * a_node, struct dlist * the_list) {

  if (dlist_is_empty(the_list) == true) { // insert at the begin
    the_list->head = a_node;
    the_list->tail = a_node;
    a_node->next = NULL;
    a_node->prev = NULL;
  } else {                                // insert at the end
    the_list->tail->next = a_node;
    a_node->prev = the_list->tail;
    a_node->next = NULL;
    the_list->tail = a_node;
  }
}

/*
 * Remove a node with specific value.
 *
 * Parameters:
 *   value_to_remove: specific value to remove
 *   the_list: the doubly linked list from which the node will be removed.
 */
void dlist_remove(char * value_to_remove, struct dlist * the_list) {

  struct dnode * node_to_remove;

  if (dlist_is_empty(the_list) == false) {
    node_to_remove = the_list->head;
    while (node_to_remove != NULL
	   && strcmp(node_to_remove->word, value_to_remove) != 0) {
      node_to_remove = node_to_remove->next;
    } // end of while

    if (node_to_remove != NULL) {
      // remove the node
      //node_to_remove->next->prev = node_to_remove->prev;
      if(node_to_remove->prev == NULL) {
	the_list->head = node_to_remove->next;
	node_to_remove->next->prev = NULL;
	
      }
      if(node_to_remove->next == NULL) {
	node_to_remove->prev->next = NULL;
      }
      if((node_to_remove->prev != NULL) && (node_to_remove->next != NULL)){

	node_to_remove->prev->next = node_to_remove->next;
      }
      
    }
    free(node_to_remove->word);
    free(node_to_remove);
  }  // end of list not empty
}

/*
 * Test if a list is empty.
 */
bool dlist_is_empty(struct dlist * the_list) {
  
  return (the_list->head == NULL);
}

/*
 * Traverse the list from begin to end, print all information.
 *
 * Parameters:
 *    a_list: the list to traverse.
 */
void dlist_traverse(struct dlist * a_list) {

  struct dnode * ptr;

  if (a_list == NULL)
    return;
  else {
    ptr = a_list->head;
    while (ptr != NULL) {
      printf("%d %s\n", ptr->count, ptr->word);
      ptr = ptr->next;
    } // end of while
  }
  free(ptr);
}

/*
 * make a new dnode with 'word' and 'count' as contents.
 * initialize all links to be null.
 *
 * Parameters:
 *   word: the word to be stored in the node.
 *   count: the count value to be stored.
 *
 * Return: the newly created node with contents.
 */
struct dnode * make_node(char * word, int count) {

  struct dnode * new_node = (struct dnode *)malloc(sizeof(struct dnode));

  new_node->word = (char *)malloc(strlen(word) + 1);
  strcpy(new_node->word, word);
  new_node->count = count;
  new_node->prev = new_node->next = NULL;

  return new_node;
}

void freeSpace(struct dlist * the_list){
  struct dnode *ptr;
  struct dnode *ptr2;
  if (the_list != NULL){
    ptr = the_list->head;
    while (ptr != NULL) {
      ptr2 = ptr->next;
      free(ptr->word);
      free(ptr);
      ptr = ptr2;
    } 
}
  
  free(the_list);
}
