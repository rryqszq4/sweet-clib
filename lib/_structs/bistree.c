#include <stdlib.h>
#include <string.h>

#include "bistree.h"

static void destroy_right(Bistree *tree, BiTreeNode *node);

static void 
rotate_left(BiTreeNode **node)
{
	BiTreeNode *left, *grandchild;

	left = bitree_left(*node);

	if (((AvlNode *)bitree_data(left))->factor == AVL_LFT_HEAVY){
		bitree_left(*node) = bitree_right(left);
		bitree_right(left) = *node;
		((AvlNode *)bitree_data(*node))->factor = AVL_BALANCED;
		((AvlNode *)bitree_data(left))->factor = AVL_BALANCED;
		*node = left;
	}else {
		grandchild = bitree_right(left);
		bitree_right(left) = bitree_left(grandchild);
		bitree_left(grandchild) = left;
		bitree_left(*node) = bitree_right(grandchild);
		bitree_right(grandchild) = *node;

		switch (((AvlNode *)bitree_data(grandchild))->factor){
			
		}
	}
}