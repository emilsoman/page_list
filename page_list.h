#ifndef PAGE_LIST
#define PAGE_LIST
#include <stddef.h>

void * page_list_new(size_t record_size, size_t records_per_page);
void page_list_add_record(void *list, void *record);
void * page_list_get_record_by_index(void *list, size_t index);
void page_list_for_each_record(void *list, void (*each_callback) (void *record));
void page_list_free(void *list);
#endif
