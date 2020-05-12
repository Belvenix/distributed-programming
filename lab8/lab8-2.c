#include  <unistd.h>
#include <fcntl.h>
#include  <stdio.h>
#include <string.h>
#define MAX_BUFFER 100
#define DEBUG 1
#define NO_DEBUG 0

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

void  second(int id, int debug){
  int f = 0;

  if(debug > 0)
    printf ("Worker %d startuje\n", id);

  char string[MAX_BUFFER];
  if(debug > 0)
    printf ("Otwieram pierwszy fifo\n");
  f = open("test", O_RDONLY);
  read(f, &string, sizeof(string));

  if(debug > 0)
    printf("String to [%s]\n", string);

  char string2[MAX_BUFFER];
  change(string, string2);
  close(f);
  if(debug > 0)
    printf ("Otwieram drugi fifo\n");
  f = open("test2", O_WRONLY);
  write(f, &string2, sizeof(string2));
  close(f);
  if(debug > 0)
    printf("Zmieniony String to [%s]\n", string2);

  if(debug > 0)
    printf ("Worker %d konczy\n", id);
}

int  main(){
  second(1,DEBUG);
}
