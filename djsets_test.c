#include "adt.h"
#include <stdio.h>
#include <assert.h>

unsigned int
djb2_hash(void *k)
{
  unsigned int hash = 5381;
  unsigned char * str = (unsigned char *)k;
  int c;
  while (c = *str++) {
    hash = hash * 33 ^ c;
  }
  return hash;
}

int
key_eq(void * k1, void *k2)
{
  char *x, *y;
  x = (char *)k1;
  y = (char *)k2;
  return strcmp(x, y) == 0;
}

int main()
{
  struct djsets *s;
  s = djsets_create(100, djb2_hash, key_eq, 0);

  djsets_make_set(s, "1");
  djsets_make_set(s, "2");
  djsets_make_set(s, "3");
  djsets_make_set(s, "4");
  assert(djsets_find_set(s, "1") != djsets_find_set(s, "2"));

  djsets_union(s, "1", "2");
  assert(djsets_find_set(s, "1") == djsets_find_set(s, "2"));

  djsets_union(s, "1", "3");
  assert(djsets_find_set(s, "2") == djsets_find_set(s, "3"));
  assert(djsets_find_set(s, "1") == djsets_find_set(s, "2"));
  assert(djsets_find_set(s, "1") == djsets_find_set(s, "3"));

  djsets_union(s, "2", "3"); /* should have no effect */
  assert(djsets_find_set(s, "2") == djsets_find_set(s, "3"));
  assert(djsets_find_set(s, "1") == djsets_find_set(s, "2"));
  assert(djsets_find_set(s, "1") == djsets_find_set(s, "3"));

  djsets_make_set(s, "5");
  assert(djsets_find_set(s, "3") != djsets_find_set(s, "5"));

  djsets_union(s, "3", "5");
  assert(djsets_find_set(s, "2") == djsets_find_set(s, "5"));
  assert(djsets_find_set(s, "1") == djsets_find_set(s, "5"));
  assert(djsets_find_set(s, "3") == djsets_find_set(s, "3"));
  assert(djsets_find_set(s, "3") != djsets_find_set(s, "4"));

  djsets_free(s);

  return 0;
}

