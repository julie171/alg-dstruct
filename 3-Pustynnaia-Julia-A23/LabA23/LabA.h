#pragma once
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct word_t {
	char* word;
	int length;
	struct word_t* next;
} word_t;

word_t* ReadWord(FILE* f);

char ChangeRegister(char letter);

int Alphabet(char* newWord, char* prevWord, int length, int letter);

word_t* CreateList(FILE* f);

void Clearing(word_t* head);

void PrintList(FILE* f);

void Check(FILE* f);

#ifdef __cplusplus
}
#endif