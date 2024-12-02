#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define INF LLONG_MAX

typedef struct {
    int vertex;
    long long dist;
} HeapNode;

typedef struct {
    HeapNode* nodes;
    int size;
    int capacity;
} MinHeap;

typedef struct {
    int from, to, weight;
} Edge;

MinHeap* createMinHeap(int capacity) {
    MinHeap* heap = (MinHeap*)malloc(sizeof(MinHeap));
    heap->nodes = (HeapNode*)malloc(capacity * sizeof(HeapNode));
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void swapHeapNodes(HeapNode* a, HeapNode* b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void heapifyUp(MinHeap* heap, int idx) {
    int parent = (idx - 1) / 2;
    while (idx > 0 && heap->nodes[idx].dist < heap->nodes[parent].dist) {
        swapHeapNodes(&heap->nodes[idx], &heap->nodes[parent]);
        idx = parent;
        parent = (idx - 1) / 2;
    }
}

void heapifyDown(MinHeap* heap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < heap->size && heap->nodes[left].dist < heap->nodes[smallest].dist) {
        smallest = left;
    }
    if (right < heap->size && heap->nodes[right].dist < heap->nodes[smallest].dist) {
        smallest = right;
    }
    if (smallest != idx) {
        swapHeapNodes(&heap->nodes[idx], &heap->nodes[smallest]);
        heapifyDown(heap, smallest);
    }
}

void insertHeap(MinHeap* heap, int vertex, long long dist) {
    if (heap->size == heap->capacity) return;
    heap->nodes[heap->size] = (HeapNode){vertex, dist};
    heapifyUp(heap, heap->size);
    heap->size++;
}

HeapNode extractMin(MinHeap* heap) {
    HeapNode root = heap->nodes[0];
    heap->nodes[0] = heap->nodes[--heap->size];
    heapifyDown(heap, 0);
    return root;
}

bool isHeapEmpty(MinHeap* heap) {
    return heap->size == 0;
}

void freeHeap(MinHeap* heap) {
    free(heap->nodes);
    free(heap);
}

long long dijkstraWithHeap(Edge* edges, int edgesSize, int n, int source, int destination, long long* dist) {
    for (int i = 0; i < n; i++) dist[i] = INF;
    dist[source] = 0;

    MinHeap* heap = createMinHeap(n);
    insertHeap(heap, source, 0);

    while (!isHeapEmpty(heap)) {
        HeapNode current = extractMin(heap);
        int u = current.vertex;

        for (int i = 0; i < edgesSize; i++) {
            if (edges[i].from == u || edges[i].to == u) {
                int v = (edges[i].from == u) ? edges[i].to : edges[i].from;
                long long weight = edges[i].weight;

                if (dist[u] != INF && dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    insertHeap(heap, v, dist[v]);
                }
            }
        }
    }

    long long result = dist[destination];
    freeHeap(heap);
    return result;
}

int** modifiedGraphEdges(int n, int** edges, int edgesSize, int* edgesColSize, int source, int destination, int target, int* returnSize, int** returnColumnSizes) {
    Edge* edgeList = (Edge*)malloc(edgesSize * sizeof(Edge));
    for (int i = 0; i < edgesSize; i++) {
        edgeList[i] = (Edge){edges[i][0], edges[i][1], edges[i][2] == -1 ? 1 : edges[i][2]};
    }

    long long* dist = (long long*)malloc(n * sizeof(long long));
    long long shortestDistance = dijkstraWithHeap(edgeList, edgesSize, n, source, destination, dist);

    if (shortestDistance > target) {
        free(edgeList);
        free(dist);
        *returnSize = 0;
        *returnColumnSizes = NULL;
        return NULL;
    }

    long long extra = target - shortestDistance;
    for (int i = 0; i < edgesSize && extra > 0; i++) {
        if (edges[i][2] == -1) {
            long long increase = extra > 1000000000 ? 1000000000 : extra;
            edgeList[i].weight += increase;
            edges[i][2] += increase;
            extra -= increase;
        }
    }

    shortestDistance = dijkstraWithHeap(edgeList, edgesSize, n, source, destination, dist);
    free(dist);

    if (shortestDistance != target) {
        free(edgeList);
        *returnSize = 0;
        *returnColumnSizes = NULL;
        return NULL;
    }

    int** result = (int**)malloc(edgesSize * sizeof(int*));
    *returnColumnSizes = (int*)malloc(edgesSize * sizeof(int));
    for (int i = 0; i < edgesSize; i++) {
        result[i] = (int*)malloc(3 * sizeof(int));
        result[i][0] = edgeList[i].from;
        result[i][1] = edgeList[i].to;
        result[i][2] = edgeList[i].weight;
        (*returnColumnSizes)[i] = 3;
    }

    *returnSize = edgesSize;
    free(edgeList);
    return result;
}
