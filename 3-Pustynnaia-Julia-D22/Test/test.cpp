#include "pch.h"
#include "LabD.h"

#define TEST_NUM_OF_VERTICES 9000

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
    GRAPH TRAVERSAL TIME: 120 ms
    USED MEMORY: 2 ÌB
*/

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
    int returnValue = LabSolution("TestFile.txt", "Output.txt");
    ASSERT_TRUE(returnValue != 0);
}