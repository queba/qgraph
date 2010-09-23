#include "qgraph.h"
#include <stdio.h>

void
simple_discover_visit(graph_vertex_t v, int time, void *arg)
{
  printf(" %s ", (char *)v->data, time);
}

void
simple_finish_visit(graph_vertex_t v, int ftime, void *arg)
{
  printf(" %s ", (char *)v->data, ftime);
}

int main()
{
  graph_t *g;

  if ((g = qgraph_new()) != NULL) {
    graph_vertex_t s, w, z, t, x, y, u, v;
    s = qgraph_add_vertex(g, "s");
    z = qgraph_add_edge_v(g, s, "z");
    w = qgraph_add_edge_v(g, s, "w");
    qgraph_add_edge(g, z, w);
    v = qgraph_add_vertex(g, "v");
    qgraph_add_edge(g, v, s);
    y = qgraph_add_edge_v(g, z, "y");
    x = qgraph_add_edge_v(g, y, "x");
    qgraph_add_edge(g, w, x);
    qgraph_add_edge(g, x, z);
    t = qgraph_add_vertex(g, "t");
    qgraph_add_edge(g, t, v);
    u = qgraph_add_vertex(g, "u");
    qgraph_add_edge(g, u, t);
    qgraph_add_edge(g, t, u);
    qgraph_add_edge(g, u, v);
    qgraph_add_edge(g, v, w);

    /*qgraph_bfs(g, s, simple_discover_visit, NULL);*/
    qgraph_dfs(g, simple_discover_visit, simple_finish_visit, NULL);
    printf("\n");

    qgraph_del(g, 0);
  } else {
    fprintf(stderr, "out of mem\n");
  }

  return 0;
}

