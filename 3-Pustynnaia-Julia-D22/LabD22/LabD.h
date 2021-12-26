#pragma once

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct {
        int* neighbour;
        int quantOfNeighb;
    } array_of_neighbours_t;

    typedef struct {
        array_of_neighbours_t* arrayOfVert;
        int quantOfVertices;
    } adjacency_list_t;

    typedef struct node_t {
        int value;
        struct node_t* next;
    } node_t;

    typedef struct {
        int length;
        node_t* head;
        node_t* tail;
    } queue_t;

    void AdjacencyListDestroy(adjacency_list_t* adjacencyList);

    adjacency_list_t* AdjancencyListInit(int quantOfVertices);

    adjacency_list_t* ReadFromStream(FILE* stream, int* s, int* t, int* K);

    queue_t* QueueInit();

    void FreeAllQueue(queue_t* queue);

    void FreeWrongWay(queue_t* queue, int vertex, int* visited);

    void FreeVisited(int* visited);

    int TheLongestWay(adjacency_list_t* graph, queue_t* queue, int vertex, int K, int t, int* success, int* visited);

    void WriteQueue(FILE* stream, queue_t* queue, int success);

    int* VisitedInit(adjacency_list_t* graph);

    int LabSolution(const char* input, const char* output);

#ifdef __cplusplus
}
#endif