#include  <unistd.h>
#include  <stdio.h>
#define  ODCZYT 0
#define  ZAPIS 1
#define  WORKERS  10
int  semafor [2];

void  LOCK(){
  int x;
  read(semafor[ODCZYT], &x, sizeof(x));
}

void  UNLOCK (){
  int x = 0;
  write(semafor[ZAPIS], &x, sizeof(x));
}

void  first(int id , int  potok_k, int potok_d [2]){
  int x = 0,i,j;
  printf (" Worker %d startuje\n", id);
  //blokuje lokalne workery
  LOCK();
  for(i = 0; i < 7; i++){
    printf ("[%d] ", id);
    fflush(stdout);
    sleep (1);
  }
  UNLOCK ();
  printf (" Worker %d konczy\n", id);
  write(potok_k , &x, sizeof(x));
}

void  second(int id , int  potok_k, int potok_d [2]){

}

void  third(int id , int  potok_k, int potok_d [2]){

}


int  main(){
  int  potok_konczacy [2], potok_danowy [2], i, x;
  pipe(potok_danowy);
  pipe(potok_konczacy);
  pipe(semafor);
  UNLOCK (); //  inicjalizacja  semafora
  for(i = 0; i < WORKERS; i++){
    if(!fork()){
      worker(i, potok_konczacy[ZAPIS], potok_d);
      return  0;
    }
  }
  //nie konczy sie main
  for(i = 0; i < WORKERS; i++)
    read(potok_konczacy[ODCZYT], &x, sizeof(x));
  puts(" koniec ");
}
