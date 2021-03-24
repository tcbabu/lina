#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#include <X11/Xlib.h>
#include <termio.h>
#include <grp.h>
#include <math.h>

#include "kulina.h"
#include "passwdfile.h"
#include "headers.h"
#include "linaCallbacks.h"
#include "images.c"

LINACONFIG lc;
int TextBox;
int xm,ym,xl,yl;
int Loop=1;
int Xres=0,Yres=0;
void *Img=NULL,*Img1=NULL;
int kgRootImage(void *);
int Session=2,Action=1,PowerDown=1,Sid=-1,TextLogin=0;
int Index=0;
char Guest[]="guest";
char *LoginId,*Epasswd,Passwd[50],*UserHome,*UserName;
Display *Dsp;
char TTY[100];

//int red=0,green=150,blue=230,bred=50,bgreen=200,bblue=250;
//int red=0,green=150,blue=230;
int red=50,green=100,blue=170;
float fac = 0.2;
static struct termios tty;
int CheckXserver(void);
int Wireless(int);
int WC=0;
int MakekeybrdGroup(DIALOG *D,void *arg,int Red,int Green,int Blue,int Xo,int Yo) ;

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
     if(ierr== -1) printf("ERROR IN  IOCTL\n");
     printf("TTY: %s\n",TTY);
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
   printf("ERROR IN IOCTL\n");
 close(op);
 }
 return;
}

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
   printf("Got Signal ;%d\n",sig);
   fflush(stdout);
}
void User2Signal(int sig) {
   printf("Got Signal;%d\n",sig);
//   Loop=1;
   fflush(stdout);
}
void HangupSignal(int sig) {
   printf("Got Hangup;%d\n",sig);
   fflush(stdout);
   kill(Sid,sig);
}
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
#if 0
     while((ch=GetLine(pip0,buff)) ) {
         if(ch< 0) {
           if(!BRK) continue;
           else break;
         }
         printf("%s\n",buff);
         if(SearchString(buff,(char *)"extension")>=0) {
           break;
         }
         if(SearchString(buff,(char *)"Fatal server error:")>=0) {
           ret=0;
           break;
         }
     }
#endif
     printf("Returning\n");
     return ret;
}
int ProcessStop(int pip0,int pip1,int Pid) {
     char buff[1000];
     int ch,i=0,j,KILL=0;
     write(pip1,"\n",1);
     while((ch=GetLine(pip0,buff)) ) {
         if(ch< 0) {continue;}
          printf("%s\n",buff);
         if(SearchString(buff,(char *)"(EE) Server")>=0) {
           KILL=1;
         }
         if(SearchString(buff,(char *)"Killed Server")>=0) {
           break;
         }
     }
//     if(!KILL) killpg(Sid,SIGKILL);
     printf("Returning\n");
     return 1;
}
int WaitForProcess(int pip0,int pip1,int Pid) {
     char buff[1000],device[20],connection[20],reso[30];
     int ch,i=0,j;
     write(pip1,"\n",1);
     while((ch=GetLine(pip0,buff)) ) {
         if(ch< 0) continue;
//          printf("%s\n",buff);
     }
//     printf("Okay Back\n");
     return 1;
}
int runjob(char *job,int (*ProcessOut)(int,int,int)){
   FILE *fp,*fp1;
   int pip[2],pid,status,pip2[2];
   char *args[100],buff[1000],pt[300];
   char *pgrpath=NULL;
   int i=0,pos=0;
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
//     printf("Waiting pid: %d\n",pid);
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
int StartServer(void){
   FILE *fp,*fp1;
   int pip[2],pid,status,pip2[2],argc,cid;
   char *args[100],buff[1000],pt[300],job[200];
   char *pgrpath=NULL;
   int i=0,pos=0;
   if(CheckXserver()) return;
   Sid=-1;
   sprintf(job,"Xorg :0.0 vt7 -quiet -noreset -nopn -background none");
//   sprintf(job,"Xorg  ");
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
   argc=i;
   pgrpath=kgWhich(args[0]);
   if (pgrpath==NULL) return 0;
   pid=getpid();
   cid = fork();
   if(cid == 0) { /* child process */
//     if(fork()!=0) exit(0); /* to avoid zombie */
     close(0);
     dup(pip2[0]);
     close(pip2[0]);
     close(1);
     open("/dev/null",O_WRONLY|O_CREAT,0777);
     close(2);
     dup(pip[1]);
     close(pip[1]);
     signal(SIGUSR1,SIG_IGN);
     setpgid(0,getpid());
//     kill(pid,SIGUSR1);
     fprintf(stderr,"Starting Server\n");
     execv(pgrpath,args);
     fprintf(stderr,"Failed to execute \n");
     exit(1);
   }
   else {   /* parent process */
     close(pip2[0]);
     close(pip[1]);
     Sid=cid;
     if(!ProcessXOut(pip[0],pip2[1],cid)) cid=0;
     pause();
     if(pgrpath!= NULL) free(pgrpath);
//     pause();
     fprintf(stderr,"Started Server...\n");
     return cid;
   }
}
int CleanServer(void){

     killpg(Sid,SIGHUP);
     return 1;
}
int StopServer(void){
#if 0
   FILE *fp,*fp1;
   int pip[2],pid,status,pip2[2],argc,cid;
   char *args[100],buff[1000],pt[300],job[200];
   char *pgrpath=NULL;
   int i=0,pos=0;
   if( pipe(pip) < 0) return 0;
   if( pipe(pip2) < 0) return 0;
   cid = fork();
   if(cid == 0) { /* child process */
//     if(fork()!=0) exit(0); /* to avoid zombie */
     close(0);
     dup(pip2[0]);
     close(pip2[0]);
     close(1);
     dup(pip[1]);
     close(2);
     dup(pip[1]);
     close(pip[1]);
     killpg(Sid,SIGHUP);
     killpg(Sid,SIGTERM);
     printf("Waiting for pid: %d\n",Sid);
     WAIT(Sid);
     remove("/tmp/.X0-lock");
     fprintf(stderr,"Killed Server\n");
     exit(1);
   }
   else {   /* parent process */
     close(pip2[0]);
     close(pip[1]);
     ProcessStop(pip[0],pip2[1],cid);
     WAIT(cid);
     Sid=-1;
     return pid;
   }
#else
     int status;
     killpg(Sid,SIGHUP);
     killpg(Sid,SIGTERM);
     printf("Waiting for pid: %d\n",Sid);
     WAIT(Sid);
     remove("/tmp/.X0-lock");
     return 1;
#endif
}
void ModifylinaGc(Gclr *gc) {
/*
//  You may change default settings here 
//  probably you can allow the user to create a config in $HOME
//  and try to read that file (if exits); so dynamic configuration is possible
   gc->FontSize =8;
   gc->Font=23;
*/
   int r,g,b;
   r = lc.FontRed;
   g= lc.FontGreen;
   b = lc.FontBlue;
   r %=256;
   g %=256;
   b %=256;
   kgDefineColor(101,r,g,b);
   r = lc.HighRed;
   g= lc.HighGreen;
   b = lc.HighBlue;
   r %=256;
   g %=256;
   b %=256;
   kgDefineColor(102,r,g,b);
   gc->menu_char = 101;
   gc->but_char  =101;
   gc->txt_char  =101;
   gc->txt_pchar  =101;
   gc->info_char  =101;
   gc->msg_char  =101;
   gc->char_clr = 101;
   gc->cur_clr = 102;
   gc->ItemHighColor = 102;
   gc->MsgFont=8;
   gc->MenuFont=8;
   gc->PromptFont=8;
   gc->ButtonFont=8;
   gc->GuiFontSize=10;

}
void * kgSessionImage(int size,int red,int green,int blue) {
   void *fid,*img=NULL;
   int count,count1;
   int r,g,b,np=120,i,sign=1;
   double Dang,dang,ang,ro,ss,cs,dl,cl,cr,size1;
   float R,ri;
   count = 2*np;
   count1= count-1;
   Dang = 300.0/(np-1);
   dang = Dang*3.14159265/180.0;
   R = size;
     
   fid = kgInitImage(size,size,4);
   kgUserFrame(fid,0.0,0.0,+R,+R);
   size *=0.6;
   size1= R*0.2;
   if(size1 > 3 ) size1=3;
   if(size1 < 2) size1=2;
   ri= size;
   ro = ri+size1;
   kgChangeColor(fid,1001,red,green,blue);
   kgRoundedRectangle(fid,R*0.5,R*0.6,ri+size1,ri,1001,0.2,0.08*R);
   kgRoundedRectangleFill(fid,R*0.5,R*0.2,ri+3.*size1,size1*2,0,1001,0.1);
   img = kgGetResizedImage(fid);
   kgCloseImage(fid);
   return img;
}
void * kgKeyboardImage(int size,int red,int green,int blue) {
   void *fid,*img=NULL;
   int count,count1;
   int r,g,b,np=120,i,sign=1;
   double Dang,dang,ang,ro,ss,cs,dl,cl,cr,size1;
   float R,ri;
   count = 2*np;
   count1= count-1;
   Dang = 300.0/(np-1);
   dang = Dang*3.14159265/180.0;
   R = size;
     
   fid = kgInitImage(size,size,4);
   kgUserFrame(fid,0.0,0.0,+R,+R);
   size *=0.4;
   size1= R*0.5;
   if(size1 > 3 ) size1=3;
   if(size1 < 2) size1=2;
   ri= size;
   ro = ri*1.5;
   kgChangeColor(fid,1001,red,green,blue);
   kgRoundedRectangle(fid,R*0.5,R*0.5,R*0.9,ri,1001,0.2,0.08*R);
   kgRoundedRectangleFill(fid,R*0.2,R*0.5,R*0.1,R*0.1,0,1001,0.1);
   kgRoundedRectangleFill(fid,R*0.4,R*0.5,R*0.1,R*0.1,0,1001,0.1);
   kgRoundedRectangleFill(fid,R*0.6,R*0.5,R*0.1,R*0.1,0,1001,0.1);
   kgRoundedRectangleFill(fid,R*0.8,R*0.5,R*0.1,R*0.1,0,1001,0.1);
   kgRoundedRectangleFill(fid,R*0.5,R*0.35,R*0.9,size1*2,0,1001,0.1);
   img = kgGetResizedImage(fid);
   kgCloseImage(fid);
   return img;
}
int ArcFill(void *fid,float xo,float yo,float r,float dr,float ang1,float ang2,int flag ,int color)
  {
    double a1,a2,a3,rd,th,t;
    int n,i,j,full=0;
    float *x,*y;
    if( fabs((double)(ang1-ang2))> 359.0) full=1;
    a1 = ang1*3.14159265/180.0;
    a2 = ang2*3.14159265/180.0;
    th= 2.0*3.14159265/180.0;
    n = fabs(a2-a1)/th+0.5;
    x = malloc(sizeof(float)*2*(n+2));
    y = malloc(sizeof(float)*2*(n+2));
    if(y==NULL) {
      printf("Error: In mem allocation..\n");
      exit(0);
    }
    th = (a2-a1)/n;
    t =a1;
    i=0;
    *(x+i)= r*cos(t)+xo;
    *(y+i) = r*sin(t)+yo;
    for(i=1;i<n;i++){
      t +=th;
      *(x+i)= r*cos(t)+xo;
      *(y+i) = r*sin(t)+yo;
    }
    t =a2;
    j=n;
    if(!full) {
      *(x+j)= r*cos(t)+xo;
      *(y+j) = r*sin(t)+yo;
       j++;
      r -=dr;
      *(x+j)= r*cos(t)+xo;
      *(y+j) = r*sin(t)+yo;
      for(i=1;i<n;i++){
        j++;
        t -=th;
        *(x+j)= r*cos(t)+xo;
        *(y+j) = r*sin(t)+yo;
      }
      j++;
      t =a1;
      *(x+j)= r*cos(t)+xo;
      *(y+j) = r*sin(t)+yo;
      j++;
    }
    kgPolyFill(fid,j,x,y,flag,color);
    free(x),free(y);
    return(j);
  }
void * kgWirelessImage(int size,int red,int green,int blue) {
   void *fid,*img=NULL;
   int count,count1;
   int r,g,b,np=120,i,sign=1;
   double Dang,dang,ang,ro,ss,cs,dl,cl,cr,size1;
   float R,ri;
   float Red,Green,Blue,H,S,V;
   count = 2*np;
   count1= count-1;
   Dang = 300.0/(np-1);
   dang = Dang*3.14159265/180.0;
   R = size;
     
   fid = kgInitImage(size,size,4);
   kgUserFrame(fid,0.0,0.0,+R,+R);
   size *=0.7;
   size1= size*0.1;
   if(size1 > 3 ) size1=3;
   if(size1 < 2) size1=2;
   ri= size;
   ro = ri+size1;
   kgChangeColor(fid,1001,red,green,blue);
   RGBtoHSV((float)red,(float)green,(float)blue,&H,&S,&V);
   V = 0.4*V;
   HSVtoRGB(&Red,&Green,&Blue,H,S,V);
   kgChangeColor(fid,1000,(int)Red,(int)Green,(int)Blue);
   ang =3.14159265*0.5+3.14159265*30/180;
   ArcFill(fid,R*0.5,R*0.2,ri,size1*1.25,135.0,45.,0,1001);
   ri -=size1;
   ri -=(size1*1.5);
   ArcFill(fid,R*0.5,R*0.2,ri,size1*1.25,135.0,45.,0,1001);
   ri -=size1;
   ri -=(size1*1.5);
   ArcFill(fid,R*0.5,R*0.2,ri,size1*1.25,135.0,45.,0,1001);
   ri -=size1;
   ri -=(size1*1.5);
   kgArcFill(fid,R*0.5,R*0.2,ri,135.0,45.,0,1001);
   img = kgGetResizedImage(fid);
   kgCloseImage(fid);
   return img;
}
int linaGroup( DIALOG *D,void **v,void *pt) {
  int GrpId=0,oitems=0,i,j;
  DIA *d=NULL,*dtmp;
  void **imgs;
  imgs =(void **)pt;
  char *xpm0;// Pixmap info
//  xpm0 = (char *)malloc(40);
//  strcpy(xpm0, (char *)"##/root/lina/WRK/thara.png");
  xpm0 = imgs[0];
  DIP p0 = { 
    'p',
    349,198,  
    765,354,  
    (void *)xpm0,
    12, /* bkgr colour */ 
    0,0 /* border hide */ 
    ,0.5 // transparency
  };
  p0.transparency = lc.Transparency;
  strcpy(p0.Wid,(char *)"linaWidget1");
  T_ELMT *e1  ; 
  e1 =(T_ELMT *)malloc(sizeof(T_ELMT)*1);
  e1[0].fmt = (char *)malloc(13);
  strcpy(e1[0].fmt,(char *)"Password%12s");
  e1[0].v=(void *)v[0];
  e1[0].sw=1;
  e1[0].noecho=1;
  e1[0].img=NULL;
  DIT t1 = { 
    't',
    415,268,  
    645,303,
    24, 
    1,1, 
    e1,
    1,1,
    NULL,linatextbox1callback,0,0,18,9 /* args,Call back */
  };
  t1.type = 1;
  strcpy(t1.Wid,(char *)"linaWidget3");
  t1.pt=NULL;
  DIM m2 = { 
    'm',
    480,231,  
    700,266,  
    -1,0  
  };
  strncpy(m2.msg,(char *)"!z21!f01",499);
  strcat(m2.msg,GetRealName(&lc,Index));
  strcpy(m2.Wid,(char *)"UserMsg");
  char *xpm3;// Pixmap info
//  xpm3 = (char *)malloc(3);
//  strcpy(xpm3, (char *)"##");
  DIP p3 = { 
    'p',
    430,220,  
//    450,240,  
//    500,290,  
    480,270,  
    (void *)xpm3,
    -1, /* bkgr colour */ 
      0,0 /* border hide */ 
     ,0.0
  };
  p3.xpm = (void *)GetUserImage(&lc,Index);
  strcpy(p3.Wid,(char *)"UserPic");
  BUT_STR  *butn4=NULL; 
  butn4= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn4[0].sw=1;
  strcpy(butn4[0].title,(char *)"");
//  xpm0 = (char *)malloc(13);
//  strcpy(xpm0,"##login.png");
  butn4[0].xpmn=(void *)&login_str;
  butn4[0].xpmp=NULL;
  butn4[0].xpmh=NULL;
  butn4[0].bkgr=-200200200;
  butn4[0].butncode='';
  DIL h4 = { 
    'h',
    645,263,  
    675,313,
    2,4,  
    28, 
    42, 
    1,1, 
    0,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn4, 
    linasplbutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(h4.Wid,(char *)"linaWidget6");
  BUT_STR  *butn5=NULL; 
  butn5= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn5[0].sw=1;
//  strcpy(butn5[0].title,(char *)"Switch User");
  strcpy(butn5[0].title,(char *)" ");
  butn5[0].xpmn=NULL;
  butn5[0].xpmp=NULL;
  butn5[0].xpmh=NULL;
  butn5[0].bkgr=-1;
  butn5[0].butncode='';
  DIN b5 = { 
    'n',
//    488,236,  
//    718,278,
    480,231,  
    700,266,  
    2,2,  
    200, 
    24, 
    1,1, 
    10,0.2500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn5, 
    linabutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b5.Wid,(char *)"linaWidget8");
  BUT_STR  *butn6=NULL; 
  butn6= (BUT_STR *)malloc(sizeof(BUT_STR)*4);
  butn6[0].sw=1;
  strcpy(butn6[0].title,(char *)"");
  butn6[0].xpmn=imgs[1];
  butn6[0].xpmp=NULL;
  butn6[0].xpmh=NULL;
  butn6[0].bkgr=-1;
  butn6[0].butncode='';
  butn6[1].sw=1;
  strcpy(butn6[1].title,(char *)"");
  butn6[1].xpmn=imgs[2];
  butn6[1].xpmp=NULL;
  butn6[1].xpmh=NULL;
//  butn6[1].xpmn= (char *)malloc(100); 
//  strcpy(butn6[1].xpmn,(char *)"##stop.png");
//  butn6[1].xpmn= (void *)kgPowerdownImage(48,220,220,210);
  butn6[1].bkgr=-1;
  butn6[1].butncode='';
  butn6[2].sw=1;
  strcpy(butn6[2].title,(char *)"");
  butn6[2].xpmn=imgs[3];
  butn6[2].xpmp=NULL;
  butn6[2].xpmh=NULL;
//  butn6[1].xpmn= (char *)malloc(100); 
//  strcpy(butn6[1].xpmn,(char *)"##stop.png");
//  butn6[2].xpmn= (void *)&wireless1_str;
  butn6[2].bkgr=-1;
  butn6[2].butncode='';
  butn6[3].sw=1;
  strcpy(butn6[3].title,(char *)"");
  butn6[3].xpmn=imgs[4];
  butn6[3].xpmp=NULL;
  butn6[3].xpmh=NULL;
  butn6[3].bkgr=-1;
  butn6[3].butncode='';
  DIL h6 = { 
    'h',
    1142,590,  
    1460,636,
    30,10,  
    44, 
    44, 
    4,1, 
    0,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn6, 
    linasplbutton2callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(h6.Wid,(char *)"linaWidget9");
  char *xpm7;// Pixmap info
  xpm7 = (char *)DateTimeImg(&lc,300,100);
  DIP p7 = { 
    'p',
    1000,20,  
    1300,120,  
    (void *)xpm7,
    12, /* bkgr colour */ 
      0,0 /* border hide */ 
     ,0.3
  };
  if(lc.ShowTime == 0 ) p7.hide=1;
  strcpy(p7.Wid,(char *)"linaWidget8");
  dtmp = D->d;
  i=0;
  if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
  dtmp = (DIA *)realloc(dtmp,sizeof(DIA )*(i+9));
  d =dtmp+i; 
  d[8].t=NULL;
  d[0].t = (DIT *)malloc(sizeof(DIP));
  *d[0].p = p0;
  d[0].p->item = -1;
  d[1].t = (DIT *)malloc(sizeof(DIT));
  *d[1].t = t1;
  d[1].t->item = -1;
  d[2].t = (DIT *)malloc(sizeof(DIM));
  *d[2].m = m2;
  d[2].m->item = -1;
  d[3].t = (DIT *)malloc(sizeof(DIP));
  *d[3].p = p3;
  d[3].p->item = -1;
  d[4].t = (DIT *)malloc(sizeof(DIL));
  linasplbutton1init(&h4,pt) ;
  *d[4].h = h4;
  d[4].h->item = -1;
  d[5].t = (DIT *)malloc(sizeof(DIN));
  linabutton1init(&b5,pt) ;
  *d[5].N = b5;
  d[5].N->item = -1;
  d[6].t = (DIT *)malloc(sizeof(DIL));
  linasplbutton2init(&h6,pt) ;
  *d[6].h = h6;
  d[6].h->item = -1;
  d[7].t = (DIT *)malloc(sizeof(DIP));
  *d[7].p = p7;
  d[7].p->item = -1;
  d[8].t = NULL;
  GrpId=kgOpenGrp(D);
  D->d = dtmp;
  j=0;
  while(d[j].t!=NULL){ kgAddtoGrp(D,GrpId,(void *)(d[j].t));j++;}
  return GrpId;
} 

/* One can also use the following code to add Widgets to an existing Dialog */

int MakelinaGroup(DIALOG *D,void *arg) {
   int GrpId;
   WIDGETGRP *Gpt;
/*************************************************

    Text_Box1  1 data values

*************************************************/
   char  *v0 ;
   v0 = (char *)malloc(sizeof(100));
   v0[0] = '\0';
   void** v=(void **)malloc(sizeof(void*)*2);
   v[1]=NULL;
   v[0]=(void *)(v0);
   void *pt=NULL; /* pointer to send any extra information */
   GrpId = linaGroup(D,v,pt);
   Gpt = kgGetWidgetGrp(D,GrpId);
   Gpt->arg= v;
   return GrpId;
}

int lina( void *parent,void **v,void *pt) {
  int ret=1,GrpId,k,gid,bgid;
  float H,S,V,R,G,B;
  void **imgs;
  DIALOG D;
  DIA *d=NULL;
  D.VerId=1401010200;
  kgInitUi(&D);
  D.d=NULL;
  GrpId = linaGroup(&D,v,pt);
  d = D.d;
  D.d = d;
  TextBox=1;
  kgColorTheme(&D,(int)lc.Red,(int)lc.Green,(int)lc.Blue);
  ModifylinaGc(&(D.gc));    /*  set colors for gui*/
  d = D.d;
  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 0;
  D.df = 6;
  D.tw = 4;
  D.bw = 4;
  D.lw = 4;
  D.rw = 4;
  D.xo = 10;   /* Position of Dialog */ 
  D.yo = 10;
  D.xl = 1307;    /*  Length of Dialog */
  D.yl = 624;    /*  Width  of Dialog */
  D.Initfun = linainit;    /*   init fuction for Dialog */
  D.Cleanupfun = linacleanup;    /*   init fuction for Dialog */
  D.kbattn = 0;    /*  1 for drawing keyborad attention */
  D.butattn = 0;    /*  1 for drawing button attention */
  D.fullscreen = 1;    /*  1 for for fullscreen mode */
  D.Deco = 0;    /*  1 for Window Decorration */
  D.transparency = 1.0;    /*  float 1.0 for full transparency */
  D.Newwin = 0;    /*  1 for new window not yet implemented */
  D.NoWinMngr=1;
  D.DrawBkgr = 1;    /*  1 for drawing background */
//  D.Bkpixmap = "##/etc/xdg/lina/background.jpg";    /*  background image */
  D.Bkpixmap = Img;
  
  D.Sticky = 0;    /*  1 for stickyness */
  D.Resize = 0;    /*  1 for Resize option */
  D.MinWidth = 100;    /*   for Resize option */
  D.MinHeight = 100;    /*   for Resize option */
#if 1 
  D.Callback = linaCallBack;    /*  default callback */
#else 
  D.Callback = NULL;    
#endif
  D.ResizeCallback = linaResizeCallBack;  /*  Resize callback */
  if(lc.ShowTime==0) D.WaitCallback = NULL;  /*  Wait callback */
  else D.WaitCallback = linaWaitCallBack;  /*  Wait callback */
  D.Fixpos = 1;    /*  1 for Fixing Position */
  D.NoTaskBar = 1;    /*  1 for not showing in task bar*/
  D.StackPos = 0;    /* -1,0,1 for for Stack Position -1:below 0:normal 1:above*/
  D.Shapexpm = NULL;    /*  PNG/jpeg file for window shape;Black color will not be drawn */
  D.parent = parent;    /*  1 for not showing in task bar*/
  D.pt = pt;    /*  any data to be passed by user*/
//  strcpy(D.name,"Kulina Designer ver 1.0");    /*  Dialog name you may change */
  if(D.fullscreen!=1) {    /*  if not fullscreen mode */
     int xres,yres; 
     kgDisplaySize(&xres,&yres); 
      // D.xo=D.yo=0; D.xl = xres-10; D.yl=yres-80;
  }
  else {    // for fullscreen
     int i,dx,dy,xl,yl,xm,ym;
     int xres,yres; 
     DIT *t;
     kgDisplaySize(&xres,&yres); 
     Xres=xres; Yres=yres;
     D.xo=D.yo=0; D.xl = xres; D.yl=yres;
     xm = xres/2;
     ym = yres/2;
     if (yres < 770) ym = yres - 385; //TCB
     t =d[0].t;
     xl = t->x2-t->x1+1;
     yl = t->y2-t->y1+1;
     xl *=0.5;
     yl *=0.5;
     dx = xm -xl -t->x1;
     dy = ym -yl -t->y1;
     for(i=0;i<6;i++) {
       t =d[i].t;
       t->x1+=dx; t->x2+=dx;
       t->y1+=dy; t->y2+=dy;
     }
     t = d[6].t;
     xl = t->x2 -t->x1;
     yl = t->y2 -t->y1;
     t->x1 = xm -xl/2;
     t->y1 = yres -50;
     t->x2 = t->x1 + xl;
     t->y2 = t->y1 + yl;
     t = d[7].t;
     xl = t->x2 -t->x1;
     yl = t->y2 -t->y1;
     t->x1 = xres -xl-40;
     t->y1 = 40;
     t->x2 = t->x1 + xl;
     t->y2 = t->y1 + yl;
//     kgShiftGrp(&D,lc.KbGrp,(xres-840)/2-5,yres-305);
     lc.KbGrp=MakekeybrdGroup(&D,&TextBox,(int)lc.Red,(int)lc.Green,(int)lc.Blue,(xres-840)/2,yres-305);
      lc.KbVis=0;
      kgSetGrpVisibility((void *)&D,lc.KbGrp,lc.KbVis);
//     D.StackPos = 1; // you may need it
  }    /*  end of fullscreen mode */
  printf("Calling Ui\n");
  ret= kgUi(&D);
  kgFreeImage(Img);
  Img=NULL;
  kgCleanUi(&D);
  printf("Lina return\n");
  fflush(stdout);
  return ret;
}
#if 0
void *Runlina(void *arg) {
/*************************************************

    Text_Box1  1 data values

*************************************************/
   char  v0[600]=" ";
   void* v[1];
   v[0]=(void *)(v0);
   void *pt=NULL; /* pointer to send any extra information */
   lina(NULL,v,pt );
   return NULL;
}
#else
int CheckDisplay(void) {
   int i=0,j=0,status;
#if 0
   Display *Dsp;
   while( (Dsp=XOpenDisplay(NULL))==NULL) { 
       printf("Failed to Open\n");
       fflush(stdout);
       if(i > 5){
         j++;
         if(j > 4) kill(Sid,SIGKILL);
         StopServer();
         if(!StartServer()) printf("Failed to start server\n");
         i=0;
         continue;
       }
       usleep(10000);
       i++;
   }
   printf("Opened Display\n");
   fflush(stdout);
   XCloseDisplay(Dsp);
   printf("Closed Display\n");
   fflush(stdout);
#endif
   if(Xres==0) kgDisplaySize(&Xres,&Yres); 
   if(Img == NULL) {
     Img = kgGetImage(lc.Bkgr);
     if(Img==NULL) {
       void *fid;
       float R,G,B,H,S,V;
       fid = kgInitImage(Xres,Yres,1);
       kgUserFrame(fid,0.,0.,(float)Xres+1.,(float)Yres+1.);
       RGBtoHSV((float)lc.Red,(float)lc.Green,(float)lc.Blue,&H,&S,&V);
       V *=0.4;
       HSVtoRGB(&R,&G,&B,H,S,V);
       kgChangeColor(fid,10,(int)R,(int)G,(int)B);
       kgRoundedRectangleFill(fid,Xres*0.5,Yres*0.5,(Xres+2.0),Yres+2.0,0,10,0.);
       Img=kgGetResizedImage(fid);
       kgCloseImage(fid);
     }
     Img1= kgChangeSizeImage(Img,Xres,Yres);
     kgFreeImage(Img);
     Img=Img1;
   }
   printf("Making Root Image     \n");
   fflush(stdout);
   kgRootImage(Img);
   printf("Root Image     \n");
   fflush(stdout);
   return Sid;
}
int GetData(int pip0,int pip1,int Pid) {
     char buff[1000],dummy[20];
     int ch,i=0,j;
     write(pip1,"\n",1);
     while((ch=GetLine(pip0,buff)) ) {
         if(ch< 0) continue;
         printf("%s\n",buff);
         if(SearchString(buff,"info:") >= 0){
            sscanf(buff,"%s%d%d%d%d",dummy,&Action,&Session,&PowerDown,&Index);
         }
     }
     return 1;
}
int logindia( int (*ProcessOut)(int,int,int)){
   FILE *fp,*fp1;
   int pip[2],pid,status,pip2[2];
   char *args[100],buff[1000],pt[300];
   int i=0,pos=0;
   int   v0 = 1;
   char  v1[500]="" ;
   char  v2[100]="" ;
   void* v[3];
   void *imgs[6];
#if 0
   v[0]=(void *)(&v0);
   v[1]=(void *)(v1);
   v[2]=(void *)(v2);
#else
   v[0]=(void *)(v1);
   v[1]=NULL;
#endif
   if( pipe(pip) < 0) return 0;
   if( pipe(pip2) < 0) return 0;
   pid = fork();
   if(pid == 0) { /* child process */
//     if(fork()!=0) exit(0); /* to avoid zombie */
     printf("Calling CheckDisplay\n");
     fflush(stdout);
     CheckDisplay();
     close(0);
     dup(pip2[0]);
     close(pip2[0]);
     close(1);
     dup(pip[1]);
     close(2);
     open("/dev/null",O_WRONLY|O_CREAT,0777);
     /*dup(pip[1]);*/
     close(pip[1]);
     imgs[0]= MakePixmapImage( 300,130,lc.Red,lc.Green,lc.Blue,lc.fac);
     imgs[1]= kgSessionImage(36,lc.ButRed,lc.ButGreen,lc.ButBlue);
     imgs[2]= (void *)kgPowerdownImage(36,lc.ButRed,lc.ButGreen,lc.ButBlue);
     imgs[3]= (void *)kgWirelessImage(36,lc.ButRed,lc.ButGreen,lc.ButBlue);
     imgs[4]= (void *)kgKeyboardImage(36,lc.ButRed,lc.ButGreen,lc.ButBlue);
     imgs[5]= NULL;
     lina(NULL,v,imgs );
//   Following print is a must; do not remove
     printf("info: %d %d %d %d \n",Action,Session,PowerDown,Index);
     exit(1);
   }
   else {   /* parent process */
     close(pip2[0]);
     close(pip[1]);
     if(ProcessOut != NULL)ProcessOut(pip[0],pip2[1],pid);
     WAIT(pid);
     return pid;
   }
}
int RunLoginDia(void) {
   int lid,status;
   logindia(GetData);
   return 1;
}
void RunSession(int Index,char *session) {
  char buff[200];
  int uid,gid;
  sprintf(buff,"bash -l -c \\\"%s\"\\ ",session);
  printf("PATH=%s\n",getenv("PATH"));
  printf("BASH:%s\n",buff);
  signal(SIGHUP,HangupSignal);
  setsid();
  setpgid(0,0); //0 means setting setting process groupid to current pid;
  uid = GetUid(&lc,Index);
  gid = GetGid(&lc,Index);
  printf("calling SetSupGroups\n");
  SetSupGroups(&lc,Index);

  setresgid(gid,gid,gid);
  setresuid(uid,uid,uid);
//  setgid(gid);
//  setuid(uid);
  chdir(GetHomeDir(&lc,Index));
  setenv("HOME",GetHomeDir(&lc,Index),1);
  setenv("SHELL",GetShell(&lc,Index),1);
  setenv("USER",GetRealName(&lc,Index),1);
  setenv("LOGNAME",GetRealName(&lc,Index),1);
  changejob(buff);
  fprintf(stderr,"Failed To Execute : %s\n",Session);
}
void RunSession_n(int Index,char *session) {
  char buff[200];
  sprintf(buff,"su -l -s /bin/bash -c %s %s",session,GetUserId(&lc,Index));
  signal(SIGHUP,HangupSignal);
#if 0
  setsid();
  setgid(GetGid(&lc,Index));
  setuid(GetUid(&lc,Index));
  chdir(GetHomeDir(&lc,Index));
  setenv("HOME",GetHomeDir(&lc,Index),1);
  setenv("SHELL",GetShell(&lc,Index),1);
#endif
  changejob(buff);
  fprintf(stderr,"Failed To Execute : %s\n",Session);
}

int KillOtherJobs(char *term) {
   char buff[300];
   int Id=0,Okay=0,count=0;
   char tty[50],dummy[50],program[100];
   FILE *pp;
   while(!Okay) {
     count++;
     if(count== 5) {
       if ((Id=fork())==0) {
          system("stty sane");
          changejob("agetty -c -t 20 tty1");
       }
       else return Id;
     }
     if(count==20) return -1;
     pp = popen("ps -e","r");
     if(pp==NULL) return 0;
     fgets(buff,299,pp);
     while( fgets(buff,299,pp) != NULL) {
        sscanf(buff,"%d%s%s%s",&Id,tty,dummy,program);
        if(strcmp(tty,term)==0) {
           if(strcmp("agetty",program)==0) {
             Okay=1;
             break;
           }
           else {
            kill(Id,SIGKILL);
           }
        }
     }
     sleep(1);
     pclose(pp);
   } //not Okay
   return Id;
}
int CheckXserver(void) {
   char buff[300];
   int Id=0,Okay=0,count=0;
   char tty[50],dummy[50],program[100];
   FILE *pp;
     pp = popen("ps -e","r");
     fgets(buff,299,pp);
     while( fgets(buff,299,pp) != NULL) {
        sscanf(buff,"%d%s%s%s",&Id,tty,dummy,program);
        if((strcmp("X",program)==0)||(strcmp("Xorg",program)==0)) {
             Okay=Id;
             break;
        }
     }
     pclose(pp);
   return Okay;
}
int CheckProcess(char *procname) {
   char buff[300];
   int Id=0,Okay=0,count=0,ln;
   char tty[50],dummy[50],program[100];
   FILE *pp;
     ln = strlen(procname);
     pp = popen("ps -e","r");
     fgets(buff,299,pp);
     while( fgets(buff,299,pp) != NULL) {
        sscanf(buff,"%d%s%s%s",&Id,tty,dummy,program);
        program[ln]='\0';
        if((strcmp(procname,program)==0)) {
             Okay=Id;
             break;
        }
     }
     pclose(pp);
   return Okay;
}
int CheckProcessCommand(char *procname) {
   char buff[300];
   int Id=0,Okay=0,count=0,ln;
   char tty[50],dummy[50],program[100];
   FILE *pp;
     ln = strlen(procname);
     pp = popen("ps -elf","r");
     fgets(buff,299,pp);
     while( fgets(buff,299,pp) != NULL) {
        if(SearchString(buff,procname)>=0 ) {
          printf("Command: %s\n",buff);
          Okay=1;
          break;
        }
     }
     pclose(pp);
   return Okay;
}

int WaitForAgetty(int JobId) {
   char buff[300];
   int Id=0,Okay=0,count=0,Nologin=0;
   char tty[50],dummy[50],program[100];
   FILE *pp;
   while(!Okay) {
   Okay=1;
   pp = popen("ps -e","r");
   if(pp==NULL) return 0;
   fgets(buff,299,pp);
   while( fgets(buff,299,pp) != NULL) {
      sscanf(buff,"%d%s%s%s",&Id,tty,dummy,program);
      if(Id==JobId) {
             if(strcmp(program,"agetty")==0) {Nologin=1;count++;}
             else count=0;
             Okay=0;
             sleep(3);
             break;
      }
   }
   pclose(pp);
   if(count> 10) break;
   } //not Okay
   return Id;
}

int Runlina(void *arg) {
/*************************************************

    Selectmenu1  1 data value
    Text_Box1  2 data values

*************************************************/
   int pid,id,i,j,lid,cid,Parid,ierr;
   pid_t Mid;
   int status,Pip[2];
   char buff[200],*Command;
   void *pt=NULL; /* pointer to send any extra information */
   FILE *pp;
   remove("/tmp/.X0-lock");
   runjob("killall -9 X",WaitForProcess);
   runjob("killall -9 Xorg",WaitForProcess);
   signal(SIGUSR1,User1Signal);
   LoginId=Guest;
   UserName=Guest;
   InitConfig(&lc);
   Mid=fork();
   if((Mid)==0) {
   close(1);
   remove("/var/log/lina.log");
   id=open("/var/log/lina.log",O_CREAT|O_RDWR,0777);
   close(2);
   dup(id);
   if( pipe(Pip) < 0) return 0;
   while(Loop) {
     Passwd[0]='\0';
     TextLogin=0;
     CleanLinaConfig(&lc);
     ReadConfig(&lc);
     LoginId=lc.DefUser;
     Session = GetSessionFromName(&lc,lc.DefSession);
     if(Session==0) Session=1;
     if(UserName==NULL) {UserName=Guest;LoginId=Guest;}
     if(!StartServer()) printf("Failed to start server\n");
     printf("Started Server\n");
     fflush(stdout);
     cid=fork();
     if(cid == 0) {
       signal(SIGUSR1,SIG_DFL);
       if(Index==0) Index = GetIndexFromLogin(&lc,LoginId);
       if(Index==0) {
          Index=1; 
       }
       if(Index !=0) UserName=GetRealName(&lc,Index);
       Action=3;
       Parid=getppid();
       setpgid(cid,Sid);
       RunLoginDia();
       close(Pip[0]);
       switch(Action) {
         case 1:
           Command = GetSessionCommand(&lc,Session);
           if(Command==NULL) {TextLogin=1;Loop=0;PowerDown=0;}
           sprintf(buff,"%d %d %d %d %d %d  \n",Action,Session,PowerDown,Loop,TextLogin,Index);
           write(Pip[1],buff,100);
           if(!TextLogin) RunSession(Index,Command);
         break;
         case 2:
           Loop=0;
           sprintf(buff,"%d %d %d %d %d %d  \n",Action,Session,PowerDown,Loop,TextLogin,Index);
           write(Pip[1],buff,100);
         break;
         default:
          sprintf(buff,"%d %d %d %d %d %d  \n",Action,Session,PowerDown,Loop,TextLogin,Index);
          write(Pip[1],buff,100);
         break;
       }
       printf("Child Loop: %x\n",Loop);
       exit(Loop);
     }
     else {
       read(Pip[0],buff,100);
       sscanf(buff,"%d%d%d%d%d%d",&Action,&Session,&PowerDown,&Loop,&TextLogin,&Index);
       printf("%d %d %d %d %d %d  \n",Action,Session,PowerDown,Loop,TextLogin,Index);
       WAIT(cid);
     }
#if 0
     if(TextLogin) StopServer();
     else CleanServer();
#else
     StopServer();
#endif

   } // while loop
   if(!TextLogin) {
     switch(PowerDown) {
          default:
          exit(0);
          break;
          case 1:
#if 1
          if(CheckProcess("plymouthd") ) {
//            runjob("systemctl start plymouth-halt",NULL);
            runjob("plymouth --quit",NULL);
          }
#endif
          if(CheckProcess("systemd") ) {
            runjob("systemctl start systemd-poweroff",NULL);
          }
          system("shutdown -h now");
          break;
          case 2:
#if 1
          if(CheckProcess("plymouthd") ) {
//            runjob("systemctl start plymouth-reboot",NULL);
            runjob("plymouth --quit",NULL);
          }
#endif
          if(CheckProcess("systemd") ) {
            runjob("systemctl start systemd-reboot",NULL);
          }
          system("shutdown -r now");
          break;
     }
   }
   else {
      if(CheckProcess("plymouthd") ) {
        runjob("plymouth --quit",NULL);
      }
   }
   exit(0); 
   } //if fork
   else {
   int Kid;
       WAIT(Mid);
//         printf("Calling change job\n");
         Mid = KillOtherJobs("tty1");
//         printf("Mid: %d\n",Mid);
         if(Mid> 0) WaitForAgetty(Mid) ;
         if(CheckProcess("systemd") ) {
           runjob("systemctl restart lina",NULL);
         }
   }
   return 1;
}
#endif
