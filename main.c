//liste dwukierunkowa posortowana funkcja komparatora jest przekazywana przez wskaźnik
//Napisz usun dodaj i wyswietl. stworz konsolowa aplikacje ktora bedziesz mogl to uzyc
//wskaznik na poprzedni, na nastepny i na przechowywana dana, do tego dac typ danych
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>


typedef struct Node{
	void *data;
	struct Node *prev;
	struct Node *next;
} Node;

typedef struct List{
	int (*cmp)(void*,void*);
	Node* list;
} List;

//TODO: make more comparison functions
int compI(void* a, void* b){
	if(*(int*)a > *(int*)b) return -1;
	else if(*(int*)a == *(int*)b) return 0;
	else return 1;
}
//TODO: make more option of what we want to add or alternatively add more functions
//NAAAAH just remove the int from there and replace it with void * val and then play with it ;)
void addI(List* x, int val){
	Node* tmp = (Node*)malloc(sizeof(Node));
	tmp->data = malloc(sizeof(int));
	*(int*)(tmp->data) = val;
	tmp->next = NULL;
	tmp->prev = NULL;
	if (x->list == NULL){
		x->list = tmp;	
	}
	else{
		Node * top = x->list;
		
		while(top->next != NULL){
			//compI returns -1 or 0 if first element is bigger or equal to the second element. 1 Otherwise
			if(compI(top->data, tmp->data) == -1 ||compI(top->data, tmp->data) == 0 ){
				top = top->next;
			}
			else{
				Node * prevtop = top->prev;
				prevtop->next = tmp;
				top->prev = tmp;
				tmp->next = top;
				tmp->prev = prevtop;
				return;
			}		
		}
		if(compI(top->data, tmp->data) == -1 ||compI(top->data, tmp->data) == 0 ){
			top->next = tmp;
			tmp->prev = top;
			return;
		}
		else{
			//We don't really care which object has who, the most important thing is that they are sorted
			int *swap = top->data;
			top->data = tmp->data;
			tmp->data = swap;
			top->next = tmp;
			tmp->prev = top;
			return;
		}
		
		
	}
}

void del(List * L, int n){
	Node * top = L->list;
	for (int i = 1; top != NULL && i < n; i++)
	{
		top = top->next;
	}
	//Now we free this memory hue hue
	//Also here we would have to free our void structure
	if(top == NULL){
		//Something is clearly wrong
		return;
	}

	Node * prev = top->prev;
	Node * next = top->next;
	if(prev == NULL){
		L->list = top->next;
		free(top->data);
		free(top);
		return;
	}
	else if (next == NULL){
		prev->next = NULL;
		free(top->data);
		free(top);
		return;
	}
	else{
		prev->next = next;
		next->prev = prev;
		free(top->data);
		free(top);
		return;
	}
}

void del_all(List * L){
	Node * top = L->list;
	Node * tmp = L->list;
	while (top!=NULL)
	{
		tmp = top->next;
		top->prev = NULL;
		top->next = NULL;
		free(top->data);
		free(top);
		top = tmp;
	}
	L->list = NULL;
}

//TODO: make an option of what we want to print ;)
void print_u(List* L, char type){
	
	Node * top = L->list;
	if (type == 'i'){
		printf("Printing the integer list:\n");	
		while (top != NULL){	
			printf("%i, ", *(int*)(top->data));
			top = top->next;
		}
		printf("\nFinished printing.\n");
	}	
}

char getOneToken(){
	char line[256];
	char ch;
	if (fgets(line, sizeof line, stdin) == NULL) {
        printf("Input error.\n");
    }
    ch = line[0];
	return ch;
}

// atoi takes the number that comes from the beggining of the line, hence when ew type '123abc'
// it will return 123 without an error. However if we use abc123 it will return 'error' which is
// by default (int)0
int getOneInt(){
	char line[256];
	int ret;
	if (fgets(line, sizeof line, stdin) == NULL) {
        printf("Input error.\n");
    }
	ret = atoi(line);
	return ret;
}

void chooseOption(List* L, char token){
	if(token =='p'){
		print_u(L, 'i');
	}
	else if(token == 'a'){
		printf("Please specify the data type to be added('i' - integer): ");
		token = getOneToken();
		if (token == 'i'){
			printf("\nPlease specify the integer to be added: ");
			int * a = (int*)malloc(sizeof(int));
			scanf(" %i",a);
			addI(L,*a);
		}
	}
	else if (token == 'd'){
		printf("Please specify which element you want to delete (starts from 1): ");
		int a=0;
		a = getOneInt();
		printf("\n");
		del(L,a);
	}
	else if (token == 'r')
	{
		del_all(L);
	}
	else if(token =='h'){
		printf("Available options:\n'a' - adds new element (user defined type)\n'p' - prints the whole list\n");
		printf("'d' - removes one specified element from the list\n'r' - removes all elements from the list\n");
	}
	
	
}

//TODO: make a small program that enables us to do some fancy stuff with this queue
void main(void){
	List *L = (List*)malloc(sizeof(List));
	L->cmp = compI;
	L->list = NULL;
	char buffor = 'a';
	printf("Welcome to the great application, which uses Jakub Belter's implementation of special sorted 2-way lists.\n");
	printf("First you will specify the command that you want to use. You will see the prompt that helps you decide the command\n");
	printf("Then you might be asked to specify more parameters if needed.\n");
	printf("To quit please enter 'q'\n\n");
	while (buffor != 'q')
	{
		printf("Please enter command: ");
		buffor = getOneToken();
		printf("\n");
		if (buffor == 'a'){
			int a = rand();
		}
		chooseOption(L,buffor);
		
	}
	del_all(L);
	free(L);
}