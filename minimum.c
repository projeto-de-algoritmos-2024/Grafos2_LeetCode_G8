#include <stdlib.h>

// ---- DATA ----
typedef struct {
    int x, y;
} square;

typedef struct {
    int dist;
    square from;
    square to;
} dist_node;

// --------

// ---- HEAP -----
typedef struct {
    int N;
    int len;
    dist_node **data;
} Heap;

inline int compare(dist_node *a, dist_node *b) {
    return (a->dist > b->dist) - (a->dist < b->dist);
}

inline int get_parent(int i) {
    return (i-1)/2;
}

inline int left_child(int i) {
    return i*2+1;
}

inline int right_child(int i) {
    return i*2+2;
}

inline void swap(Heap *h, int i, int j) {
    dist_node *temp = h->data[i];
    h->data[i] = h->data[j];
    h->data[j] = temp;
}

Heap *create_heap(int N) {
    Heap *h = malloc(sizeof *h);
    h->N = N;
    h->len = 0;
    h->data = calloc(N, sizeof(dist_node*));
    return h;
}

void swim(Heap *h, int i) {
    int current = i;
    while (current > 0 && compare(h->data[current], h->data[get_parent(current)]) > 0) {
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

    if (left < h->len && compare(h->data[current], h->data[smallest]) < 0)
      smallest = left;

    if (left < h->len && compare(h->data[right], h->data[smallest]) < 0)
      smallest = left;

    if (smallest == current) break;

    swap(h, current, smallest);
    current = smallest;
  }
}

dist_node *pop(Heap *h) {
  dist_node *n = h->data[0];
  swap(h, 0, --h->len);
  sink(h, 0);

  return n;
}

void fix(Heap *h, dist_node *n) {
  int index = n - *h->data;
  if (compare(n, h->data[get_parent(index)]) < 0) 
    swim(h, index);
  else 
    sink(h, index);
}

// ---- MAIN ----

dist_node **nodes;
Heap *heap;

int minimumObstacles(int** grid, int gridSize, int* gridColSize) {
    
}
