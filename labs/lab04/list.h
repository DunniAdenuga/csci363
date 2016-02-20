#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>  /* for 'htonl' and 'ntohl' */

#define true  1
#define false 0

#define BUFFSIZE  4096
#define STRLEN    256

typedef int bool;

struct node_t {
  int    len;
  char   words[STRLEN];
  double rate;
  char   byte;
  struct node_t * next;
};

struct list_t {
  struct node_t * head;
  struct node_t * tail;
  int len; //number of nodes in list
};

void insert(struct list_t * list, struct node_t * node);
bool isEmpty(struct list_t list);
struct node_t * make_node(char * word, int len, double rate);
void print_list(struct list_t * list);

