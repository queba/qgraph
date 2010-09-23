#include "qgraph.h"
#include <string.h>
#include <stdlib.h>

static void *
del_vertex(SList *item, void *del_data)
{
  SList *cur;
  int should_del_data = *(int *)del_data;
  graph_vertex_t v = (graph_vertex_t)slist_unbox(item);
  cur = v->adj_list;
  while (cur) {
    SList *next = cur->next;
    free((void *)cur->userdata);
    free(cur);
    cur = next;
  }
  if (should_del_data) {
    free((void *)v->data);
  }
  free(v);
  return NULL;
}

graph_t *
qgraph_new(void)
{
  graph_t *g = (graph_t *)malloc(sizeof(graph_t));
  memset(g, 0, sizeof(graph_t));
  return g;
}

void
qgraph_del(graph_t *g, int del_data)
{
  slist_foreach(g->vertices, del_vertex, &del_data);
  free(g);
}

graph_vertex_t
qgraph_add_vertex(graph_t *g, void *value)
{
  graph_vertex_t v;
  if (g) {
    v = (graph_vertex_t)malloc(sizeof(struct _graph_vertex_t));
    if (v) {
      memset(v, 0, sizeof(struct _graph_vertex_t));
      v->data = value;
      g->vertices = slist_concat(g->vertices, slist_box(v));
      v->seq = g->v_num++;
    }
  }
  return v;
}

void
qgraph_add_w_edge(graph_t *g, graph_vertex_t from, graph_vertex_t to, int w)
{
  struct _adj_list_entry *e;
  e = (struct _adj_list_entry *)malloc(sizeof(struct _adj_list_entry));
  if (e) {
    e->v = to;
    e->weight = w;
    if (from->adj_list) {
      from->adj_list = slist_concat(from->adj_list, slist_box(e));
    } else {
      from->adj_list = slist_box(e);
    }
    g->e_num++;
  }
}

void
qgraph_add_edge(graph_t *g, graph_vertex_t from, graph_vertex_t to)
{
  qgraph_add_w_edge(g, from, to, 0);
}

graph_vertex_t
qgraph_add_w_edge_v(graph_t *g, graph_vertex_t from, void *to_val, int w)
{
  graph_vertex_t to = NULL;
  to = qgraph_add_vertex(g, to_val);
  if (to) {
    qgraph_add_w_edge(g, from, to, w);
  }
  return to;
}

graph_vertex_t
qgraph_add_edge_v(graph_t *g, graph_vertex_t from, void *to_val)
{
  return qgraph_add_w_edge_v(g, from, to_val, 0);
}

void
qgraph_bfs(graph_t *g, graph_vertex_t s, qgraph_discover_visitor dvisitor,
    void *arg) {
  int i, qsize = 0;
  SList *q; /* TODO this slist lacks a tail, making it inefficient as a queue */
  graph_vertex_t cur;
  short *state;
  int *distance;
  
  state = (short *)calloc(g->v_num, sizeof(short));
  distance = (int *)calloc(g->v_num, sizeof(int));

  q = slist_box(s);
  qsize++;
  state[s->seq] = 1;
  if (dvisitor)
    dvisitor(s, distance[s->seq], arg);
  while (qsize) {
    SList *adj_node, *next;
    graph_vertex_t adj;

    next = slist_tail(q);
    cur = (graph_vertex_t)slist_unbox(q);
    q = next;
    qsize--;

    adj_node = cur->adj_list;
    while (adj_node) {
      adj = (graph_vertex_t)adj_node->userdata;
      if (state[adj->seq] == 0) {
        state[adj->seq] = 1;
        distance[adj->seq] = distance[cur->seq] + 1;
        if (dvisitor)
          dvisitor(adj, distance[adj->seq], arg);
        q = slist_concat(q, slist_box(adj));
        qsize++;
      }
      adj_node = adj_node->next;
    }
    state[cur->seq] = 2;
  }
  free(state);
  free(distance);
}

static void
qgraph_dfs_recursive(graph_vertex_t v, short *state, int *time,
    qgraph_discover_visitor dv, qgraph_finish_visitor fv, void *arg)
{
  SList *curnode = v->adj_list;
  graph_vertex_t cur;
  state[v->seq] = 1;
  if (dv) {
    dv(v, (*time)++, arg);
  }
  
  while (curnode) {
    cur = (graph_vertex_t)curnode->userdata;
    if (state[cur->seq] == 0) {
      qgraph_dfs_recursive(cur, state, time, dv, fv, arg);
    }
    curnode = curnode->next;
  }
  state[v->seq] = 2;
  if (fv) {
    fv(v, (*time)++, arg);
  }
}

void
qgraph_dfs(graph_t *g, qgraph_discover_visitor dv, qgraph_finish_visitor fv,
    void *arg)
{
  short *state;
  int time = 0;
  SList *curnode = g->vertices;
  graph_vertex_t cur;

  state = (short *)calloc(g->v_num, sizeof(short));

  while (curnode) {
    cur = (graph_vertex_t)curnode->userdata;
    if (state[cur->seq] == 0) {
      qgraph_dfs_recursive(cur, state, &time, dv, fv, arg);
    }
    curnode = curnode->next;
  }
  free(state);
}

static void
topo_sort_finish_callback(graph_vertex_t v, int ftime, void *arg)
{
  SList **l = (SList **)arg;
  if (*l) {
    *l = slist_concat(slist_box(v->data), *l);
  } else {
    *l = slist_box(v->data);
  }
}

SList *
qgraph_topo_sort(graph_t *g)
{
  SList *list = NULL;
  qgraph_dfs(g, NULL, topo_sort_finish_callback, &list);
  return list;
}

