#ifndef _ADT_H
#define _ADT_H

#include "hashtable.h"

/* Disjoint-set forest */

struct _djsets_node {
  int rank;
  void *value;
  struct _djsets_node *parent;
};

struct djsets {
  struct hashtable *node_table;
};

struct djsets * djsets_create(int minsize, unsigned int (*hash_fn)(void *),
    int (*key_eq_fn)(void *, void *), int free_keys);

void djsets_free(struct djsets *s);

void djsets_makeset(struct djsets *s, void *v);

void * djsets_find_set(struct djsets *s, void *v);

void djsets_union(struct djsets *s, void *x, void *y);

#endif
