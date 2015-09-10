#ifndef BISTREE_H
#define BISTREE_H

#include "bitree.h"

#define	AVL_LFT_HEAVY	1
#define AVL_BALANCED	0
#define AVL_RGT_HEAVY 	-1

typedef struct AvlNode_
{
	void	*data;
	int		hidden;
	int 	factor;
} AvlNode;

typedef biTree BisTree;

void bistree_init(Bistree *tree, int (*compare)(const void *key1, const void *key2), void (*destroy)(void *data));
void bistree_destroy(Bistree *tree);
int bistree_insert(Bistree *tree, const void *data);
int bistree_remove(Bistree *tree, const void *data);
int bistree_lookup(Bistree *tree, void **data);

#define bistree_size(tree) ((tree)->size);

#endif;