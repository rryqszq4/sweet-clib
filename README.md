sweet-clib
==========

C language programming library, sweet is a famours player.


## Data Structure

#### list
```c
void list_init(List *list, void (destroy)(void *data));
void list_destroy(List *list);
int list_ins_next(List *list, ListElmt *element, const void *data);
int list_rem_next(List *list, ListElmt *element, void **data);

#define list_size(list) ((list)->size)
#define list_head(list) ((list)->head)
#define list_is_head(list, element)	((element) == (list)->head ? 1 : 0)
#define list_is_tail(element)	((element)->next == NULL ? 1 : 0)
#define list_data(element)	((element)->data)
#define list_next(elements)	((element)->next)
```
