#include <stdio.h>
#include <stdlib.h>
#include <limits.h>


#define MAXN 100005

typedef struct {
    int node;
    int weight;
} Edge;

typedef struct {
    Edge* edges;
    int size;
    int capacity;
} AdjList;


typedef struct {
    int node;
    int cost;
} HeapNode;

typedef struct {
    HeapNode* data;
    int size;
    int capacity;
} PriorityQueue;

void addEdge(AdjList* graph, int u, int v, int w) {
    if (graph[u].size == graph[u].capacity) {
        graph[u].capacity *= 2;
        graph[u].edges = realloc(graph[u].edges, graph[u].capacity * sizeof(Edge));
    }
    graph[u].edges[graph[u].size++] = (Edge){v, w};
}

PriorityQueue* createPQ(int capacity) {
    PriorityQueue* pq = malloc(sizeof(PriorityQueue));
    pq->data = malloc(capacity * sizeof(HeapNode));
    pq->size = 0;
    pq->capacity = capacity;
    return pq;
}

void swap(HeapNode* a, HeapNode* b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void push(PriorityQueue* pq, int node, int cost) {
    if (pq->size == pq->capacity) {
        pq->capacity *= 2;
        pq->data = realloc(pq->data, pq->capacity * sizeof(HeapNode));
    }
    pq->data[pq->size++] = (HeapNode){node, cost};
    int i = pq->size - 1;
    while (i > 0 && pq->data[i].cost < pq->data[(i - 1) / 2].cost) {
        swap(&pq->data[i], &pq->data[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

HeapNode pop(PriorityQueue* pq) {
    HeapNode top = pq->data[0];
    pq->data[0] = pq->data[--pq->size];
    int i = 0;
    while (1) {
        int left = 2 * i + 1, right = 2 * i + 2, smallest = i;
        if (left < pq->size && pq->data[left].cost < pq->data[smallest].cost) smallest = left;
        if (right < pq->size && pq->data[right].cost < pq->data[smallest].cost) smallest = right;
        if (smallest == i) break;
        swap(&pq->data[i], &pq->data[smallest]);
        i = smallest;
    }
    return top;
}
