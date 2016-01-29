#ifndef LIST_H
#define LIST_H

/*
 * A simple doubly linked list
 * Xiannong Meng
 * 2015-12-07
 */

typedef int bool;
#define true  1
#define false 0

struct dnode {
  char * word;
  int    count;
  struct dnode * prev;
  struct dnode * next;
};

struct dlist {
  struct dnode * head;
  struct dnode * tail;
};

struct dlist * dlist_create();
void dlist_insert(struct dnode *, struct dlist *);
void dlist_remove(char *, struct dlist *);
bool dlist_is_empty(struct dlist *);
void dlist_traverse(struct dlist *);

struct dnode * make_node(char *, int);
void freeSpace(struct dlist*);
#endif /* ifndef LIST_H */
