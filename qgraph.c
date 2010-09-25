#include "qgraph.h"
#include <string.h>
#include <stdlib.h>

static void *
del_vertex(SList *item, void *del_data)
{
  SList *cur;
  int should_del_data = *(int *)del_data;
  struct qgraph_vertex * v = (struct qgraph_vertex *)slist_unbox(item);
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

struct qgraph *
qgraph_new(void)
{
  struct qgraph *g = (struct qgraph *)malloc(sizeof(struct qgraph));
  memset(g, 0, sizeof(struct qgraph));
  return g;
}

void
qgraph_del(struct qgraph *g, int del_data)
{
  slist_foreach(g->vertices, del_vertex, &del_data);
  free(g);
}

struct qgraph_vertex *
qgraph_add_vertex(struct qgraph *g, void *value)
{
  struct qgraph_vertex * v;
  if (g) {
    v = (struct qgraph_vertex *)malloc(sizeof(struct qgraph_vertex));
    if (v) {
      memset(v, 0, sizeof(struct qgraph_vertex *));
      v->data = value;
      g->vertices = slist_concat(g->vertices, slist_box(v));
      v->seq = g->v_num++;
    }
  }
  return v;
}

void
qgraph_add_w_edge(struct qgraph *g, struct qgraph_vertex * from,
    struct qgraph_vertex  *to, int w)
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
qgraph_add_edge(struct qgraph *g, struct qgraph_vertex * from,
    struct qgraph_vertex * to)
{
  qgraph_add_w_edge(g, from, to, 0);
}

struct qgraph_vertex *
qgraph_add_w_edge_v(struct qgraph *g, struct qgraph_vertex * from, void *to_val,
    int w)
{
  struct qgraph_vertex * to = NULL;
  to = qgraph_add_vertex(g, to_val);
  if (to) {
    qgraph_add_w_edge(g, from, to, w);
  }
  return to;
}

struct qgraph_vertex *
qgraph_add_edge_v(struct qgraph *g, struct qgraph_vertex * from, void *to_val)
{
  return qgraph_add_w_edge_v(g, from, to_val, 0);
}

void
qgraph_bfs(struct qgraph *g, struct qgraph_vertex * s, qgraph_discover_visitor
    dvisitor, void *arg) {
  int i, qsize = 0;
  SList *q; /* TODO this slist lacks a tail, making it inefficient as a queue */
  struct qgraph_vertex * cur;
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
    struct qgraph_vertex * adj;

    next = slist_tail(q);
    cur = (struct qgraph_vertex *)slist_unbox(q);
    q = next;
    qsize--;

    adj_node = cur->adj_list;
    while (adj_node) {
      adj = (struct qgraph_vertex *)adj_node->userdata;
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
qgraph_dfs_recursive(struct qgraph_vertex * v, short *state, int *time,
    qgraph_discover_visitor dv, qgraph_finish_visitor fv, void *arg)
{
  SList *curnode = v->adj_list;
  struct qgraph_vertex * cur;
  state[v->seq] = 1;
  if (dv) {
    dv(v, (*time)++, arg);
  }
  
  while (curnode) {
    cur = (struct qgraph_vertex *)curnode->userdata;
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
qgraph_dfs(struct qgraph *g, qgraph_discover_visitor dv,
    qgraph_finish_visitor fv, void *arg)
{
  short *state;
  int time = 0;
  SList *curnode = g->vertices;
  struct qgraph_vertex * cur;

  state = (short *)calloc(g->v_num, sizeof(short));

  while (curnode) {
    cur = (struct qgraph_vertex *)curnode->userdata;
    if (state[cur->seq] == 0) {
      qgraph_dfs_recursive(cur, state, &time, dv, fv, arg);
    }
    curnode = curnode->next;
  }
  free(state);
}

static void
topo_sort_finish_callback(struct qgraph_vertex * v, int ftime, void *arg)
{
  SList **l = (SList **)arg;
  if (*l) {
    *l = slist_concat(slist_box(v->data), *l);
  } else {
    *l = slist_box(v->data);
  }
}

SList *
qgraph_topo_sort(struct qgraph *g)
{
  SList *list = NULL;
  qgraph_dfs(g, NULL, topo_sort_finish_callback, &list);
  return list;
}

