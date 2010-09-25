#ifndef _GRAPH_H
#define _GRAPH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "slist.h"

// digraph represented by adjacency lists

struct qgraph_vertex {
  SList      *adj_list;
  int         seq;
  const void *data;
};

struct _adj_list_entry {
  struct qgraph_vertex * v;  
  int weight;
};

struct qgraph {
  int v_num; // number of vertices
  int e_num; // number of edges

  SList *vertices;
};

typedef struct _edge_t {
  int src_seq;
  int dst_seq;
  int weight;
} edge_t;

typedef void (*qgraph_discover_visitor)(struct qgraph_vertex *, int time,
    void *arg);

typedef void (*qgraph_finish_visitor)(struct qgraph_vertex *, int time,
    void *arg);

struct qgraph *
qgraph_new(void);

void
qgraph_del(struct qgraph *g, int del_data);

struct qgraph_vertex *
qgraph_add_vertex(struct qgraph * g, void *value);

/* add a weighted edge to this graph. whether this edge is directional depends
 * on the intention of the api user.
 */
void
qgraph_add_w_edge(struct qgraph *g, struct qgraph_vertex * from,
    struct qgraph_vertex * to, int w);

/* add an unweighted edge to this graph. whether this edge is directional
 * depends on the intention of the api user.
 */
void
qgraph_add_edge(struct qgraph *g, struct qgraph_vertex * from,
    struct qgraph_vertex * to);

/* shortcut method by combining the operations of adding a new vertex and adding
 * an edge. the newly-added vertex is returned as a pointer */
struct qgraph_vertex *
qgraph_add_w_edge_v(struct qgraph *g, struct qgraph_vertex * from, void *to_val,
    int w);

/* shortcut method by combining the operations of adding a new vertex and adding
 * an edge. the newly-added vertex is returned as a pointer */
struct qgraph_vertex *
qgraph_add_edge_v(struct qgraph *g, struct qgraph_vertex * from, void *to_val);

void
qgraph_bfs(struct qgraph *g, struct qgraph_vertex * s,
    qgraph_discover_visitor dv, void *arg);

void
qgraph_dfs(struct qgraph *g, qgraph_discover_visitor dv,
    qgraph_finish_visitor fv, void *arg);

/*
 * topologically sort the graph by calling dfs. the caller should be responsible
 * for freeing the returned list object.
 */
SList *
qgraph_topo_sort(struct qgraph *g);

struct qgraph *
qgraph_mst_kruskal(struct qgraph *g);

#ifdef __cplusplus
}
#endif

#endif
