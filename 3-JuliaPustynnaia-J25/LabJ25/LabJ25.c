#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define INPUT_SIZE 128
#define MAP_SIZE 4000133
#define CONST_1 4000133
#define CONST_2 9157

typedef struct {
	char* value;
	int isFilled;
} node_t;

typedef struct {
	int size;
	node_t* nodes;
} hashTable_t;


char* CopyStr(char* str) {
	int length = strlen(str) + 1;
	char* newStr = (char*)malloc(length * sizeof(char));
	if (newStr == NULL)
		return NULL;
	newStr[length - 1] = '\0';
	strcpy(newStr, str);
	return newStr;
}

hashTable_t TableInit(int size) {
	hashTable_t table;
	table->size = size;
	table->nodes = (node_t*)malloc(size * sizeof(node_t));
	if (table->nodes == NULL)
		table->size = 0;
	for (int i = 0; i < table->size; ++i) {
		table->nodes[i].value = NULL;
		table->nodes[i].isFilled = 0;
	}
	return table;
}

unsigned Func1(char* str, int size) {
	unsigned res = 0;
	for (int i = 0; str[i] != 0; ++i) {
		res += (unsigned)str[i];
		res %= CONST_1;
	}
	return res;
}

unsigned Func2(char* str, int size) {
	unsigned res = 0;
	for (int i = 0; str[i] != 0; ++i) {
		res += (unsigned)str[i];
		res %= CONST_2;
	}
	return 1 + res;
}

int FindNode(hashTable_t* map, const char* str) {
	unsigned x = Func1;
	unsigned y = Func2;
	unsigned index = 0;
	for (int i = 0; i < map->size; ++i) {
		index = (x + i * y) % (map->size);
		node_t* node = &map->nodes[index];
		if (node->isFilled == 1 && strcmp(node->value, str) == 0)
			return 1;
		else if (node->isFilled == 0)
			return 0;
	}
	return 0;
}

void AddNode(hashTable_t* map, const char* str) {
	if (FindNode(map, str) == 1)
		return;
	unsigned x = Func1;
	unsigned y = Func2;
	unsigned index = 0;
	for (int i = 0; i < map->size; ++i) {
		index = (x + i * y) % (map->size);
		node_t* node = &map->nodes[index];
		if (node->isFilled == 0) {
			node->value = CopyStr(str);
			if (node->value == NULL)
				return;
			node->isFilled = 1;
			return;
		}
	}
}

void RemoveNode(hashTable_t* map, const char* str) {
	if (FindNode(map, str) == 0)
		return;
	unsigned x = Func1;
	unsigned y = Func2;
	unsigned index = 0;
	for (int i = 0; i < map->size; ++i) {
		index = (x + i * y) % (map->size);
		node_t* node = &map->nodes[index];
		if (node->isFilled == 1 && strcmp(node->value, str) == 0) {
			node->isFilled = 0;
			free(node->value);
			node->value = NULL;
			return;
		}
	}
}

void FreeTable(hashTable_t* table) {
	for (int i = 0; i < table->size; ++i)
		if (table->nodes[i].value != NULL) {
			free(table->nodes[i].value);
			table->nodes[i].value = NULL;
		}
	free(table->nodes);
	table->nodes = NULL;
	table->size = 0;
	return;
}

int Interface() {
	char buf[INPUT_SIZE] = { 0 };
	char action = 0;
	char str[INPUT_SIZE] = { 0 };
	hashTable_t table = TableInit(MAP_SIZE);
	if (table.size == 0)
		return 0;
	while (fgets(buf, INPUT_SIZE, stdin) != NULL) {
		sscanf(buf, "%c %s", &action, &str);
		switch (action) {
		case 'a':
			AddNode(&table, str);
			break;
		case 'r':
			RemoveNode(&table, str);
			break;
		case 'f':
			fprintf(stdout, "%s\n", FindNode(&table, str) ? "yes" : "no");
			break;
		default:
			FreeTable(&table);
			return 0;
		}
	}
	FreeTable(&table);
	return 1;
}

int main() {
	if (Interface() == 0)
		return -1;
	return 0;
}