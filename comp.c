#include "list.h"


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