#include "list.h"
#include "bitree.h"
#include "traverse.h"

int inorder(const BiTreeNode *node, List *list){
	if (!bitree_is_eob(node)){

		if (!bitree_is_eob(bitree_left(node)))
			if (inorder(bitree_lef(node), list) != 0)
				return -1;

		if (list_ins_nex(list, list_tail(list), bitree_data(node)) != 0)
			return -1;

		if (!bitree_is_eob(bitree_right(node)))
			if (inorder(bitree_right(node), list) != 0)
				return -1;
	}

	return 0;
}