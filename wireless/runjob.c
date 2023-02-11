
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <termio.h>
#include <fcntl.h>
#include <math.h>
#include <string.h>
#include "wireless.h"
#include <sys/types.h>
#include <sys/wait.h>

#define WAIT(pid) {\
  pid_t w;\
  int status;\
  do { \
                   w = waitpid(pid, &status, WUNTRACED | WCONTINUED); \
                   if (w == -1) { \
                       perror("waitpid"); \
                       exit(EXIT_FAILURE); \
                   }\
               } while (!WIFEXITED(status) && !WIFSIGNALED(status));\
}

#if 0
int settty(void) {
 int ierr,op=0,i;
 FILE *pp;
   pp=popen("tty","r");
   if(pp != NULL) {
     int op;
     fscanf(pp,"%s",TTY);
     pclose(pp);
     strcpy(TTY,"/dev/tty1");
     op = open(TTY,O_RDWR|O_SYNC);
     ierr=ioctl(op, TCGETS,  &tty);
     if(ierr== -1) fprintf(stderr,"ERROR IN  IOCTL\n");
     fprintf(stderr,"TTY: %s\n",TTY);
 tty.c_iflag=9474;
 tty.c_oflag=5;
 tty.c_cflag=1215;
 tty.c_lflag=35387;
 for(i=0;i<NCCS;i++) tty.c_cc[i]=0;
 tty.c_cc[0]=0x03;
 tty.c_cc[1]=0x1c;
 tty.c_cc[2]=0x7f;
 tty.c_cc[3]=0x15;
 tty.c_cc[4]=0x04;
 tty.c_cc[5]=0x00;
 tty.c_cc[6]=0x01;
 tty.c_cc[7]=0x00;
 tty.c_cc[8]=0x11;
 tty.c_cc[9]=0x13;
 tty.c_cc[10]=0x1a;
 tty.c_cc[11]=0x00;
 tty.c_cc[12]=0x12;
 tty.c_cc[13]=0x0f;
 tty.c_cc[14]=0x17;
 tty.c_cc[15]=0x16;

 ierr=ioctl(op, TCSETS, &tty);
 if(ierr==-1 )
   fprintf(stderr,"ERROR IN IOCTL\n");
 close(op);
 }
 return;
}
#endif

int CheckString(char *s1,char *s2) {
   int ch,ln,ret=0;
   ln = strlen(s2);
   if(strlen(s1)<ln) ret=0;
   else {
     ch = s1[ln];
     s1[ln]='\0';
     if( strcmp(s1,s2)==0 ) ret=1;
     s1[ln]=ch;
   }
   return ret;
}
int SearchString(char *s1,char *s2) {
   int ch,ln,ret=-1,ln1,i=0;
   ln = strlen(s2);
   ln1 = strlen(s1);
   if(ln1<ln) ret=-1;
   else {
    i=0;
    for(i=0;i<=(ln1-ln);i++) {
      if(CheckString(s1+i,s2)){
        ret=i;
        break;
      }
    }
   }
   return ret;
}
void User1Signal(int sig) {
   fprintf(stderr,"Got Signal ;%d\n",sig);
}
void User2Signal(int sig) {
   fprintf(stderr,"Got Signal;%d\n",sig);
}
#if 0
void HangupSignal(int sig) {
   kill(Sid,sig);
}
#endif
int GetLine(int pip0,char *buff){
     unsigned char ch;
     fd_set rfds;
     struct timeval tv;
     int retval,chnl,status,i;
     int ret,val;
     chnl = pip0;
     i=0;
     FD_ZERO(&rfds);
     FD_SET(pip0,&rfds);
     while(1) {
       tv.tv_sec = 10;
       tv.tv_usec =0;
       ret=0;
       retval = select(chnl+1,&rfds,NULL,NULL,&tv);
       val=1;
       if((retval> 0)&&(FD_ISSET(chnl,&rfds))){
         if(read(chnl,&ch,1) != 1){
           ret=0;
           break;
         }
         buff[i++]=ch;
         if( (ch=='\n')||(ch=='\r')) {ret=ch;break;}
       }
       else {ret=-1;break;}
     }
     buff[i]='\0';
     return ret;
}
int ProcessXOut(int pip0,int pip1,int Pid) {
     char buff[1000],device[20],connection[20],reso[30];
     int ch,i=0,j,BRK=0,ret=1;
     write(pip1,"\n",1);
     return ret;
}
int ProcessStop(int pip0,int pip1,int Pid) {
     char buff[1000];
     int ch,i=0,j,KILL=0;
     write(pip1,"\n",1);
     while((ch=GetLine(pip0,buff)) ) {
         if(ch< 0) {continue;}
         if(SearchString(buff,(char *)"(EE) Server")>=0) {
           KILL=1;
         }
         if(SearchString(buff,(char *)"Killed Server")>=0) {
           break;
         }
     }
     return 1;
}
int WaitForProcess(int pip0,int pip1,int Pid) {
     char buff[1000],device[20],connection[20],reso[30];
     int ch,i=0,j;
     write(pip1,"\n",1);
     while((ch=GetLine(pip0,buff)) ) {
         if(ch< 0) continue;
     }
     return 1;
}
int runjob(char *job,int (*ProcessOut)(int,int,int)){
   FILE *fp,*fp1;
   int pip[2],pid,status,pip2[2];
   char *args[100],buff[1000],pt[300];
   char *pgrpath=NULL;
   int i=0,pos=0;
//   printf("JOB: %s\n",job);
   if(job==NULL) return 0;
   if( pipe(pip) < 0) return 0;
   if( pipe(pip2) < 0) return 0;
//   pipew =pip2[1];
   while(job[i]==' ') i++;
   strcpy(buff,job+i);
   i=0;
   while ( sscanf(buff+pos,"%s",pt) > 0 ) {
     if(pt[0]=='\"') {
      pos++;
      args[i]=buff+pos;
      while(buff[pos]!='\"')pos++;
      buff[pos]='\0';
      i++;
     }
#if 0
     if(pt[0]=='$') {
      pos++;
      args[i]=buff+pos;
      while(buff[pos]!='$')pos++;
      buff[pos]='\0';
      i++;
     }
#endif
#if 1
     if(pt[0]=='\\') {
      pos++;
      args[i]=buff+pos;
      while(buff[pos]!='\\')pos++;
      buff[pos]='\0';
      i++;
     }
#endif
     else {
       args[i]=buff+pos;
       pos +=strlen(pt);
       i++;
       if(buff[pos]< ' ') break;
       buff[pos]='\0';
     }
     pos++;
     while(buff[pos]==' ') pos++;
   }
   args[i]=NULL;
   if(i==0) return 0;
   pgrpath=kgWhich(args[0]);
   if (pgrpath==NULL) return 0;
   pid = fork();
   if(pid == 0) { /* child process */
//     if(fork()!=0) exit(0); /* to avoid zombie */
     close(0);
     dup(pip2[0]);
     close(pip2[0]);
     close(1);
     dup(pip[1]);
     close(2);
#if 1
     open("/dev/null",O_WRONLY|O_CREAT,0777);
#else
     dup(pip[1]);
#endif
     close(pip[1]);
     execv(pgrpath,args);
     fprintf(stderr,"Failed to execute \n");
     sleep(5);
     exit(1);
   }
   else {   /* parent process */
     int ret =0;
     close(pip2[0]);
     close(pip[1]);
     if(ProcessOut != NULL)ret=ProcessOut(pip[0],pip2[1],pid);
     WAIT(pid);
     free(pgrpath);
     return ret;
   }
}
int changejob(char *job){
   FILE *fp,*fp1;
   int pip[2],pid,status,pip2[2];
   char *args[100],buff[1000],pt[300];
   char *pgrpath=NULL;
   int i=0,pos=0;
   if(job==NULL) return 0;
   while(job[i]==' ') i++;
   strcpy(buff,job+i);
   i=0;
   while ( sscanf(buff+pos,"%s",pt) > 0 ) {
     if(pt[0]=='\"') {
      pos++;
      args[i]=buff+pos;
      while(buff[pos]!='\"')pos++;
      buff[pos]='\0';
      i++;
     }
     if(pt[0]=='\\') {
      pos++;
      args[i]=buff+pos;
      while(buff[pos]!='\\')pos++;
      buff[pos]='\0';
      i++;
     }
     else {
       args[i]=buff+pos;
       pos +=strlen(pt);
       i++;
       if(buff[pos]< ' ') break;
       buff[pos]='\0';
     }
     pos++;
     while(buff[pos]==' ') pos++;
   }
   args[i]=NULL;
   if(i==0) return 0;
   pgrpath=kgWhich(args[0]);
   if (pgrpath==NULL) return 0;
   execv(pgrpath,args);
   fprintf(stderr,"Failed to execute \n");
   sleep(5);
   exit(1);
}
