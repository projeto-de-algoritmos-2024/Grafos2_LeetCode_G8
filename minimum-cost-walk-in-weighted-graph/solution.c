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

