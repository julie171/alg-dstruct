#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>  
#include "LabD.h"


void AdjacencyListDestroy(adjacency_list_t* adjacencyList) {
    int vertex = 0;
    for (vertex = 0; vertex < adjacencyList->quantOfVertices; vertex++)
        if (adjacencyList->arrayOfVert[vertex].neighbour != NULL)
            free(adjacencyList->arrayOfVert[vertex].neighbour);
    free(adjacencyList->arrayOfVert);
    free(adjacencyList);
}

adjacency_list_t* AdjancencyListInit(int quantOfVertices) {
    adjacency_list_t* adjacencyList = NULL;
    int vertex;
    adjacencyList = (adjacency_list_t*)malloc(sizeof(adjacency_list_t));
    if (adjacencyList == NULL)
        return NULL;
    adjacencyList->quantOfVertices = quantOfVertices;
    adjacencyList->arrayOfVert = (array_of_neighbours_t*)malloc(quantOfVertices * sizeof(array_of_neighbours_t));
    if (adjacencyList->arrayOfVert == NULL) {
        free(adjacencyList);
        return NULL;
    }
    for (vertex = 0; vertex < quantOfVertices; vertex++) {
        adjacencyList->arrayOfVert[vertex].neighbour = NULL;
        adjacencyList->arrayOfVert[vertex].quantOfNeighb = 0;
    }
    return adjacencyList;
}

adjacency_list_t* ReadFromStream(FILE* stream, int* s, int* t, int* K) {
    int quantOfVertices = 0;
    char check = 0;
    int returnValue = 0;
    int currentVertex = 0;
    int currentNeighbour = 0;
    int* tempStorage = NULL;
    int lastNeighbourId = 0;
    adjacency_list_t* adjacencyList = NULL;
    returnValue = fscanf_s(stream, "%d %d %d %d", &quantOfVertices, s, t, K);
    if (returnValue == -1)
        return NULL;
    adjacencyList = AdjancencyListInit(quantOfVertices);
    if (adjacencyList == NULL)
        return NULL;
    returnValue = fscanf_s(stream, "%c", &check, sizeof(char));
    while (returnValue != -1) {
        fscanf_s(stream, "%d", &currentVertex);
        returnValue = fscanf_s(stream, "%c", &check, sizeof(char));
        while (check != '\n' && returnValue != -1) {
            fscanf_s(stream, "%d", &currentNeighbour);
            returnValue = fscanf_s(stream, "%c", &check, sizeof(char));
            tempStorage = (int*)realloc(adjacencyList->arrayOfVert[currentVertex - 1].neighbour,
                ++adjacencyList->arrayOfVert[currentVertex - 1].quantOfNeighb * sizeof(int));
            if (tempStorage == NULL) {
                AdjacencyListDestroy(adjacencyList);
                return NULL;
            }
            adjacencyList->arrayOfVert[currentVertex - 1].neighbour = tempStorage;
            lastNeighbourId = adjacencyList->arrayOfVert[currentVertex - 1].quantOfNeighb - 1;
            adjacencyList->arrayOfVert[currentVertex - 1].neighbour[lastNeighbourId] = currentNeighbour - 1;
            tempStorage = (int*)realloc(adjacencyList->arrayOfVert[currentNeighbour - 1].neighbour,
                ++adjacencyList->arrayOfVert[currentNeighbour - 1].quantOfNeighb * sizeof(int));
            if (tempStorage == NULL) {
                AdjacencyListDestroy(adjacencyList);
                return NULL;
            }
            adjacencyList->arrayOfVert[currentNeighbour - 1].neighbour = tempStorage;
            lastNeighbourId = adjacencyList->arrayOfVert[currentNeighbour - 1].quantOfNeighb - 1;
            adjacencyList->arrayOfVert[currentNeighbour - 1].neighbour[lastNeighbourId] = currentVertex - 1;
        }
    }
    return adjacencyList;
}

queue_t* QueueInit() {
    queue_t* queue = (queue_t*)malloc(sizeof(queue_t));
    if (queue == NULL)
        return NULL;
    queue->head = NULL;
    queue->tail = NULL;
    queue->length = 0;
    return queue;
}

void FreeAllQueue(queue_t* queue) {
    node_t* iter = queue->head;
    while (iter != NULL) {
        queue->head = queue->head->next;
        free(iter);
        iter = queue->head;
    }
    free(queue);
}

void FreeWrongWay(queue_t* queue, int vertex, int* visited) {
    node_t* ptr = queue->head;
    if (ptr->next != NULL) {
        while (ptr->next != queue->tail)
            ptr = ptr->next;
        queue->tail = ptr;
        ptr = ptr->next;
        queue->tail->next = NULL;
        queue->length = queue->length - 1;
    }
    else {
        queue->tail = NULL;
        queue->head = NULL;
    }
    free(ptr);
}

void FreeVisited(int* visited) {
    free(visited);
}

int TheLongestWay(adjacency_list_t* graph, queue_t* queue, int vertex, int K, int t, int* success, int* visited) {
    int neighborId = 0;
    int neighbValue = 0;
    int* neighbList = graph->arrayOfVert[vertex].neighbour;
    queue_t* tmp = NULL;
    visited[vertex] = 1;
    if (queue->head == NULL) {
        queue->head = (node_t*)malloc(sizeof(node_t));
        if (queue->head == NULL) {
            FreeAllQueue(queue);
            FreeVisited(visited);
            return 0;
        }
        queue->head->value = vertex;
        queue->head->next = NULL;
        queue->tail = queue->head;
        queue->length = 1;
    }
    else {
        queue->tail->next = (node_t*)malloc(sizeof(node_t));
        if (queue->tail->next == NULL) {
            FreeAllQueue(queue);
            FreeVisited(visited);
            return 0;
        }
        queue->length += 1;
        queue->tail->next->value = vertex;
        queue->tail = queue->tail->next;
        queue->tail->next = NULL;
    }
    for (; neighborId < graph->arrayOfVert[vertex].quantOfNeighb; neighborId++) {
        neighbValue = neighbList[neighborId];
        if (visited[neighbValue] == 0) {
            if (neighbValue == t) {
                visited[neighbValue] = 1;
                queue->tail->next = (node_t*)malloc(sizeof(node_t));
                if (queue->tail->next == NULL) {
                    FreeAllQueue(queue);
                    FreeVisited(visited);
                    return 0;
                }
                queue->length++;
                queue->tail->next->value = neighbValue;
                queue->tail = queue->tail->next;
                queue->tail->next = NULL;
                if (queue->length - 1 >= K) {
                    (*success) = 1;
                    return 1;
                }
            }
            else {
                if (TheLongestWay(graph, queue, neighbValue, K, t, success, visited) == 0) {
                    FreeAllQueue(queue);
                    FreeVisited(visited);
                    return 0;
                }
                if ((*success) == 1)
                    return 1;
            }
            FreeWrongWay(queue, neighbList[neighborId], visited);
        }
    }
    return 1;
}

void WriteQueue(FILE* stream, queue_t* queue, int success) {
    node_t* ptr = NULL;
    if (success == 0)
        fprintf(stream, "%d", 0);
    else {
        ptr = queue->head;
        while (ptr != NULL) {
            fprintf(stream, "%d ", ptr->value + 1);
            ptr = ptr->next;
        }
        FreeAllQueue(queue);
    }
}

int* VisitedInit(adjacency_list_t* graph) {
    int* visited = (int*)calloc(graph->quantOfVertices, sizeof(int));
    return visited;
}

int LabSolution(FILE* input, FILE* output) {
    int s = 0;
    int t = 0;
    int K = 0;
    int* visited = NULL;
    int success = 0;
    adjacency_list_t* graph = ReadFromStream(input, &s, &t, &K);
    if (graph == NULL)
        return 0;
    queue_t* queue = QueueInit();
    if (queue == NULL) {
        AdjacencyListDestroy(graph);
        return 0;
    }
    visited = VisitedInit(graph);
    if (visited == NULL) {
        FreeAllQueue(queue);
        return 0;
    }
    if (TheLongestWay(graph, queue, s - 1, K, t - 1, &success, visited) == 0) {
        AdjacencyListDestroy(graph);
        return 0;
    }
    FreeVisited(visited);
    WriteQueue(output, queue, success);
    AdjacencyListDestroy(graph);
    FreeAllQueue(queue);
    return 1;
}