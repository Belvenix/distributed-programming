//liste dwukierunkowa posortowana funkcja komparatora jest przekazywana przez wskaźnik
//Napisz usun dodaj i wyswietl. stworz konsolowa aplikacje ktora bedziesz mogl to uzyc
//wskaznik na poprzedni, na nastepny i na przechowywana dana, do tego dac typ danych

#include "list.h"

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
			printf("Unknown data type '%c', please check whether you didn't misspelled something.\n", *varType);
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
			*varType = '0';
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
int main(void){
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
	//We need to delete everything from the list beforehand
	chooseOption(L, 'r', varType);
	free(varType);
	free(L); 
	return 1;
}