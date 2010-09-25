#include "qgraph.h"
#include "adt.h"

static unsigned int
_vertex_hash(void *v)
{
  if (v) {
    return *(int *)v;
  }
}

static int
_vertex_eq(void *k1, void *k2)
{
  int *v1, *v2;
  v1 = (int *) k1;
  v2 = (int *) k2;

  return *v1 == *v2;
}

static int
_edge_comp(void *k1, void *k2)
{
  edge_t *e1, *e2;
  e1 = (edge_t *)k1;
  e2 = (edge_t *)k2;
  return e1->weight - e2->weight;
}

graph_t *
qgraph_mst_kruskal(graph_t *g)
{
  graph_t *r = NULL;
  if (r = qgraph_new()) {
    SList *v;
    struct djsets *djs;
    edge_t *edges;
    int i = 0;
    if ((djs = djsets_create(g->v_num, _vertex_hash, _vertex_eq, 0)) == NULL) {
      free(r);
      return NULL;
    }

    if ((edges = (edge_t *)malloc(sizeof(edge_t))) == NULL) {
      djsets_free(djs);
      free(r);
      return NULL;
    }

    for (v = g->vertices; g; g = g->next) {
      SList *vv;
      djsets_make_set(djs, ((graph_vertex_t)v->userdata)->seq);
      for (vv = ((graph_vertex_t)v->userdata)->adj_list; vv; vv = vv->next) {
        struct _adj_list_entry *vve;
        graph_vertex_t dst_v;
        edge_t *e = &edges[i++];
        vve = (struct _adj_list_entry *)vv->userdata;
        dst_v = (graph_vertex_t)vve->v;
        e->src = ((graph_vertex_t)v->userdata)->seq;
        e->dst = dst->seq;
        e->weight = vve->weight;
      }

      qsort(edges, g->e_num, sizeof(edge_t), _edge_comp);

      for (i = 0; i < g->e_num; i++) {
        edge_t e = edges[i];
        if (djsets_find_set(djs, e.src_seq) != djsets_find_set(djs,
              e.dst_seq)) {
          /* add an edge to the result graph (tree) */
        }
      }
    }

    djsets_free(djs);
  }
  return r;
}

