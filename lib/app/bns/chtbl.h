#ifndef	CHTBL_H
#define CHTBL_H

#include <stdlib.h>

#include "list.h"

typedef struct CHTbl_ {
	int		buckets;

	int		(*h)(const void *key);
	int		(*match)(const void *key1, const void *key2);
	void	(*destroy)(void *data);

	int		size;
	List	*table;
} CHTbl;

/**
 * Public Interface
 **/

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

#endif








