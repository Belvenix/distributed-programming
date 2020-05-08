#include  <stdio.h>
#include  <pthread.h>
void *f(void *i){
  printf (" Thread  says: Hallo  world\n");
  return  NULL;
}

int  main(){
  pthread_t w;
  pthread_create(&w, NULL , f, NULL);
  pthread_join(w, NULL);
  printf (" Hello  world\n");
}
