#include "adt.h"
#include <stdlib.h>

struct djsets *
djsets_create(int minsize, unsigned int (*hash_fn)(void *),
    int (*key_eq_fn)(void *, void *), int free_keys)
{
  struct djsets *s;
  if ((s = (struct djsets *)malloc(sizeof(struct djsets)))) {
    if ((s->node_table = create_hashtable(minsize, hash_fn, key_eq_fn,
            free_keys, 1)) == NULL) {
      free(s);
    }
  }
  return s;
}

void
djsets_free(struct djsets *s)
{
  hashtable_destroy(s->node_table);
  free(s);
}

void
djsets_make_set(struct djsets *s, void *v)
{
  struct _djsets_node *node;
  if ((node = (struct _djsets_node *)malloc(sizeof(struct _djsets_node)))) {
    node->parent = node;
    node->rank = 0;
    node->value = v;
    hashtable_insert(s->node_table, v, node);
  }
}

static struct _djsets_node *
_djsets_find_set(struct djsets *s, void *v)
{
  struct _djsets_node *n;
  if ((n = hashtable_search(s->node_table, v))) {
    struct _djsets_node *j = n;
    while (j->parent != j) {
      j = j->parent;
    }
    /* now that j is the root of that set, we do path compression */
    while (n->parent != n) {
      n->parent = j;
      n = n->parent;
    }
    return j;
  }
  return NULL;
}

static void
_djsets_link(struct _djsets_node *x, struct _djsets_node *y)
{
  if (x && y) {
    if (x->rank > y->rank) {
      y->parent = x;
    } else {
      x->parent = y;
      if (x->rank == y->rank) {
        y->rank++;
      }
    }
  }
}

void *
djsets_find_set(struct djsets *s, void *v)
{
  struct _djsets_node *n;
  if ((n = _djsets_find_set(s, v))) {
    return n->value;
  }
  return NULL;
}

void
djsets_union(struct djsets *s, void *x, void *y)
{
  struct _djsets_node *xn, *yn;
  xn = _djsets_find_set(s, x);
  yn = _djsets_find_set(s, y);
  _djsets_link(xn, yn);
}

