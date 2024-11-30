#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define INF 2000000000 // ----"infinito"----


long long dijkstra(int** edges, int edgesSize, int n, int src, int dest) {
    long long** graph = (long long**)malloc(n * sizeof(long long*));
    long long* distance = (long long*)malloc(n * sizeof(long long));
    int* visited = (int*)malloc(n * sizeof(int));

    for (int i = 0; i < n; ++i) {
        graph[i] = (long long*)malloc(n * sizeof(long long));
        for (int j = 0; j < n; ++j) {
            graph[i][j] = INF;
        }
        distance[i] = INF;
        visited[i] = 0;
    }
    distance[src] = 0;

    // ---insere grafo----
    for (int i = 0; i < edgesSize; ++i) {
        int from = edges[i][0], to = edges[i][1], weight = edges[i][2];
        if (weight == -1) continue; 
        graph[from][to] = weight;
        graph[to][from] = weight;
    }

    // ----Dijkstra força bruta----
    for (int i = 0; i < n; ++i) {
        int closestUnvisitedNode = -1;
        for (int j = 0; j < n; ++j) {
            if (!visited[j] && (closestUnvisitedNode == -1 || distance[j] < distance[closestUnvisitedNode])) {
                closestUnvisitedNode = j;
            }
        }

        visited[closestUnvisitedNode] = 1;

        for (int j = 0; j < n; ++j) {
            if (graph[closestUnvisitedNode][j] != INF) {
                distance[j] = distance[j] < distance[closestUnvisitedNode] + graph[closestUnvisitedNode][j]
                                  ? distance[j]
                                  : distance[closestUnvisitedNode] + graph[closestUnvisitedNode][j];
            }
        }
    }

    long long result = distance[dest];

    
    for (int i = 0; i < n; ++i) {
        free(graph[i]);
    }
    free(graph);
    free(distance);
    free(visited);

    return result;
}


int** modifiedGraphEdges(int n, int** edges, int edgesSize, int* edgesColSize, int source, int destination, int target, int* returnSize, int** returnColumnSizes) {
    long long shortestDistance = dijkstra(edges, edgesSize, n, source, destination);

    if (shortestDistance < target) { 
        *returnSize = 0;
        *returnColumnSizes = NULL;
        return NULL;
    }

    int isEqualToTarget = (shortestDistance == target);

    for (int i = 0; i < edgesSize; ++i) {
        if (edges[i][2] > 0) continue; 

        if (isEqualToTarget) { 
            edges[i][2] = INF;
            continue;
        }

        
        edges[i][2] = 1;
        shortestDistance = dijkstra(edges, edgesSize, n, source, destination);

        if (shortestDistance <= target) { 
            isEqualToTarget = 1;
            edges[i][2] += (target - shortestDistance);
        }
    }

    if (!isEqualToTarget) {
        *returnSize = 0;
        *returnColumnSizes = NULL;
        return NULL;
    }

    int** result = (int**)malloc(edgesSize * sizeof(int*));
    *returnColumnSizes = (int*)malloc(edgesSize * sizeof(int));
    for (int i = 0; i < edgesSize; ++i) {
        result[i] = (int*)malloc(3 * sizeof(int));
        (*returnColumnSizes)[i] = 3;
        result[i][0] = edges[i][0];
        result[i][1] = edges[i][1];
        result[i][2] = edges[i][2];
    }

    *returnSize = edgesSize;
    return result;
}
