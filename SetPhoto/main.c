#include <stdio.h>
#include <stdlib.h>
void *RunSetPhoto(void *);
int main(void) { 
  if(getenv("HOME")!= NULL) chdir(getenv("HOME"));
  RunSetPhoto(NULL);
  return 1;
}
