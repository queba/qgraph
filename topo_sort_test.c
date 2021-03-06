#include "qgraph.h"
#include <stdio.h>

static void *
print_elem(SList *item, void *userdata)
{
  printf("%s ", (char *)slist_unbox(item));
  return NULL;
}

int main()
{
  struct qgraph *g;
  g = qgraph_new();
  if (g) {
    SList *sorted_list;

    struct qgraph_vertex *under, *pants, *belt, *jacket, *shirt, *tie, *socks,
                         *shoes;
    under = qgraph_add_vertex(g, "undershorts");
    pants = qgraph_add_edge_v(g, under, "pants");
    belt = qgraph_add_edge_v(g, pants, "belt");
    jacket = qgraph_add_edge_v(g, belt, "jacket");
    shirt = qgraph_add_vertex(g, "shirt");
    tie = qgraph_add_edge_v(g, shirt, "tie");
    qgraph_add_edge(g, tie, jacket);
    shoes = qgraph_add_edge_v(g, pants, "shoes");
    socks = qgraph_add_vertex(g, "socks");
    qgraph_add_edge(g, socks, shoes);
    qgraph_add_vertex(g, "watch");

    sorted_list = qgraph_topo_sort(g);
    slist_foreach(sorted_list, print_elem, NULL);
    printf("\n");

    qgraph_del(g, 0);
  }

  return 0;
}

