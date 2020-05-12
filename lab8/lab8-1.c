#include  <unistd.h>
#include <fcntl.h>
#include  <stdio.h>
#include <string.h>
#define MAX_BUFFER 100
#define DEBUG 1
#define NO_DEBUG 0

void  first(int id, int debug){
  int f = 0;

  if(debug > 0)
    printf ("Worker %d startuje\n", id);

  char string[MAX_BUFFER];
  scanf("%[^\n]%*c",string);
  f = open("test", O_WRONLY);
  write(f, &string, sizeof(string));
  close(f);
  if(debug > 0)
    printf ("Worker %d konczy\n", id);
}

int  main(){
  first(0,NO_DEBUG);
  return 0;
}
