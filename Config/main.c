#include <stdio.h>
void *Runconfiglina(void *);
int main(void) { 
  FILE *fp;
  fp = fopen("/etc/Junk","w");
  if(fp== NULL) return 0;
  else { fclose(fp); remove("/etc/Junk");}
  Runconfiglina(NULL);
  return 1;
}
