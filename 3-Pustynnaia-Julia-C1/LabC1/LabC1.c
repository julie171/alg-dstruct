#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>  
#include "LabC1.h"

#define MAX_INT_LENGTH 10

int LengthOfNumber(int num) {
    int res = 0;
    while (num) {
        res++;
        num /= 10;
    }
    return res;
}

void AdjacencyListDestroy(adjacency_list_t* adjacencyList) {
    int i = 0;
    for (i = 0; i < adjacencyList->quantOfVertices; i++) 
        if (adjacencyList->arrayOfVert[i].neighbour != NULL)
            free(adjacencyList->arrayOfVert[i].neighbour);
    free(adjacencyList->arrayOfVert);
    free(adjacencyList);
}

stack_t* StackPush(stack_t* stack, int vertex) {
    node_t* newNode = (node_t*)malloc(sizeof(node_t));
    if (newNode == NULL)
        return NULL;
    newNode->vertex = vertex;
    newNode->next = stack->top;
    stack->top = newNode;
    return stack;
}

int StackIsEmpty(stack_t* stack) {
    if (stack->top != NULL)
        return 0;
    else
        return -1;
}

int StackPop(stack_t* stack) {
    node_t* nodeToFree = NULL;
    int vertex = 0;
    if (StackIsEmpty(stack) == 0) {
        nodeToFree = stack->top;
        vertex = nodeToFree->vertex;
        stack->top = nodeToFree->next;
        free(nodeToFree);
        return vertex;
    }
    return -1;
}

void StackDestroy(stack_t* stack) {
    node_t* iter = stack->top;
    while (iter != NULL) {
        node_t* nodeToFree = iter;
        iter = iter->next;
        free(nodeToFree);
    }
    free(stack);
}

int StackPushAllNeighboursOfVertex(stack_t* stack, adjacency_list_t* graph, int vertex) {
    int i = graph->arrayOfVert[vertex].quantOfNeighb - 1;
    int neighbour = 0;
    for (; i >= 0; i--) {
        neighbour = graph->arrayOfVert[vertex].neighbour[i];
        if (StackPush(stack, neighbour) == NULL) 
            return 0;
    }
    return 1;
}

stack_t* StackInit() {
    stack_t* stack = (stack_t*)malloc(sizeof(stack_t));
    if (stack == NULL)
        return NULL;
    stack->top = NULL;
    return stack;
}

int DepthFirstSearch(adjacency_list_t* graph, int vertex, FILE* stream) {
    int* visited = NULL;
    int currentVertex = 0;
    stack_t* stack = StackInit();
    if (stack == NULL)
        return 0;
    visited = (int*)calloc(graph->quantOfVertices, sizeof(int));
    if (visited == NULL) {
        StackDestroy(stack);
        return 0;
    }
    visited[vertex] = 1;
    fprintf(stream, "%d ", vertex);
    if (StackPushAllNeighboursOfVertex(stack, graph, vertex) == 0) {
        StackDestroy(stack);
        return 0;
    }
    while (StackIsEmpty(stack) == 0) {
        currentVertex = StackPop(stack);
        if (visited[currentVertex] == 0) {
            visited[currentVertex] = 1;
            fprintf(stream, "%d ", currentVertex);
            if (StackPushAllNeighboursOfVertex(stack, graph, currentVertex) == 0) {
                StackDestroy(stack);
                return 0;
            }
        }
    }
    free(visited);
    StackDestroy(stack);
    return 1;
}

adjacency_list_t* ReadAdjacencyList(FILE* stream) {
    int quantOfVertices = 0;
    int i = 0;
    int currentVertex = 0;
    int currentNeighbour = 0;
    int* tempStorage = NULL;
    int lastNeighbourId = 0;
    int maxStrLength = 0;
    char* stringStorage = NULL;
    char* string = NULL;
    adjacency_list_t* adjacencyList = NULL;
    char* storVerticesQuant = (char*)malloc(MAX_INT_LENGTH * sizeof(char));
    if (storVerticesQuant == NULL)
        return NULL;
    fgets(storVerticesQuant, MAX_INT_LENGTH, stream);
    quantOfVertices = atoi(storVerticesQuant);
    free(storVerticesQuant);
    adjacencyList = (adjacency_list_t*)malloc(sizeof(adjacency_list_t));
    if (adjacencyList == NULL)
        return NULL;
    adjacencyList->quantOfVertices = quantOfVertices;
    adjacencyList->arrayOfVert = (array_of_neighbours_t*)malloc(quantOfVertices * sizeof(array_of_neighbours_t));
    if (adjacencyList->arrayOfVert == NULL) {
        free(adjacencyList);
        return NULL;
    }
    for (i = 0; i < quantOfVertices; i++) {
        adjacencyList->arrayOfVert[i].neighbour = NULL;
        adjacencyList->arrayOfVert[i].quantOfNeighb = 0;
    }
    maxStrLength = (LengthOfNumber(quantOfVertices - 1) + 1) * quantOfVertices + 1;
    stringStorage = (char*)malloc(maxStrLength * sizeof(char));
    if (stringStorage == NULL) {
        AdjacencyListDestroy(adjacencyList);
        return NULL;
    }
    for (i = 0; i < quantOfVertices; i++) {
        fgets(stringStorage, maxStrLength, stream);
        string = stringStorage;
        sscanf_s(string, "%d", &currentVertex);
        while (isdigit(*string) != 0)
            string++;
        string++;
        while (sscanf_s(string, "%d", &currentNeighbour) > 0) {
            while (isdigit(*string) != 0)
                string++;
            string++;
            tempStorage = (int*)realloc(adjacencyList->arrayOfVert[currentVertex].neighbour,
                ++adjacencyList->arrayOfVert[currentVertex].quantOfNeighb * sizeof(int));
            if (tempStorage == NULL) {
                free(stringStorage);
                AdjacencyListDestroy(adjacencyList);
                return NULL;
            }
            adjacencyList->arrayOfVert[currentVertex].neighbour = tempStorage;
            lastNeighbourId = adjacencyList->arrayOfVert[currentVertex].quantOfNeighb - 1;
            adjacencyList->arrayOfVert[currentVertex].neighbour[lastNeighbourId] = currentNeighbour;
            tempStorage = (int*)realloc(adjacencyList->arrayOfVert[currentNeighbour].neighbour, ++adjacencyList->arrayOfVert[currentNeighbour].quantOfNeighb * sizeof(int));
            if (tempStorage == NULL) {
                free(stringStorage);
                AdjacencyListDestroy(adjacencyList);
                return NULL;
            }
            adjacencyList->arrayOfVert[currentNeighbour].neighbour = tempStorage;
            lastNeighbourId = adjacencyList->arrayOfVert[currentNeighbour].quantOfNeighb - 1;
            adjacencyList->arrayOfVert[currentNeighbour].neighbour[lastNeighbourId] = currentVertex;
        }
    }
    free(stringStorage);
    return adjacencyList;
}

int LabSolution() {
    adjacency_list_t* graph = ReadAdjacencyList(stdin);
    if (graph == NULL)
        return -1;
    if (DepthFirstSearch(graph, 0, stdout) == 0) {
        AdjacencyListDestroy(graph);
        return -1;
    }
    AdjacencyListDestroy(graph);
    return 0;
}

