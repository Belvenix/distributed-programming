//liste dwukierunkowa posortowana funkcja komparatora jest przekazywana przez wskaźnik
//Napisz usun dodaj i wyswietl. stworz konsolowa aplikacje ktora bedziesz mogl to uzyc
//wskaznik na poprzedni, na nastepny i na przechowywana dana, do tego dac typ danych
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


//Comparison function for integer type
int compI(const void* const a, const void* const b){
	if(*(int*)a > *(int*)b) return -1;
	else if(*(int*)a == *(int*)b) return 0;
	else return 1;
}

//Comparison function for character type
int compC(const void* const a, const void* const b){
	if(*(char*)a > *(char*)b) return -1;
	else if(*(char*)a == *(char*)b) return 0;
	else return 1;
}

//Comparison function for 'struct Data' type
int compD(const void* const a, const void* const b){
	Data c = *(Data*) a;
	Data d = *(Data*) b;
	if (c.a > d.a){
		if(c.c != d.c){
			return -1;
		}
		else{
			return 1;
		}
	}
	else if (c.a == d.a){
		if (c.b > d.b){
			return -1;
		}
		else if(c.b == d.b){
			return 0;
		}
		else{
			return 1;
		}
		
		
	}
	else{
		if(c.c != d.c){
			return 1;
		}
		else{
			return -1;
		}
	}
	
	
}

//Comparison function for character pointer (string) type
int compS(const void* const a, const void* const b){
	return strcmp((char*)a,(char*)b);
}

//function that adds one element to the List. It doesn't need to know the type of the val
void addI(List* x, void * val){
	Node_t* tmp = (Node_t*)malloc(sizeof(Node_t));
	tmp->data = val;
	tmp->next = NULL;
	tmp->prev = NULL;
	if (x->list == NULL){
		x->list = tmp;	
	}
	else{
		Node_t * top = x->list;
		while (top!=NULL){
			if(x->cmp(top->data,tmp->data) == -1 || x->cmp(top->data,tmp->data) == 0){
				if(top->next == NULL){
					top->next = tmp;
					tmp->prev = top;
					return;
				}
				else{
					top = top->next;
				}
			}
			else if (x->cmp(top->data,tmp->data) == 1){
				if(top->prev == NULL){
					x->list = tmp;
					tmp->next = top;
					top->prev = tmp;
					return;
				}
				else
				{
					tmp->prev = top->prev;
					top->prev = tmp;
					tmp->prev->next = tmp;
					tmp->next = top;
					return;
				}	
			}
			else{
				printf("\nThere is an error in return vvalue of comparator function. Please make sure the values are exactly {-1,0,1}\n");
			}
		}
	}
}

//function removes only one element of the list at n location (starts from 1 from the beggining of the list)
void del(List * L, int n){
	Node_t * top = L->list;
	for (int i = 1; top != NULL && i < n; i++){
		top = top->next;
	}
	//Now we free this memory hue hue
	//Also here we would have to free our void structure
	if(top == NULL){
		printf("Trying to delete element that is not in the list!\n");
		//Something is clearly wrong
		return;
	}

	Node_t * prev = top->prev;
	Node_t * next = top->next;
	if(prev == NULL){
		if(next == NULL){
			L->list = NULL;
		}
		else{
			L->list = top->next;
			L->list->prev = NULL;
		}
		free(top->data);
		free(top);
		return;
	}
	else{
		if(next == NULL){
			prev->next = NULL;

		}
		else{
			prev->next = next;
			next->prev = prev;
		}
		free(top->data);
		free(top);
		return;
	}
}

//function clears whole L->list. Removes every element.
void del_all(List * L){
	Node_t * top = L->list;
	Node_t * tmp = L->list;
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

void print_i(Node_t * top){	
	printf("%i, ", *(int*)(top->data));
}

void print_c(Node_t * top){
	printf("%c, ", *(char*)(top->data));
}

void print_d(Node_t * top){
	printf("(%i; %c; ", (*(Data*)(top->data)).a,(*(Data*)(top->data)).b);
	printf("%s), ", (*(Data*)(top->data)).c ? "true" : "false");
}

void print_s(Node_t * top){
	printf("'%s', ",(char*)(top->data));
}

//function which prints the list according to specified type of variable
void printA(List* L){
	if (L->prnt != NULL){
		Node_t * top = L->list;
		printf("Printing the list:\n");
		while (top != NULL){	
			L->prnt(top);	
			top = top->next;
		}
		printf("\nFinished printing.\n\n");
	}
	else{
		printf("\nError: there is no print function assigned!");
		return;
	}
}

//function returns string from input (maximum 255 characters)
//it needs the pointer to variable to make sure it gets the data
char * getOneLine(char * line){
	if (fgets(line, sizeof line, stdin) == NULL) {
        printf("Input error.\n");
    }
	return line;
}

//function returns ONE char token without trailing white space 
char getOneToken(){
	char line[LINE_SIZE];
	char ch;
    ch = (getOneLine(line))[0];
	return ch;
}

//function returns an integer from input without trailing white space
int getOneInt(){
	char line[LINE_SIZE];
	int ret;
	if (fgets(line, sizeof line, stdin) == NULL) {
        printf("Input error.\n");
    }
	// atoi takes the number that comes from the beggining of the line, hence when ew type '123abc'
	// it will return 123 without an error. However if we use abc123 it will return 'error' which is
	// by default (int)0
	ret = atoi(line);
	return ret;
}

/*
This function is responsible for all of the application logic. We can add more tokens - options to our program
As well as our own varType that can be added to 2-way function.
L is a list with its comparison function L->cmp(void*,void*) and it's list element L->list which points to Node_t struct
which is a struct (void*,Node_t*,Node_t*)
token is our option and varType is our variable type we use in our list.
*/
void chooseOption(List* L, char token, char *varType){
	if(token =='p'){
		if(L->list != NULL){
			printA(L);
			return;
		}
		else
		{
			printf("Sorry! Your list seems to be empty!\n");
		}
		
	}
	else if(token == 'a'){
		if (*varType == 'i'){
			//Make sure we have a good comparison function
			L->cmp = compI;
			L->prnt = print_i;
			printf("\nPlease specify the integer to be added: ");
			int * a = malloc(sizeof(int));
			*a = getOneInt();
			addI(L,a);
			printf("\nSuccessfully added the integer %i.\n",*a);
			return;
		}
		else if(*varType == 'c'){
			//Make sure we have a good comparison function
			L->cmp = compC;
			L->prnt = print_c;
			printf("\nPlease specify the character to be added: ");
			char * a = malloc(sizeof(char));
			*a = getOneToken();
			addI(L,a);
			printf("\nSuccessfully added the character '%c'.\n", *a);
			return;
		}
		else if(*varType == 'd'){
			//Make sure we have a good comparison function
			L->cmp = compD;
			L->prnt = print_d;
			Data * a = malloc(sizeof(Data));
			printf("\nPlease specify the Data int to be added: ");
			a->a = getOneInt();
			printf("\nPlease specify the Data char to be added: ");
			a->b = getOneToken();
			while (token != 't' && token != 'f'){
				printf("\nPlease specify the Data bool to be added (t - true, f - false): ");
				token = getOneToken();
			}
			a->c = (token == 't') ? true : false;
			addI(L,a);
			printf("\nSuccessfully added the Data (a = %i, b = '%c', c = %s).\n",a->a,a->b,((a->c == 0) ? "true" : "false"));
			return;
		}
		else if(*varType == 's'){
			L->cmp = compS;
			L->prnt = print_s;
			printf("Please specify the string to be added: ");
			char * a = malloc(sizeof(char) * LINE_SIZE);
			gets(a);
			addI(L,a);
			printf("\nSuccessfully added the string '%s'.\n", a);
		}
		else if(*varType == '0'){
			printf("Please specify the data type to be added('i' - integer, 'c' - character, 'd' - Data struct, 's' - string): ");
			*varType = getOneToken();
			chooseOption(L, token, varType);
			return;
		}
		else{
			// If we specify wrong varType we get this message and we go back to the choice
			// WARNING it is possible to overflow if we keep doing it however the app looks simpler
			printf("Unknown data type '%c', please check whether you didn't misspelled something.\n", token);
			*varType = '0';
			chooseOption(L, token, varType);
			return;
		}
		
	}
	else if (token == 'd'){
		printf("Please specify which element you want to delete (starts from 1): ");
		int a=0;
		a = getOneInt();
		printf("\n");
		del(L,a);
		if (L->list == NULL){
			L->cmp = NULL;
			L->prnt = NULL;
		}
		return;
	}
	else if (token == 'r'){
		printf("Deleting every element in the list.\n");
		del_all(L);
		L->cmp = NULL;
		L->prnt = NULL;
		*varType = '0';
		return;
	}
	else if(token =='h'){
		printf("Available options:\n'a' - adds new element (user defined types)\n'p' - prints the whole list\n");
		printf("'d' - removes one specified element from the list\n'r' - removes all elements from the list\n");
		return;
	}
	else if(token =='q'){
		return;
	}
	else{
		printf("Unknown token '%c', please check whether you didn't misspelled something.\n", token);
		return;
	}
}

//TODO: make a small program that enables us to do some fancy stuff with this queue
void main(void){
	List *L = (List*)malloc(sizeof(List));
	L->cmp = NULL;
	L->list = NULL;
	L->prnt = NULL;
	char buffor = 'a';
	//This represents type of data we will have inside the list
	//Values of *varType variable:
	// '0' - means no type
	// 'i' - means integer
	// 'c' - means char
	// 's' - means string
	// 'd' - means struct Data defined above
	char *varType = (char*)malloc(sizeof(char));
	*varType = '0';
	printf("Welcome to the great application, which uses Jakub Belter's implementation of special sorted 2-way lists.\n");
	printf("First you will specify the command that you want to use. You will see the prompt that helps you decide the command\n");
	printf("Then you might be asked to specify more parameters if needed.\n");
	printf("To quit please enter 'q'. For help enter 'h'.\n\n");
	while (buffor != 'q')
	{
		printf("Please enter command: ");
		buffor = getOneToken();
		printf("\n");
		chooseOption(L, buffor, varType);		
	}
	free(varType);
	free(L); 
}