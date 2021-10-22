#include <stdio.h>
#include "memallocator.h"

void* g_head = NULL;
void* s_beginningOfHeap = NULL;
void* g_nextSuitable = NULL;
int s_size = 0;

int memgetminimumsize() {
    return sizeof(int) + sizeof(void*) + sizeof(void*) + sizeof(int);
}

int memgetblocksize() {
    return sizeof(int) + sizeof(void*) + sizeof(void*) + sizeof(int);
}

int* LeftSizeOfBlock(void* block) {
    return (int*)block;
}

void** NextBlock(void* block) {
    return (void**)((char*)block + sizeof(int));
}

void** PrevBlock(void* block) {
    return (void**)((char*)block + sizeof(int) + sizeof(void*));
}

int* RightSizeOfBlock(void* block) {
    return (int*)((char*)block + *LeftSizeOfBlock(block) - sizeof(int));
}

int meminit(void* pMemory, int size) {
    if (size <= memgetminimumsize())
        return 0;
    void* block = pMemory;
    *LeftSizeOfBlock(block) = size;
    *NextBlock(block) = NULL;
    *PrevBlock(block) = NULL;
    *RightSizeOfBlock(block) = size;
    g_head = block;
    s_beginningOfHeap = pMemory;
    s_size = size;
    g_nextSuitable = g_head;
    return size;
}

void memdone() {
    void* block = s_beginningOfHeap;
    while ((char*)block < (char*)s_beginningOfHeap + s_size) {
        if (*RightSizeOfBlock(block) < 0)
            fprintf(stderr, "[MEMORY LEAK] block at 0x%p\n", (char*)block + sizeof(int) + sizeof(void*) + sizeof(void*));
        block = (void*)((char*)block + *LeftSizeOfBlock(block));
    }
}

void* memalloc(int size) {
    if (size < 1 || size > s_size - memgetblocksize())
        return NULL;
    void* suitBlock = g_nextSuitable;
    int found = 0;
    void* freeBlock = NULL;
    void* prev = NULL;
    while (found != 1 && suitBlock != NULL) {
        if (*LeftSizeOfBlock(suitBlock) >= memgetblocksize() + size) {
            found = 1;
        }
        prev = suitBlock;
        suitBlock = *NextBlock(suitBlock);
        if (found == 0 && g_nextSuitable != g_head && suitBlock == NULL)
            suitBlock = g_head;
        if (suitBlock == g_nextSuitable)
            break;
    }
    if (found == 0) {
        return NULL;
    }
    suitBlock = prev;
    if (*LeftSizeOfBlock(suitBlock) > memgetblocksize() + size + memgetblocksize()) {
        freeBlock = (void*)((char*)suitBlock + memgetblocksize() + size);
        *LeftSizeOfBlock(freeBlock) = *LeftSizeOfBlock(suitBlock) - memgetblocksize() - size;
        *NextBlock(freeBlock) = *NextBlock(suitBlock);
        *PrevBlock(freeBlock) = *PrevBlock(suitBlock);
        *RightSizeOfBlock(freeBlock) = *LeftSizeOfBlock(freeBlock);
        if (*PrevBlock(suitBlock) != NULL)
            *NextBlock(*PrevBlock(suitBlock)) = freeBlock;
        else
            g_head = freeBlock;
        if (*NextBlock(suitBlock) != NULL)
            *PrevBlock(*NextBlock(suitBlock)) = freeBlock;
        *LeftSizeOfBlock(suitBlock) = memgetblocksize() + size;
    }
    else {
        if (*PrevBlock(suitBlock) != NULL) {
            *NextBlock(*PrevBlock(suitBlock)) = *NextBlock(suitBlock);
            if (*NextBlock(suitBlock) != NULL)
                *PrevBlock(*NextBlock(suitBlock)) = *PrevBlock(suitBlock);
        }
        else {
            g_head = *NextBlock(suitBlock);
            if (g_head != NULL)
                *PrevBlock(g_head) = NULL;
        }
    }
    if (*NextBlock(suitBlock) != NULL)
        g_nextSuitable = *NextBlock(suitBlock);
    else
        g_nextSuitable = g_head;
    *RightSizeOfBlock(suitBlock) = -*LeftSizeOfBlock(suitBlock);
    *NextBlock(suitBlock) = NULL;
    *PrevBlock(suitBlock) = NULL;
    return (void*)((char*)suitBlock + sizeof(int) + sizeof(void*) + sizeof(void*));
}

// Free memory previously allocated by memalloc
void memfree(void* p) {
    void* futureFree = (void*)((char*)p - sizeof(void*) - sizeof(int) - sizeof(void*));
    void* leftBlock = NULL;
    void* rightBlock = NULL;
    int mergedWithLeft = 0;
    int mergedWithRight = 0;
    if ((char*)futureFree - 1 > (char*)s_beginningOfHeap)
        leftBlock = (void*)((char*)futureFree - (*((int*)futureFree - 1) >= 0 ? *((int*)futureFree - 1) : -*((int*)futureFree - 1)));
    if ((char*)futureFree + *LeftSizeOfBlock(futureFree) + 1 < (char*)s_beginningOfHeap + s_size)
        rightBlock = (void*)(RightSizeOfBlock(futureFree) + 1);
    *RightSizeOfBlock(futureFree) = -*RightSizeOfBlock(futureFree);
    if (leftBlock != NULL && *RightSizeOfBlock(leftBlock) > 0) {
        mergedWithLeft = 1;
        *LeftSizeOfBlock(leftBlock) += *LeftSizeOfBlock(futureFree);
        *RightSizeOfBlock(leftBlock) = *LeftSizeOfBlock(leftBlock);
        futureFree = leftBlock;
    }
    if (rightBlock != NULL && *RightSizeOfBlock(rightBlock) > 0) {
        mergedWithRight = 1;
        if (mergedWithLeft == 0) {
            *NextBlock(futureFree) = g_head;
            *PrevBlock(g_head) = futureFree;
            *PrevBlock(futureFree) = NULL;
            g_head = futureFree;
        }
        if (rightBlock == g_head) {
            g_head = *NextBlock(rightBlock);
            *PrevBlock(g_head) = NULL;
        }
        else {
            *NextBlock(*PrevBlock(rightBlock)) = *NextBlock(rightBlock);
            if (*NextBlock(rightBlock) != NULL)
                *PrevBlock(*NextBlock(rightBlock)) = *PrevBlock(rightBlock);
        }
        *LeftSizeOfBlock(futureFree) += *LeftSizeOfBlock(rightBlock);
        *RightSizeOfBlock(futureFree) = *LeftSizeOfBlock(futureFree);
        if (g_nextSuitable == rightBlock)
            g_nextSuitable = futureFree;
    }
    if (mergedWithLeft == 0 && mergedWithRight == 0) {
        *NextBlock(futureFree) = g_head;
        if (g_head != NULL)
            *PrevBlock(g_head) = futureFree;
        else
            g_nextSuitable = futureFree;
        *PrevBlock(futureFree) = NULL;
        g_head = futureFree;
    }
}
