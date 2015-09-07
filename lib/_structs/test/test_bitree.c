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











