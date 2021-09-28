#include "LabA.h"
#include <stdlib.h>
#include <stdio.h>

word_t* ReadWord(FILE* f) {
	int letter = 0;                                         //место буквы в массиве
	int returnValue = 0;                                    //возвращаемое значение fscanf_s, если -1 конец файла
	char* writtenWord = NULL;                               //тут мы записываем слово
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
		tempStorage = (char*)realloc(writtenWord, (1 + letter) * sizeof(char));  //выделяем память для ещё одного символа
		if (tempStorage != NULL) {
			writtenWord = tempStorage;
		}
		returnValue = fscanf_s(f, "%c", &writtenWord[letter], sizeof(char));
	}
	writtenWord[letter] = '\0';                //так как последний символ будет разделителем или же концом файла, мы его меняем на символ конца строки
	(*word).word = writtenWord;
	(*word).length = letter;
	if (letter > 0 && returnValue == -1)       //случай, когда мы записали слово и оно оказалось в конце файла
		(*word).length = -1;
	else if (letter == 0 && returnValue == -1)  //случай, когда в конце файла оказался разделитель
		(*word).length = -2;
	return word;
}

char ChangeRegister(char letter) {              //меняем заглавные буквы на прописные
	if (letter >= 'A' && letter <= 'Z')
		letter = letter + ('a' - 'A');
	return letter;
}

int Alphabet(char* newWord, char* prevWord, int length, int letter) {
	char newLetter;                        //буква слова, которое надо поместить в список
	char prevLetter;                       //буква слова, с которым мы сравниваем наше прочитанное только что
	newLetter = ChangeRegister(newWord[letter]);
	prevLetter = ChangeRegister(prevWord[letter]);
	while (newLetter == prevLetter) {
		letter++;
		newLetter = ChangeRegister(newWord[letter]);
		prevLetter = ChangeRegister(prevWord[letter]);
		if (letter == length)                       //случай, когда слова одинаковы
			return -1;
	}
	if (prevLetter > newLetter)                     //случай, когда новое слово стоит по алфавиту первее того слова, с которым мы сравниваем 
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
	int finish = 0;                                          //переменная для того, чтобы понять, когда будет конец файла
	new = ReadWord(f);                                       //читаем первое слово
	while (new->length == 0)                                 //случай, когда это не слово, а разделитель
		new = ReadWord(f);
	if (new->length == -2) {               //случай, когда в файле есть только разделитель
		head->next = new;
		return head;
	}
	if (new->length == -1) {                                 //случай, когда первое слово является и последним
		while (new->word[letter] != '\0')                         //считаем его длину
			letter++;
		new->length = letter;
		head->next = new;
		return head;
	}
	new->next = NULL;
	head->next = new;                                        //узел с указателем на первый элемент
	while (finish != 1) {                                    //проверка на то, закончился файл или нет
		prev = head;                                         //указатель на предыдущий элемент
		new = ReadWord(f);                                   //читаем следующее слово
		while (new->length == 0) {                           //проверка на разделитель
			if (new->length == -2) {                         //проверка на то, что последним элементом является разделитель
				free(new);
				break;
			}
			new = ReadWord(f);
		}
		if (new->length == -2)                                    //выходим, если разделитель в конце файла
			break;
		if (new->length == -1) {                                  //проверка на последнее слово в файле
			while (new->word[letter] != '\0')
				letter++;
			new->length = letter;
			finish = 1;
		}
		length = new->length;                                      //длина поступившего к нам слова
		word = head->next;                                         //начнем сравнивать новое слово с другими с первого слова в списке
		while (word != NULL) {
			if (length < word->length) {
				new->next = word;
				prev->next = new;
				break;
			}
			else if (length == word->length) {
				letter = Alphabet(new->word, word->word, length, 0);                            //расставляем 2 слова по алфавиту
				while (word->next != NULL && word->next->length == length && letter != -1) {    //делаем то же самое с другими словами, если новое слово стоит дальше по алфавиту проверяемого нами слова и у последнего такая же длина
					prev = word;
					word = word->next;
					letter = Alphabet(new->word, word->word, length, letter);
				}
				if (letter == -1) {                            //случай, когда слова одинаковы, либо же новое слово оказалось стоящим первее по алфавиту
					prev->next = new;
					new->next = word;
					break;
				}
				new->next = word->next;
				word->next = new;
				letter = 0;
				break;
			}
			else if (length > word->length) {       //случай, когда слово оказалось длиннее сравниваемого с ним
				prev = word;
				word = word->next;
				if (word == NULL) {                 //когда мы дошли до конца файла и не нашли слова длиннее нового
					prev->next = new;
					new->next = word;
					break;
				}
			}
		}
	}
	return head;
}

void Clearing(word_t* head) {                           //очистка памяти
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

void PrintList(FILE* f) {                              //печатаем слова, длина которых больше N
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

void Check(FILE* f) {                                  //проверяем, существуют ли в нашем списке слова заданной нами длины
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