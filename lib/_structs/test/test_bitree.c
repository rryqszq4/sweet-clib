#include <stdio.h>
#include <stdlib.h>

#include "../bitree.h"
#include "../traverse.h"


static void 
print_preorder(const BiTreeNode *node)
{
	if (!bitree_is_eob(node)){
		fprintf(stdout, "Node=%03d\n", *(int *)bitree_data(node));

		if (!bitree_is_eob(bitree_left(node)))
			printf_preorder(bitree_left(node));

		if (!bitree_is_eob(bitree_right(node)))
			printf_preorder(bitree_right(node));
	}
	return ;
}

static void
print_inorder(const BiTreeNode *node)
{
	if (!bitree_is_eob(node)){

		if (!bitree_is_eob(bitree_left(node)))
			print_inorder(bitree_left(node));

		fprintf(stdout, "Node=%03d\n", *(int *)bitree_data(node));

		if (!bitree_is_eob(bitree_right(node)))
			print_inorder(bitree_right(node));
	}

	return ;
}

static void
print_postorder(const BiTreeNode *node)
{
	if (!bitree_is_eob(node)){

		if (!bitree_is_eob(bitree_left(node)))
			print_postorder(bitree_left(node));

		if (!bitree_is_eob(bitree_right(node)))
			print_postorder(bitree_right(node));

		fprintf(stdout, "Node=%03d\n", *(int *)bitree_data(node));
	}

	return ;
}

static int
insert_int(BiTree *tree, int i)
{
	BiTreeNode *node, *prev;
	int direction, *data;

	node = tree->node;
	direction = 0;

	while (!bitree_is_eob(node)){
		prev = node;
		if (i == *(int *)bitree_data(node)){
			return -1;
		}else if(i < *(int *)bitree_data(node)){
			node = bitree_left(node);
			direction = 1;
		}else {
			node = bitree_right(node);
			direction = 2;
		}
	}

	if ((data = (int *)malloc(sizeof(int))) == NULL)
		return -1;

	*data = i;

	if (direction == 0)
		return bitree_ins_left(tree, NULL, data);
	if (direction == 1)
		return bitree_ins_left(tree, prev, data);
	if (direction == 2)
		return bitree_ins_right(tree, prev, data);

	return -1;
}











