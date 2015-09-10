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

#### stack
```c
typedef List Stack;

#define stack_init list_init
#define stack_destroy list_destroy

int stack_push(Stack *stack, const void *data);
int stack_pop(Stack *stack, void **data);

#define stack_peek(stack) ((stack)->head == NULL ? NULL : (stack)->head->data)
#define stack_size list_size
```


#### queue
```c
typedef List Queue;

#define queue_init list_init
#define queue_destroy list_destroy

int queue_enqueue(Queue *queue, const void *data);
int queue_dequeue(Queue *queue, void **data);

#define queue_peek(queue) ((queue)->head == NULL ? NULL : (queue)->head->data)
#define queue_size list_size
```

#### hash table
```c
int chtbl_init(CHTbl *htbl, int buckets, int (*h)(const void *key),int (*match)(const void *key1, const void *key2), void (*destroy)(void *data));
int chtbl_init_kv(CHTbl *htbl, int buckets, int (*h)(const void *key),int (*match)(const SweetListKv *key1, const SweetListKv *key2), void (*destroy)(void *data));
void chtbl_destroy(CHTbl *htbl);
int chtbl_insert(CHTbl *htbl, const void *data);
int chtbl_insert_kv(CHTbl *htbl, const SweetListKv *data);
int chtbl_remove(CHTbl *htbl, void **data);
int chtbl_remove_kv(CHTbl *htbl, SweetListKv **data);
int chtbl_lookup(const CHTbl *htbl, void **data);
int chtbl_lookup_kv(const CHTbl *htbl, SweetListKv **data);
SweetListKv *chtbl_find_kv(const CHTbl *htbl, SweetListKv **data);

#define chtbl_size(htbl) ((htbl)->size)
```

#### binary tree
```c
void bitree_init(BiTree *tree, void (*destroy)(void *data));
void bitree_destroy(BiTree *tree);
int bitree_ins_left(BiTree *tree, BiTreeNode *node, const void *data);
int bitree_ins_right(BiTree *tree, BiTreeNode *node, const void *data);
void bitree_rem_left(BiTree *tree, BiTreeNode *node);
void bitree_rem_right(BiTree *tree, BiTreeNode *node);
int bitree_merge(BiTree *merge, BiTree *left, BiTree *right, const void *data);

#define bitree_size(tree)		((tree)->size)
#define bitree_root(tree)		((tree)->root)
#define bitree_is_eob(node)		((node) == NULL)
#define bitree_is_leaf(node)	((node)->left == NULL && (node)->right == NULL)
#define bitree_data(node)		((node)->data)
#define bitree_left(node)		((node)->left)
#define bitree_right(node)		((node)->right)
```

#### binary search tree
but use AVL tree
```c
typedef biTree BisTree;

void bistree_init(Bistree *tree, int (*compare)(const void *key1, const void *key2), void (*destroy)(void *data));
void bistree_destroy(Bistree *tree);
int bistree_insert(Bistree *tree, const void *data);
int bistree_remove(Bistree *tree, const void *data);
int bistree_lookup(Bistree *tree, void **data);

#define bistree_size(tree) ((tree)->size);
```









