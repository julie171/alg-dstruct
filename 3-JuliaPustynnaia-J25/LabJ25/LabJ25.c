#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define INPUT_SIZE 16
#define SIZEMAP 4000133
#define CONST_1 4000133
#define CONST_2 9157

typedef struct {
	int key;
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

hashTable_t* TableInit(int size) {
	hashTable_t* table = (hashTable_t*)malloc(sizeof(hashTable_t));
	if (table == NULL)
		return NULL;
	table->size = size;
	table->nodes = (node_t*)malloc(size * sizeof(node_t));
	if (table->nodes == NULL) {
		free(table);
		return NULL;
	}
	for (int i = 0; i < table->size; ++i) {
		table->nodes[i].value = NULL;
		table->nodes[i].isFilled = 0;
	}
	return table;
}

int FindNode(hashTable_t* map, int key) {
	int x = key % CONST_1;
	int y = 1 + (key % CONST_2);
	int index = 0;
	for (int i = 0; i < map->size; ++i) {
		index = (x + i * y) % (map->size);
		node_t* node = &map->nodes[index];
		if (node->isFilled == 1 && node->key == key)
			return 1;
		else if (node->isFilled == 0)
			return 0;
	}
	return 0;
}

void AddNode(hashTable_t* map, int key, char* str) {
	int x = key % CONST_1;
	int y = 1 + (key % CONST_2);
	int index = 0;
	if (FindNode(map, key) == 1)
		return;
	for (int i = 0; i < map->size; ++i) {
		index = (x + i * y) % (map->size);
		node_t* node = &map->nodes[index];
		if (node->isFilled == 0) {
			node->value = CopyStr(str);
			if (node->value == NULL)
				return;
			node->isFilled = 1;
			node->key = key;
			return;
		}
	}
}

void RemoveNode(hashTable_t* map, int key) {
	if (FindNode(map, key) == 0)
		return;
	int x = key % CONST_1;
	int y = 1 + (key % CONST_2);
	int index = 0;
	for (int i = 0; i < map->size; ++i) {
		index = (x + i * y) % (map->size);
		node_t* node = &map->nodes[index];
		if (node->isFilled == 1 && node->key == key) {
			node->isFilled = 0;
			free(node->value);
			node->value = NULL;
			node->key = 0;
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
	free(table);
	return;
}

int Interface() {
	char buf[INPUT_SIZE] = { 0 };
	char action = 0;
	int key = 0;
	hashTable_t* table = TableInit(SIZEMAP);
	if (table == NULL)
		return 0;
	while (fgets(buf, INPUT_SIZE, stdin) != NULL) {
		sscanf(buf, "%c %d", &action, &key);
		switch (action) {
		case 'a':
			AddNode(table, key, "line");
			break;
		case 'r':
			RemoveNode(table, key);
			break;
		case 'f':
			fprintf(stdout, "%s\n", FindNode(table, key) ? "yes" : "no");
			break;
		default:
			FreeTable(table);
			return 0;
		}
	}
	FreeTable(table);
	return 1;
}

int main() {
	if (Interface() == 0)
		return -1;
	return 0;
}