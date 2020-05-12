#include  <unistd.h>
#include  <stdio.h>
#include <string.h>
#define  ODCZYT 0
#define  ZAPIS 1
#define  WORKERS  3
#define MAX_BUFFER 100
#define DEBUG 1
#define NO_DEBUG 0

int  semafor [2];

void  LOCK(){
  int x;
  read(semafor[ODCZYT], &x, sizeof(x));
}

void  UNLOCK (){
  int x = 0;
  write(semafor[ZAPIS], &x, sizeof(x));
}

void change(const char * org, char * ret){
  strcpy(ret, org);
  for (size_t i = 0; i < strlen(ret); i++) {
    if(ret[i] >= 'a' && ret[i] <= 'z'){
      ret[i] = ret[i] - 32;
    }
    else if (ret[i] >= 'A' && ret[i] <= 'Z'){
      ret[i] = ret[i] + 32;
    }
  }
}

void change2(const char * org, char * ret){
  strcpy(ret, org);
  for (size_t i = 0; i < strlen(ret); i++) {
    if(ret[i] == ' '){
      ret[i] = '\n';
    }
  }
}

void  first(int id , int  potok_k, int potok_d [2], int debug){
  int x = 0;

  //blokuje lokalne workery
  LOCK();
  if(debug > 0)
    printf ("Worker %d startuje\n", id);
  close(potok_d[ODCZYT]);
  char string[MAX_BUFFER];
  scanf("%[^\n]%*c",string);
  if(debug > 0)
    printf ("Worker %s startuje\n", string);
  write(potok_d[ZAPIS], &string, sizeof(string));
  close(potok_d[ZAPIS]);
  if(debug > 0)
    printf ("Worker %d konczy\n", id);
  UNLOCK ();

  write(potok_k , &x, sizeof(x));
}

void  second(int id , int  potok_k, int potok_d [2], int debug){
  int x = 0;

  //blokuje lokalne workery
  LOCK();
  if(debug > 0)
    printf ("Worker %d startuje\n", id);

  char string[MAX_BUFFER];
  read(potok_d[ODCZYT], &string, sizeof(string));
  if(debug > 0)
    printf("String to [%s]\n", string);

  char string2[MAX_BUFFER];
  change(string, string2);
  if(debug > 0)
    printf("Zmieniony String to [%s]\n", string2);
  write(potok_d[ZAPIS], &string2, sizeof(string2));

  close(potok_d[ODCZYT]);
  close(potok_d[ZAPIS]);
  if(debug > 0)
    printf ("Worker %d konczy\n", id);

  UNLOCK ();

  write(potok_k , &x, sizeof(x));
}

void  third(int id , int  potok_k, int potok_d [2], int debug){
  int x = 0;
  //blokuje lokalne workery
  LOCK();
  if(debug > 0)
    printf ("Worker %d startuje\n", id);

  char string[MAX_BUFFER];
  read(potok_d[ODCZYT], &string, sizeof(string));
  if(debug > 0)
    printf("String to [%s]\n", string);

  char string2[MAX_BUFFER];
  change2(string, string2);
  if(debug > 0)
    printf("Zmieniony String to [%s]\n", string2);
  write(potok_d[ZAPIS], &string2, sizeof(string2));

  close(potok_d[ZAPIS]);
  close(potok_d[ODCZYT]);
  if(debug > 0)
    printf ("Worker %d konczy\n", id);

  UNLOCK ();

  write(potok_k , &x, sizeof(x));
}


int  main(){
  int  potok_konczacy [2], potok_danowy [2], i, x;
  pipe(potok_danowy);
  pipe(potok_konczacy);
  pipe(semafor);
  UNLOCK (); //  inicjalizacja  semafora
  for(i = 0; i < WORKERS; i++){
    if(!fork()){
      if(i == 0){
        first(i, potok_konczacy[ZAPIS], potok_danowy, NO_DEBUG);
      }
      else if(i == 1){
        second(i, potok_konczacy[ZAPIS], potok_danowy, NO_DEBUG);
      }
      else if (i == 2){
        third(i, potok_konczacy[ZAPIS], potok_danowy, NO_DEBUG);
      }

      return  0;
    }
  }

  //nie konczy sie main
  for(i = 0; i < WORKERS; i++)
    read(potok_konczacy[ODCZYT], &x, sizeof(x));
  char string[MAX_BUFFER];
  read(potok_danowy[ODCZYT], &string, sizeof(string));
  printf("Wynik koncowy to: [%s]\n", string);
  puts(" koniec ");
}
