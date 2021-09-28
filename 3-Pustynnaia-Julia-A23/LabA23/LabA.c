#include "LabA.h"
#include <stdlib.h>
#include <stdio.h>

word_t* ReadWord(FILE* f) {
	int letter = 0;                                         
	int returnValue = 0;                                    
	char* writtenWord = NULL;                               
	char* tempStorage;
	word_t* word = (word_t*)malloc(sizeof(word_t));
	if (word == NULL)
		return NULL;
	word->next = NULL;
	writtenWord = (char*)malloc(sizeof(char));
	if (writtenWord == NULL)
		return NULL;
	returnValue = fscanf_s(f, "%c", &writtenWord[0], sizeof(char));
	while ((writtenWord[letter] >= 'A' && writtenWord[letter] <= 'Z') || (writtenWord[letter] >= 'a' && writtenWord[letter] <= 'z')) {
		letter++;
		tempStorage = (char*)realloc(writtenWord, (1 + letter) * sizeof(char));  
		if (tempStorage != NULL) {
			writtenWord = tempStorage;
		}
		returnValue = fscanf_s(f, "%c", &writtenWord[letter], sizeof(char));
	}
	writtenWord[letter] = '\0';                
	(*word).word = writtenWord;
	(*word).length = letter;
	if (letter > 0 && returnValue == -1)      
		(*word).length = -1;
	else if (letter == 0 && returnValue == -1)  
		(*word).length = -2;
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

word_t* CreateList(FILE* f) {
	word_t* new;
	word_t* word;
	word_t* prev;
	word_t* head = (word_t*)malloc(sizeof(word_t));
	if (head == NULL)
		return head;
	head->word = NULL;
	int letter = 0;
	int length;
	int finish = 0;                                         
	new = ReadWord(f);                                      
	while (new->length == 0)                                
		new = ReadWord(f);
	if (new->length == -2) {              
		head->next = new;
		return head;
	}
	if (new->length == -1) {                                
		while (new->word[letter] != '\0')                        
			letter++;
		new->length = letter;
		head->next = new;
		return head;
	}
	new->next = NULL;
	head->next = new;                                        
	while (finish != 1) {                                    
		prev = head;                                         
		new = ReadWord(f);                                   
		while (new->length == 0) {                           
			if (new->length == -2) {                         
				free(new);
				break;
			}
			new = ReadWord(f);
		}
		if (new->length == -2)                                    
			break;
		if (new->length == -1) {                                  
			while (new->word[letter] != '\0')
				letter++;
			new->length = letter;
			finish = 1;
		}
		length = new->length;                                      
		word = head->next;                                         
		while (word != NULL) {
			if (length < word->length) {
				new->next = word;
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
					prev->next = new;
					new->next = word;
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
	return head;
}

void Clearing(word_t* head) {                           
	word_t* word;
	word_t* prevWord;
	prevWord = head;
	word = head->next;
	while (word != NULL) {
		free(prevWord->word);
		free(prevWord);
		prevWord = word;
		word = word->next;
	}
	free(prevWord);
}

void PrintList(FILE* f) {                             
	int N = 1;
	word_t* head = CreateList(f);
	if (head == NULL)
		return;
	word_t* word;
	word = head->next;
	while (word != NULL) {
		if (word->length > N)
			printf("%s\n", word->word);
		word = word->next;
	}
	Clearing(head);
}

void Check(FILE* f) {                                  
	int N;
	word_t* word;
	word_t* head = CreateList(f);
	if (head == NULL)
		return;
	printf("Enter a number\n");
	scanf_s("%d", &N);
	word = head->next;
	while (word != NULL) {
		if (word->length == N)
			printf("%s\n", word->word);
		word = word->next;
	}
	Clearing(head);
}