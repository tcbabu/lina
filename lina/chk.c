#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#include <termio.h>
char TTY[100];

static struct termios tty;

int settty(void) {
 int ierr,op=0;
 op = open(TTY,O_RDWR|O_SYNC);
// ierr=ioctl(op, TCGETS,  &tty);
// if(ierr== -1) printf("ERROR IN  IOCTL\n");
// tty.c_lflag=ICANON;

 ierr=ioctl(op, TCSETS, &tty);
 if(ierr==-1 )
   printf("ERROR IN IOCTL\n");
 close(op);
 return;
}
int main(void) {
   FILE *pp;
   int ierr,i;
   char buff[300];
   pp=popen("tty","r");
   if(pp != NULL) {
     int op;
     fscanf(pp,"%s",TTY);
     pclose(pp);
     op = open(TTY,O_RDWR|O_SYNC);
     ierr=ioctl(0, TCGETS,  &tty);
     if(ierr== -1) printf("ERROR IN  IOCTL\n");
     printf("TTY: %s\n",TTY);
     printf("iflag: %d oflag: %d cflag: %d lflag: %d\n",tty.c_iflag,tty.c_oflag,tty.c_cflag,tty.c_lflag);
     for(i=0;i<NCCS;i++) printf(" %x ",tty.c_cc[i]);
     printf("\n");
     close(op);
   }
         settty();
         printf("Calling change job\n");
         sprintf(buff,"stty -F %-s sane",TTY);
         system(buff);
   return 1;
}
