#include  <unistd.h>
#include  <fcntl.h>
int  main(){
  intx = 0;
  int f = 0;
  puts(" Otwieram ");
  f = open("test", O_WRONLY);
  sleep (10);
  puts(" Wysylam i koncze ");
  write(f, &x, sizeof(x));
  close(f);
}
