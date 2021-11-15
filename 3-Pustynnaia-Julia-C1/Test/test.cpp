#include "pch.h"
#include "LabC1.h"

/*
IDE: Visual Studio 2019 Community Edition
OS: Windows 10 Home, Version 20H2, Build 19042.1348
CPU: AMD Ryzen 5 3500U with Radeon Vega Mobile Gfx 2.10 GHz
RAM: 8,00 GB

STRESSTEST RESULTS:
    GRAPH TRAVERSAL TIME: 32.692 sec
    USED MEMORY: 1.1 GB 
*/

#define TEST_NUM_OF_VERTICES 550000

TEST(LabSolution_StressTest, LabSolution_GraphGenerating) {
    const int maxDeltaNeighbour = TEST_NUM_OF_VERTICES;
    FILE* fileGraph = fopen("TestFile.txt", "w");
    ASSERT_TRUE(fileGraph);
    int i = 0;
    int neighbour = 0;
    fprintf(fileGraph, "%d\n", TEST_NUM_OF_VERTICES);
    for (; i < TEST_NUM_OF_VERTICES - 1; i++) {
        fprintf(fileGraph, "%d ", i);
        for (neighbour = i + rand() % (maxDeltaNeighbour - 1) + 1; neighbour < TEST_NUM_OF_VERTICES; neighbour += rand() % (maxDeltaNeighbour - 1) + 1)
            fprintf(fileGraph, "%d ", neighbour);
        fprintf(fileGraph, "\n");
    }
    fprintf(fileGraph, "%d\n", TEST_NUM_OF_VERTICES - 1);
    fclose(fileGraph);
}

TEST(LabSolution_StressTest, LabSolution_FillingInFile) {
    int depthFirstTraversalResult;
    FILE* f = fopen("TestFile.txt", "r");
    ASSERT_TRUE(f);
    adjacency_list_t* graph = ReadAdjacencyList(f);
    fclose(f);
    ASSERT_TRUE(graph);
    FILE* fileOutput = fopen("Record.txt", "w");
    if (fileOutput == NULL) {
        AdjacencyListDestroy(graph);
        ASSERT_TRUE(fileOutput);
    }
    depthFirstTraversalResult = DepthFirstSearch(graph, 0, fileOutput);
    if (depthFirstTraversalResult == 0) {
        fclose(fileOutput);
        AdjacencyListDestroy(graph);
        ASSERT_TRUE(depthFirstTraversalResult);
    }
    fclose(fileOutput);
    AdjacencyListDestroy(graph);
}
