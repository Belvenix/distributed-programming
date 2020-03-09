#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#define LINE_SIZE 256

typedef enum {true, false} bool;

typedef struct Node_t{
	void *data;
	struct Node_t *prev;
	struct Node_t *next;
} Node_t;

typedef struct List{
	int (*cmp)(const void* const a, const void* const b);
	void (*prnt)(Node_t *L);
	Node_t* list;
} List;

typedef struct Data{
	int a;
	char b;
	bool c;
} Data;

int compI(const void* const a, const void* const b);

//Comparison function for character type
int compC(const void* const a, const void* const b);

//Comparison function for 'struct Data' type
int compD(const void* const a, const void* const b);

//Comparison function for character pointer (string) type
int compS(const void* const a, const void* const b);

//function that adds one element to the List. It doesn't need to know the type of the val
void addI(List* x, void * val);

//function removes only one element of the list at n location (starts from 1 from the beggining of the list)
void del(List * L, int n);

//function clears whole L->list. Removes every element.
void del_all(List * L);

void print_i(Node_t * top);

void print_c(Node_t * top);

void print_d(Node_t * top);

void print_s(Node_t * top);