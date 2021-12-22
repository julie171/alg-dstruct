#include "pch.h"
#include "LabD.h"

#define TEST_NUM_OF_VERTICES 999

TEST(FunctionalTest, TheLongestWay_Test1) {
    FILE* f1 = NULL;
    node_t* ptr = NULL;
    int success = 0;
    int* visited = NULL;
    int k = 0;
    int s = 0;
    int t = 0;
    int K = 0;
    fopen_s(&f1, "Test1.txt", "r");
    ASSERT_TRUE(f1 != NULL);
    adjacency_list_t* graph = ReadFromStream(f1, &s, &t, &K);
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
    k = TheLongestWay(graph, queue, s - 1, K, t - 1, &success, visited);
    if (k == 0) {
        AdjacencyListDestroy(graph);
        ASSERT_TRUE(k != 0);
    }
    ptr = queue->head;
    EXPECT_TRUE(ptr->value + 1 == 1);
    ptr = ptr->next;
    EXPECT_TRUE(ptr->value + 1 == 2);
    ptr = ptr->next;
    EXPECT_TRUE(ptr->value + 1 == 3);
    FreeAllQueue(queue);
    FreeVisited(visited);
    AdjacencyListDestroy(graph);
    fclose(f1);
}

TEST(FunctionalTest, TheLongestWay_Test2) {
    FILE* f1 = NULL;
    node_t* ptr = NULL;
    int success = 0;
    int k = 0;
    int s = 0;
    int t = 0;
    int K = 0;
    int* visited = NULL;
    fopen_s(&f1, "Test2.txt", "r");
    ASSERT_TRUE(f1 != NULL);
    adjacency_list_t* graph = ReadFromStream(f1, &s, &t, &K);
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
    k = TheLongestWay(graph, queue, s - 1, K, t - 1, &success, visited);
    if (k == 0) {
        AdjacencyListDestroy(graph);
        ASSERT_TRUE(k != 0);
    }
    EXPECT_TRUE(success == 0);
    FreeVisited(visited);
    FreeAllQueue(queue);
    AdjacencyListDestroy(graph);
    fclose(f1);
}

TEST(StressTest, TheLongestWay_StressTest_GraphGenerating) {
    const int maxDeltaNeighbour = TEST_NUM_OF_VERTICES;
    FILE* fileGraph;
    fopen_s(&fileGraph, "TestFile.txt", "w");
    ASSERT_TRUE(fileGraph != NULL);
    int i = 1;
    int neighbour = 1;
    fprintf(fileGraph, "%d %d %d %d\n", TEST_NUM_OF_VERTICES, 1, (rand() % (maxDeltaNeighbour - 2) + 2), (rand() % 20 + 2));
    for (; i < TEST_NUM_OF_VERTICES - 2; i++) {
        for (neighbour = i + rand() % (maxDeltaNeighbour - i - 2) + 1; neighbour < TEST_NUM_OF_VERTICES - 1; neighbour += rand() % (maxDeltaNeighbour - 1) + 1)
            fprintf(fileGraph, "%d %d\n", i, neighbour);
    }
    fprintf(fileGraph, "%d %d", TEST_NUM_OF_VERTICES - 1, TEST_NUM_OF_VERTICES);
    fclose(fileGraph);
}

TEST(StressTest, TheLongestWay_StressTest) {
    FILE* f1 = NULL;
    node_t* ptr = NULL;
    int success = 0;
    int k = 0;
    int s = 0;
    int t = 0;
    int K = 0;
    int* visited = NULL;
    fopen_s(&f1, "TestFile.txt", "r");
    ASSERT_TRUE(f1 != NULL);
    adjacency_list_t* graph = ReadFromStream(f1, &s, &t, &K);
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
    k = TheLongestWay(graph, queue, s - 1, K, t - 1, &success, visited);
    if (k == 0) {
        AdjacencyListDestroy(graph);
        ASSERT_TRUE(k != 0);
    }
    FreeVisited(visited);
    FreeAllQueue(queue);
    AdjacencyListDestroy(graph);
    fclose(f1);
}