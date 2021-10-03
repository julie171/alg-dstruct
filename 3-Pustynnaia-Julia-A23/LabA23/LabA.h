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

char* ReadWord(FILE* f, int* returnValue, int* letter);

word_t* CreateWord(FILE* f);

char ChangeRegister(char letter);

int Alphabet(char* newWord, char* prevWord, int length, int letter);

word_t* CreateList(const char* name);

void Clearing(word_t* head);

void Clearing1Word(word_t* word);

void PrintList(const char* name);

void Check(const char* name);

#ifdef __cplusplus
}
#endif