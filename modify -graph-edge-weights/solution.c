#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

#define INF LLONG_MAX


typedef struct Node {
    int vertex;
    long long weight;
    struct Node* next;
} Node;


Node* createNode(int vertex, long long weight) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->vertex = vertex;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}

void freeGraph(Node** graph, int n) {
    for (int i = 0; i < n; ++i) {
        Node* current = graph[i];
        while (current) {
            Node* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(graph);
}


long long dijkstra(Node** graph, int n, int src, int dest) {
    long long* distance = (long long*)malloc(n * sizeof(long long));
    bool* visited = (bool*)malloc(n * sizeof(bool));
    
    for (int i = 0; i < n; ++i) {
        distance[i] = INF;
        visited[i] = false;
    }

    distance[src] = 0;

    for (int i = 0; i < n; ++i) {
        int u = -1;
        for (int j = 0; j < n; ++j) {
            if (!visited[j] && (u == -1 || distance[j] < distance[u])) {
                u = j;
            }
        }

        if (distance[u] == INF) break;

        visited[u] = true;
        Node* current = graph[u];
        while (current) {
            int v = current->vertex;
            long long weight = current->weight;
            if (distance[u] != INF && distance[u] + weight < distance[v]) {
                distance[v] = distance[u] + weight;
            }
            current = current->next;
        }
    }

    long long result = distance[dest];
    free(distance);
    free(visited);

    return result;
}


int** modifiedGraphEdges(int n, int** edges, int edgesSize, int* edgesColSize, int source, int destination, int target, int* returnSize, int** returnColumnSizes) {
    Node** graph = (Node**)malloc(n * sizeof(Node*));
    for (int i = 0; i < n; ++i) {
        graph[i] = NULL;
    }

 
    for (int i = 0; i < edgesSize; ++i) {
        int from = edges[i][0];
        int to = edges[i][1];
        int weight = edges[i][2];
        Node* newNode1 = createNode(to, weight);
        newNode1->next = graph[from];
        graph[from] = newNode1;

        Node* newNode2 = createNode(from, weight);
        newNode2->next = graph[to];
        graph[to] = newNode2;
    }

   
    long long shortestDistance = dijkstra(graph, n, source, destination);
    if (shortestDistance == target) {
       
        *returnSize = edgesSize;
        *returnColumnSizes = (int*)malloc(edgesSize * sizeof(int));
        return edges;
    }

  
    for (int i = 0; i < edgesSize; ++i) {
        if (edges[i][2] == -1) {
           
            int low = 1, high = 1000000000;  
            int bestWeight = -1;

            while (low <= high) {
                int mid = (low + high) / 2;
                edges[i][2] = mid;

               
                Node** newGraph = (Node**)malloc(n * sizeof(Node*));
                for (int j = 0; j < n; ++j) newGraph[j] = NULL;

                for (int j = 0; j < edgesSize; ++j) {
                    int from = edges[j][0], to = edges[j][1], weight = edges[j][2];
                    Node* newNode1 = createNode(to, weight);
                    newNode1->next = newGraph[from];
                    newGraph[from] = newNode1;

                    Node* newNode2 = createNode(from, weight);
                    newNode2->next = newGraph[to];
                    newGraph[to] = newNode2;
                }

              
                shortestDistance = dijkstra(newGraph, n, source, destination);
                if (shortestDistance < target) {
                    low = mid + 1;
                } else {
                    bestWeight = mid;
                    high = mid - 1;
                }

                freeGraph(newGraph, n);
            }

            if (bestWeight != -1) {
                edges[i][2] = bestWeight;
            }
        }
    }

   
    Node** finalGraph = (Node**)malloc(n * sizeof(Node*));
    for (int j = 0; j < n; ++j) finalGraph[j] = NULL;

    for (int j = 0; j < edgesSize; ++j) {
        int from = edges[j][0], to = edges[j][1], weight = edges[j][2];
        Node* newNode1 = createNode(to, weight);
        newNode1->next = finalGraph[from];
        finalGraph[from] = newNode1;

        Node* newNode2 = createNode(from, weight);
        newNode2->next = finalGraph[to];
        finalGraph[to] = newNode2;
    }

    shortestDistance = dijkstra(finalGraph, n, source, destination);
    freeGraph(finalGraph, n);

    if (shortestDistance == target) {
        *returnSize = edgesSize;
        *returnColumnSizes = (int*)malloc(edgesSize * sizeof(int));
        return edges;
    } else {
      
        *returnSize = 0;
        *returnColumnSizes = NULL;
        return NULL;
    }
}
