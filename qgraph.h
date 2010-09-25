#ifndef _GRAPH_H
#define _GRAPH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "slist.h"

// digraph represented by adjacency lists

struct _graph_vertex_t {
  SList      *adj_list;
  int         seq;
  const void *data;
};

typedef struct _graph_vertex_t * graph_vertex_t;

struct _adj_list_entry {
  graph_vertex_t v;  
  int weight;
};

typedef struct _graph_t {
  int v_num; // number of vertices
  int e_num; // number of edges

  SList *vertices;
} graph_t;

typedef struct _edge_t {
  int src_seq;
  int dst_seq;
  int weight;
} edge_t;

typedef void (*qgraph_discover_visitor)(graph_vertex_t, int time, void *arg);

typedef void (*qgraph_finish_visitor)(graph_vertex_t, int time, void *arg);

graph_t *
qgraph_new(void);

void
qgraph_del(graph_t *g, int del_data);

graph_vertex_t
qgraph_add_vertex(graph_t * g, void *value);

/* add a weighted edge to this graph. whether this edge is directional depends
 * on the intention of the api user.
 */
void
qgraph_add_w_edge(graph_t *g, graph_vertex_t from, graph_vertex_t to, int w);

/* add an unweighted edge to this graph. whether this edge is directional
 * depends on the intention of the api user.
 */
void
qgraph_add_edge(graph_t *g, graph_vertex_t from, graph_vertex_t to);

/* shortcut method by combining the operations of adding a new vertex and adding
 * an edge. the newly-added vertex is returned as a pointer */
graph_vertex_t
qgraph_add_w_edge_v(graph_t *g, graph_vertex_t from, void *to_val, int w);

/* shortcut method by combining the operations of adding a new vertex and adding
 * an edge. the newly-added vertex is returned as a pointer */
graph_vertex_t
qgraph_add_edge_v(graph_t *g, graph_vertex_t from, void *to_val);

void
qgraph_bfs(graph_t *g, graph_vertex_t s, qgraph_discover_visitor dv, void *arg);

void
qgraph_dfs(graph_t *g, qgraph_discover_visitor dv, qgraph_finish_visitor fv,
    void *arg);

/*
 * topologically sort the graph by calling dfs. the caller should be responsible
 * for freeing the returned list object.
 */
SList *
qgraph_topo_sort(graph_t *g);

graph_t *
qgraph_mst_kruskal(graph_t *g);

#ifdef __cplusplus
}
#endif

#endif
