#include  <unistd.h>
#include <fcntl.h>
#include  <stdio.h>
#include <string.h>
#define MAX_BUFFER 100
#define DEBUG 1
#define NO_DEBUG 0

void change2(const char * org, char * ret){
  strcpy(ret, org);
  for (size_t i = 0; i < strlen(ret); i++) {
    if(ret[i] == ' '){
      ret[i] = '\n';
    }
  }
}

void  third(int id, int debug){
  int f = 0;

  if(debug > 0)
    printf ("Worker %d startuje\n", id);

  char string[MAX_BUFFER];
  f = open("test2", O_RDONLY);
  if(debug > 0)
    printf("String to [%s]\n", string);

  char string2[MAX_BUFFER];
  change2(string, string2);
  write(f, &string2, sizeof(string2));
  close(f);

  if(debug > 0)
    printf("Zmieniony String to [%s]\n", string2);

  printf("Wynik to: %s\n", string2);

  if(debug > 0)
    printf ("Worker %d konczy\n", id);
}


int  main(){
  third(2,DEBUG);
}
