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

int dijkstra(AdjList* graph, int n, int start, int end) {
    int* costs = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) costs[i] = INT_MAX;

    PriorityQueue* pq = createPQ(n);
    push(pq, start, INT_MAX);
    costs[start] = INT_MAX;

    while (!isEmpty(pq)) {
        HeapNode current = pop(pq);
        int node = current.node;
        int cost = current.cost;

        if (node == end) {
            freePQ(pq);
            free(costs);
            return cost;
        }

        if (cost < costs[node]) continue;

        for (int i = 0; i < graph[node].size; i++) {
            int neighbor = graph[node].edges[i].node;
            int weight = graph[node].edges[i].weight;
            int new_cost = cost & weight;

            if (new_cost > costs[neighbor]) {
                costs[neighbor] = new_cost;
                push(pq, neighbor, new_cost);
            }
        }
    }

    freePQ(pq);
    free(costs);
    return -1;
}

int* minimumCost(int n, int** edges, int edgesSize, int* edgesColSize, int** query, int querySize, int* queryColSize, int* returnSize) {
    // ----Initialize graph----
    AdjList* graph = malloc(n * sizeof(AdjList));
    for (int i = 0; i < n; i++) {
        graph[i].edges = malloc(2 * sizeof(Edge));
        graph[i].size = 0;
        graph[i].capacity = 2;
    }

    // ----Add edges to the graph----
    for (int i = 0; i < edgesSize; i++) {
        int u = edges[i][0];
        int v = edges[i][1];
        int w = edges[i][2];
        addEdge(graph, u, v, w);
        addEdge(graph, v, u, w);
    }

    // ----Allocate results array----
    int* results = malloc(querySize * sizeof(int));
    *returnSize = querySize;

    // ----Process each query----
    for (int i = 0; i < querySize; i++) {
        int start = query[i][0];
        int end = query[i][1];
        results[i] = dijkstra(graph, n, start, end);
    }

    // ----Free graph memory----
    for (int i = 0; i < n; i++) {
        free(graph[i].edges);
    }
    free(graph);

    return results;
}