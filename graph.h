#ifndef _GRAPH_H
#define _GRAPH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "slist.h"

// digraph represented by adjacency lists

typedef struct _graph_vertex_t {
  SList      *adj_list;
  int         seq;
  const void *data;
} graph_vertex_t;

typedef struct _graph_t {
  int v_num; // number of vertices
  int e_num; // number of edges

  SList *vertices;
} graph_t;

typedef void (*qgraph_discover_visitor)(const graph_vertex_t *,
    int time, void *arg);

typedef void (*qgraph_finish_visitor)(const graph_vertex_t *,
    int time, void *arg);

graph_t *qgraph_new(void);

void qgraph_del(graph_t *g, int del_data);

graph_vertex_t * qgraph_add_vertex(graph_t * g, void *value);

void qgraph_add_edge(graph_t *g, graph_vertex_t *from, graph_vertex_t *to);

/* shortcut method */
graph_vertex_t * qgraph_add_edge_v(graph_t *g, graph_vertex_t *from,
    void *to_val);

void qgraph_bfs(graph_t *g, graph_vertex_t *src,
    qgraph_discover_visitor dvisitor, void *arg);

void qgraph_dfs(graph_t *g, qgraph_discover_visitor visitor,
    qgraph_finish_visitor fvisitor, void *arg);

SList * qgraph_topo_sort(graph_t *g);

#ifdef __cplusplus
}
#endif

#endif
