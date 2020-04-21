#include "list.h"

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