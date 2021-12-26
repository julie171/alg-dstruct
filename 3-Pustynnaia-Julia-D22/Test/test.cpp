#include "pch.h"
#include "LabD.h"

#define TEST_NUM_OF_VERTICES 999

TEST(FunctionalTest, TheLongestWay_Test1) {
    int returnValue = LabSolution("Test1.txt", "Output.txt");
    ASSERT_TRUE(returnValue != 0);
    FILE* fileStream = NULL;
    int check = 0;
    fopen_s(&fileStream, "Output.txt", "r");
    ASSERT_TRUE(fileStream != NULL);
    fscanf_s(fileStream, "%d ", &check);
    EXPECT_TRUE(check == 1);
    fscanf_s(fileStream, "%d ", &check);
    EXPECT_TRUE(check == 2);
    fscanf_s(fileStream, "%d", &check);
    EXPECT_TRUE(check == 3);
    fclose(fileStream);
}

TEST(FunctionalTest, TheLongestWay_Test2) {
    int returnValue = LabSolution("Test2.txt", "Output.txt");
    ASSERT_TRUE(returnValue != 0);
    FILE* fileStream = NULL;
    int check = 0;
    fopen_s(&fileStream, "Output.txt", "r");
    ASSERT_TRUE(fileStream != NULL);
    fscanf_s(fileStream, "%d", &check);
    EXPECT_TRUE(check == 0);
    fclose(fileStream);
}

/*
IDE: Visual Studio 2019 Community Edition
OS: Windows 10 Home, Version 20H2, Build 19042.1348
CPU: AMD Ryzen 5 3500U with Radeon Vega Mobile Gfx 2.10 GHz
RAM: 8,00 GB
STRESSTEST RESULTS:
    GRAPH TRAVERSAL TIME: 20 ms
    USED MEMORY: 100 KB
*/

TEST(StressTest, TheLongestWay_StressTest) {
    FILE* fileStream = NULL;
    node_t* ptr = NULL;
    int success = 0;
    int returnValue = 0;
    int s = 0;
    int t = 0;
    int K = 0;
    int* visited = NULL;
    fopen_s(&fileStream, "TestFile.txt", "r");
    ASSERT_TRUE(fileStream != NULL);
    adjacency_list_t* graph = ReadFromStream(fileStream, &s, &t, &K);
    fclose(fileStream);
    ASSERT_TRUE(graph != NULL);
    visited = VisitedInit(graph);
    if (visited == NULL) {
        AdjacencyListDestroy(graph);
        ASSERT_TRUE(visited != NULL);
    }
    queue_t* queue = QueueInit();
    if (queue == NULL) {
        AdjacencyListDestroy(graph);
        FreeVisited(visited);
        ASSERT_TRUE(queue != NULL);
    }
    returnValue = TheLongestWay(graph, queue, s - 1, K, t - 1, &success, visited);
    if (returnValue == 0) {
        FreeAllQueue(queue);
        FreeVisited(visited);
        AdjacencyListDestroy(graph);
        ASSERT_TRUE(returnValue != 0);
    }
    FreeVisited(visited);
    FreeAllQueue(queue);
    AdjacencyListDestroy(graph);
}