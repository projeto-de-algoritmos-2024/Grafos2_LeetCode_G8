#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// ---- STRUCTS ----

typedef struct {
    int x, y;
} vec2;

typedef struct node {
    int dist;
    struct node *from;
    int heap_index;
    vec2 pos;
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
    return (a->dist > b->dist) - (a->dist < b->dist);
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

node **nodes;
Heap *heap;
int max_x;
int max_y;

vec2 neighbors[5];
void set_neighbors(node *n) {
    for (int i=0; i<5; i++) {
      neighbors[i] = (vec2) {-1, -1};
    }
    
    int len = 0;
    if (n->pos.x - 1 >= 0) 
        neighbors[len++] = (vec2){n->pos.x - 1, n->pos.y};
    if (n->pos.x + 1 < max_x) 
        neighbors[len++] = (vec2){n->pos.x + 1, n->pos.y};
    if (n->pos.y - 1 >= 0) 
        neighbors[len++] = (vec2){n->pos.x, n->pos.y - 1};
    if (n->pos.y + 1 < max_y) 
        neighbors[len++] = (vec2){n->pos.x, n->pos.y + 1};
}

int minimumObstacles(int** grid, int gridSize, int* gridColSize) {
  max_x = gridSize;
  max_y = *gridColSize;

  heap = create_heap(gridSize * *gridColSize);
  nodes = malloc(gridSize * sizeof(node *));
  for (int i=0; i<gridSize; i++) {
    nodes[i] = calloc(*gridColSize, sizeof(node));
    for (int j=0; j<*gridColSize; j++) {
      node *current = &nodes[i][j];
      current->dist = INT_MAX;
      current->from = NULL;
      current->pos = (vec2){i, j};
      current->heap_index = -1;
      if (!(i == 0 && j == 0))
        push(heap, current);
    }
  }

  nodes[0][0].dist = 0;
  node *current = &nodes[0][0];
  while (heap->len != 0) {

    set_neighbors(current);
    for (int i=0; neighbors[i].x != -1 && neighbors[i].y != -1; i++) {

      vec2 pos = neighbors[i];
      if (pos.x == 0 && pos.y == 0)
        continue;

      node *friend_node = &nodes[pos.x][pos.y];
      int total_path_dist = grid[pos.x][pos.y] + current->dist;

      if (friend_node->dist > total_path_dist) {
        friend_node->dist = total_path_dist;
        friend_node->from = current;
        fix(heap, friend_node);
      }

    }
    current = pop(heap);
  }

  return nodes[max_x-1][max_y-1].dist;
}
