#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "page_list.h"

typedef struct _page {
  void *buffer;
  struct _page *next;
  int record_count;
} page;

typedef struct _page_list {
  page *first;
  page *last;
  int record_size;
  int records_per_page;
  int record_count;
} page_list;

static page * page_new(size_t record_size, size_t records_per_page) {
  page *p = malloc(sizeof(page));
  p->buffer = malloc(record_size * records_per_page);
  p->record_count = 0;
  p->next = NULL;
  return p;
}

static int no_space_left(page_list *list) {
  page *last_page = list->last;
  if(list->records_per_page > last_page->record_count)
    return 0;
  else
    return 1;
}

void * page_list_new(size_t record_size, size_t records_per_page) {
  page_list *list = malloc(sizeof(page_list));
  list->first = page_new(record_size, records_per_page);
  list->last = list->first;
  list->record_size = record_size;
  list->records_per_page = records_per_page;
  list->record_count = 0;
  return (void *)list;
}

void page_list_add_record(void *plist, void *record) {
  page_list *list = (page_list *)plist;
  if(no_space_left(list)) {
    page *new_page = page_new(list->record_size, list->records_per_page);
    list->last->next = new_page;
    list->last = new_page;
  }
  page *last_page = list->last;
  memcpy(last_page->buffer + (list->record_size * last_page->record_count), record, list->record_size);
  last_page->record_count++;
  list->record_count++;
}

void * page_list_get_record_by_index(void *plist, size_t index) {
  page_list *list = (page_list *)plist;
  if(index > list->record_count)
    return NULL;
  size_t page_number = index/list->records_per_page;
  size_t page_offset = index % list->records_per_page;
  page *current_page = list->first;
  while(page_number--){
    current_page = current_page->next;
  }
  return (void *)(current_page->buffer + (list->record_size * page_offset) );
}

void page_list_for_each_record(void *plist, void (*each_callback) (void *record)) {
  page_list *list = (page_list *)plist;
  page *current_page = list->first;
  while(current_page) {
    for (int i = 0; i < current_page->record_count; ++i) {
      each_callback(current_page->buffer + (i*list->record_size));
    }
    current_page = current_page->next;
  }
}

void page_list_free(void *plist){
  page_list *list = (page_list *)plist;
  page *current_page = list->first;
  page *prev_page = NULL;
  while(current_page) {
    prev_page = current_page;
    current_page = current_page->next;
    free(prev_page->buffer);
    free(prev_page);
  }
  free(list);
}

typedef struct _node {
  int value;
} node;

void print_value(void *x) {
  node *n = (node *)x;
  fprintf(stderr, "%d\n", n->value);
}

int main(){


  page_list *list = page_list_new(sizeof(node), 5);
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
