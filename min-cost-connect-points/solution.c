#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// ---- STRUCTS ----

typedef struct node {
    int smallest_dist;
    int heap_index;
    int id;
} node;

typedef struct {
    int N;
    int len;
    node **data;
} Heap;

// ---- FORWARD DECLARATIONS ----

Heap *create_heap(int N);
static inline int compare(node *a, node *b);
static inline int get_parent(int i);
static inline int left_child(int i);
static inline int right_child(int i);
static inline void swap(Heap *h, int i, int j);
void swim(Heap *h, int i);
void sink(Heap *h, int i);
void push(Heap *h, node *n);
node *pop(Heap *h);
void fix(Heap *h, node *n);

// ---- HEAP FUNCTIONS ----

Heap *create_heap(int N) {
    Heap *h = malloc(sizeof *h);
    h->N = N;
    h->len = 0;
    h->data = malloc(N*sizeof(node *));
    return h;
}

static inline int compare(node *a, node *b) {
    return (a->smallest_dist > b->smallest_dist) - (a->smallest_dist < b->smallest_dist);
}

static inline int get_parent(int i) {
    return (i-1)/2;
}

static inline int left_child(int i) {
    return i*2+1;
}

static inline int right_child(int i) {
    return i*2+2;
}

static inline void swap(Heap *h, int i, int j) {
    node *temp = h->data[i];
    h->data[i] = h->data[j];
    h->data[j] = temp;

    h->data[i]->heap_index = i;
    h->data[j]->heap_index = j;
}

void swim(Heap *h, int i) {
  int current = i;
  while (current > 0 && compare(h->data[current], h->data[get_parent(current)]) < 0) {
      int parent = get_parent(current);
      swap(h, current, parent);
      current = parent;
  }
}

void sink(Heap *h, int i) {
  int current = i;
  while (1) {
    int left = left_child(current);
    int right = right_child(current);
    int smallest = current;

    if (left < h->len && compare(h->data[left], h->data[smallest]) < 0)
      smallest = left;

    if (right < h->len && compare(h->data[right], h->data[smallest]) < 0)
      smallest = right;

    if (smallest == current) break;

    swap(h, current, smallest);
    current = smallest;
  }
}

void push(Heap *h, node *n) {
  if (h->len >= h->N) {
    perror("Heap overflow\n");
    abort();
  }
  h->data[h->len++] = n;
  n->heap_index = h->len-1;
  swim(h, h->len-1);
}

node *pop(Heap *h) {
  if (h->len == 0) {
    perror("Heap underflow\n");
    abort();
  }
  node *n = h->data[0];
  swap(h, 0, --h->len);
  sink(h, 0);

  n->heap_index = -1;
  return n;
}

void fix(Heap *h, node *n) {
  int index = n->heap_index;
  if (index == -1) {
    perror("Node not found in heap\n");
    abort();
  }

  if (index > 0 && compare(h->data[index], h->data[get_parent(index)]) < 0)
    swim(h, index);
  else 
    sink(h, index);    
}

// ---- MAIN ----

node *node_table;
Heap *heap;

typedef struct {
  int n;
  int **adj_matrix;
} graph;

static inline int get_dist(int *point_a, int *point_b) {
  return abs(point_a[0]-point_b[0]) + abs(point_a[1]-point_b[1]);
}

graph *build_graph(int** points, int pointsSize) {
  graph *g = malloc(sizeof *g);
  g->n = pointsSize;
  g->adj_matrix = malloc(pointsSize * sizeof(int *));
  for (int x=0; x<g->n; x++) {
    g->adj_matrix[x] = malloc(pointsSize * sizeof(int));
    for (int y=0; y<g->n; y++) {
      g->adj_matrix[x][y] = get_dist(points[x], points[y]);
    }
  }

  return g;
}

int minCostConnectPoints(int** points, int pointsSize, int* pointsColSize) {
  graph *g = build_graph(points, pointsSize);

  heap = create_heap(g->n);
  node_table = malloc(g->n * sizeof(node));

  for (int i=0; i<pointsSize; i++) {
    node *current = &node_table[i];
    current->smallest_dist = i==0 ? 0 : INT_MAX;
    current->id = i;
    current->heap_index = -1;
    if (i!=0)
      push(heap, current);
  }

  node *current = &node_table[0];
  while (heap->len != 0) {
    for (int j=0; j<g->n; j++) {
      node *neighbor = &node_table[j];
      if (j==current->id || neighbor->heap_index == -1)
        continue;

      int distance = g->adj_matrix[current->id][j];
      if (distance < neighbor->smallest_dist) {
        neighbor->smallest_dist = distance;
        fix(heap, neighbor);
      }

    }
    current = pop(heap);
  }

  int total = 0;
  for (int i=0; i<g->n; i++) {
    total += node_table[i].smallest_dist; 
  }

  return total;
}
