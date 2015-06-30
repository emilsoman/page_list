#include "page_list.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct _node {
  int value;
} node;

void print_value(void *x) {
  node *n = (node *)x;
  fprintf(stderr, "%d\n", n->value);
}

int main(){


  void *list = page_list_new(sizeof(node), 5);
  for (int i = 0; i < 400; ++i) {
    node *n= malloc(sizeof(node));
    n->value = i * 10;
    page_list_add_record(list, n);
    free(n);
  }

  page_list_for_each_record(list, print_value);
  page_list_free(list);
  return 0;
}
