`page_list` is a dead simple data structure which
can hold a growing list of items of the same size.

It has the following API:


```c
// Creates a new page list. Pass in the size of each item in the list
// And how many items each page should be able to store.
void * page_list_new(size_t item_size, size_t items_per_page);

// Copies an item to the list. It's safe to free the item once it's added to the list
void page_list_add_item(void *list, void *item);

// Use this API to get the nth item in the list
void * page_list_get_item_by_index(void *list, size_t index);

// Iterates over the items and invokes a callback function with each item
void page_list_for_each_item(void *list, void (*each_callback) (void *item));

// Frees the whole list and items
void page_list_free(void *list);
```
