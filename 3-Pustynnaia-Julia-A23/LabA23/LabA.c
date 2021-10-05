#include "LabA.h"
#include <stdlib.h>
#include <stdio.h>

char* ReadWord(FILE* f, int* returnValue, int* letter) {
	(*letter) = 0;
	(*returnValue) = 0;
	char* writtenWord = NULL;
	char* tempStorage;
	writtenWord = (char*)malloc(sizeof(char));
	if (writtenWord == NULL)
		return NULL;
	(*returnValue) = fscanf_s(f, "%c", &writtenWord[0], sizeof(char));
	while ((writtenWord[(*letter)] >= 'A' && writtenWord[(*letter)] <= 'Z') || (writtenWord[(*letter)] >= 'a' && writtenWord[(*letter)] <= 'z')) {
		(*letter)++;
		tempStorage = (char*)realloc(writtenWord, (1 + (*letter)) * sizeof(char));
		if (tempStorage == NULL) {
			return NULL;
		}
		writtenWord = tempStorage;
		(*returnValue) = fscanf_s(f, "%c", &writtenWord[(*letter)], sizeof(char));
	}
	writtenWord[(*letter)] = '\0';
	return writtenWord;
}

word_t* CreateWord(FILE* f) {
	int length;
	int returnValue;
	word_t* word = (word_t*)malloc(sizeof(word_t));
	if (word == NULL)
		return NULL;
	word->next = NULL;
	char* writtenWord = ReadWord(f, &returnValue, &length);
	if (writtenWord == NULL) {
		Clearing1Word(word);
		return NULL;
	}
	while (length == 0 && returnValue != -1) {
		free(writtenWord);
		writtenWord = ReadWord(f, &returnValue, &length);
		if (writtenWord == NULL) {
			Clearing1Word(word);
			return NULL;
		}
	}
	word->word = writtenWord;
	word->length = length;
	if (length > 0 && returnValue == -1)
		word->length = -1;
	else if (length == 0 && returnValue == -1)
		word->length = -2;
	return word;
}

char ChangeRegister(char letter) {
	if (letter >= 'A' && letter <= 'Z')
		letter = letter + ('a' - 'A');
	return letter;
}

int Alphabet(char* newWord, char* prevWord, int length, int letter) {
	char newLetter;
	char prevLetter;
	newLetter = ChangeRegister(newWord[letter]);
	prevLetter = ChangeRegister(prevWord[letter]);
	while (newLetter == prevLetter) {
		letter++;
		newLetter = ChangeRegister(newWord[letter]);
		prevLetter = ChangeRegister(prevWord[letter]);
		if (letter == length)
			return -1;
	}
	if (prevLetter > newLetter)
		return -1;
	else
		return letter;
}

word_t* CreateList(const char* name) {
	word_t* new;
	word_t* word;
	word_t* prev;
	word_t* head;
	int letter = 0;
	int length;
	int finish = 0;
	errno_t mistake;
	FILE* f;
	mistake = fopen_s(&f, name, "r");
	if (mistake != 0)
		return NULL;
	head = CreateWord(f);
	if (head == NULL)
		return NULL;
	if (head->length == -2) {
		Clearing1Word(head);
		return NULL;
	}
	if (head->length == -1) {
		while (head->word[letter] != '\0')
			letter++;
		head->length = letter;
		return head;
	}
	while (finish != 1) {
		prev = head;
		new = CreateWord(f);
		if (new == NULL) {
			Clearing1Word(head);
			return NULL;
		}
		if (new->length == -2) {
			Clearing1Word(new);
			break;
		}
		if (new->length == -1) {
			while (new->word[letter] != '\0')
				letter++;
			new->length = letter;
			finish = 1;
		}
		length = new->length;
		word = head;
		while (word != NULL) {
			if (length < word->length) {
				new->next = word;
				if (prev == head) {
					head = new;
					break;
				}
				prev->next = new;
				break;
			}
			else if (length == word->length) {
				letter = Alphabet(new->word, word->word, length, 0);
				while (word->next != NULL && word->next->length == length && letter != -1) {
					prev = word;
					word = word->next;
					letter = Alphabet(new->word, word->word, length, letter);
				}
				if (letter == -1) {
					new->next = word;
					if (prev == head) {
						head = new;
						break;
					}
					prev->next = new;
					break;
				}
				new->next = word->next;
				word->next = new;
				letter = 0;
				break;
			}
			else if (length > word->length) {
				prev = word;
				word = word->next;
				if (word == NULL) {
					prev->next = new;
					new->next = word;
					break;
				}
			}
		}
	}
	fclose(f);
	return head;
}

void Clearing(word_t* head) {
	word_t* word;
	word_t* prevWord;
	prevWord = head;
	word = head->next;
	while (word != NULL) {
		Clearing1Word(prevWord);
		prevWord = word;
		word = word->next;
	}
	Clearing1Word(prevWord);
}

void Clearing1Word(word_t* word) {
	free(word->word);
	free(word);
}

void PrintList(const char* name) {
	int N = 0;
	word_t* head = CreateList(name);
	if (head == NULL)
		return;
	word_t* word = head;
	while (word != NULL) {
		if (word->length > N)
			printf("%s\n", word->word);
		word = word->next;
	}
	Clearing(head);
}

void Check(const char* name) {
	int N;
	word_t* word;
	word_t* head = CreateList(name);
	if (head == NULL)
		return;
	printf("Enter a number\n");
	scanf_s("%d", &N);
	word = head;
	while (word != NULL) {
		if (word->length == N)
			printf("%s\n", word->word);
		word = word->next;
	}
	Clearing(head);
}