#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// ---- STRUCTS ----

typedef struct {
    int x, y;
} square;

typedef struct {
    int dist;
    square from;
    square to;
} node;

typedef struct {
    int N;
    int len;
    node *data;
} Heap;

// ----
// ---- FORWARD DECLARATIONS ----

Heap *create_heap(int N);
static inline int compare(node a, node b);
static inline int get_parent(int i);
static inline int left_child(int i);
static inline int right_child(int i);
static inline void swap(Heap *h, int i, int j);
void swim(Heap *h, int i);
void sink(Heap *h, int i);
node pop(Heap *h);
void fix(Heap *h, node *n);

// -----

Heap *create_heap(int N) {
    Heap *h = malloc(sizeof *h);
    h->N = N;
    h->len = 0;
    h->data = malloc(N*sizeof(node));
    return h;
}

static inline int compare(node a, node b) {
    return (a.dist > b.dist) - (a.dist < b.dist);
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
    node temp = h->data[i];
    h->data[i] = h->data[j];
    h->data[j] = temp;
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

void push(Heap *h, node n) {
  if (h->len >= h->N) {
    perror("Heap overflow\n");
    abort();
  }
  h->data[h->len++] = n;
  swim(h, h->len-1);
}

node pop(Heap *h) {
  node n = h->data[0];
  swap(h, 0, --h->len);
  sink(h, 0);

  return n;
}

void fix(Heap *h, node *n) {
  int index = n - h->data;
  if (compare(h->data[index], h->data[get_parent(index)]) < 0) 
    swim(h, index);
  else 
    sink(h, index);
}

// ---- MAIN ----


node ***nodes;
Heap *heap;
int max_x;
int max_y;

int minimumObstacles(int** grid, int gridSize, int* gridColSize) {
  max_x = gridSize;
  max_y = *gridColSize;

  heap = create_heap(gridSize * *gridColSize);
  nodes = malloc(gridSize * sizeof(node **));
  for (int i=0; i<gridSize; i++) {
    nodes[i] = calloc(*gridColSize, sizeof(node *));
    for (int j=0; j<*gridColSize; j++) {
      nodes[i][j] = malloc(sizeof(node));
      nodes[i][j]->dist = INT_MAX;
      nodes[i][j]->from = (square){-1, -1};
      nodes[i][j]->to = (square){i, j};
    }
  }

  node *current = nodes[0][0];
  while (nodes[max_x][max_y]->dist == INT_MAX) {

  }

}

