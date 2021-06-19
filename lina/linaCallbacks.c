
#include <time.h>
#include  <sys/time.h>
#include "kulina.h"
#include "passwdfile.h"
extern LINACONFIG lc;
void *RunSessionDia(void *);
void *RunPdDia(void *arg);
void *RunUserDia(void *arg);
void *Runwiredia(void *arg);
int CheckProcess(char *);
int CheckString(char *s1,char *s2);
int SearchString(char *s1,char *s2);
int GetLine(int pip0,char *buff);
int MakeConfigFile(void);
int WaitForProcess(int pip0,int pip1,int Pid);
int runjob(char *job,int (*ProcessOut)(int,int,int));
int GetWdev(void);
int Wireless(int,void *);
int WirelessStatus(void);
int keybrdcleanup(void *Tmp,int Grpid);
extern char *Wdev;
extern int Session,Action,PowerDown,TextLogin;
extern char *LoginId,*Epasswd,Passwd[50],*UserHome,*UserName;
extern int Index;
extern int Xres,Yres;
static char *Command;
extern int WC;
extern int Connected;
int  linatextbox1callback(int cellno,int i,void *Tmp) {
  /************************************************* 
   cellno: current cell counted along column strting with 0 
           ie 0 to (nx*ny-1) 
   i     : widget id starting from 0 
   Tmp   : Pointer to DIALOG 
   *************************************************/ 
  DIALOG *D;DIT *T;T_ELMT *e; 
  int ret=1;
  D = (DIALOG *)Tmp;
  T = (DIT *)kgGetWidget(Tmp,i);
  e = T->elmt;
  strcpy(Passwd,kgGetString(T,cellno));
  Action=1;
  ret = CheckLogin(&lc,Index,Passwd);
  if (!ret) {
    kgSplashMessage( Tmp,(Xres-300)/2,(Yres-50)/2,300,50,"Invalid login...\\nPress 'Esc' to clean input box",21,101,-1) ;
  }
  else kgSetExit(D);
  return ret;
}
int  linasplbutton1callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIL *B; 
  int n,ret=1; 
  D = (DIALOG *)Tmp;
  B = (DIL *) kgGetWidget(Tmp,i);
  n = B->nx;
  strcpy(Passwd,kgGetString((DIT *)kgGetWidget(Tmp,1),0));
  Action=1;
  ret = CheckLogin(&lc,Index,Passwd);
  if (!ret) {
//    kgSplashMessage( Tmp,(Xres-150)/2,(Yres-30)/2,180,40,"Invalid login...",21,101,-1) ;
    kgSplashMessage( Tmp,(Xres-300)/2,(Yres-50)/2,300,50,"Invalid login...\\nPress 'Esc' to clean input box",21,101,-1) ;
  }
  switch(butno) {
    case 1: 
      break;
    default:
      break;
  }
  kgSetCurrentWidget(Tmp,1);
  return ret;
}
void  linasplbutton1init(DIL *B,void *pt) {
}
int  linabutton1callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  char buff[100];
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  RunUserDia(Tmp);
  UserName=GetRealName(&lc,Index);
  sprintf(buff,"!z21!f01%-s",UserName);
  kgUpdateWidget(kgGetWidget(Tmp,0));
  kgChangePixmapImage(kgGetWidget(Tmp,3),GetUserImage(&lc,Index));
  kgWrite(kgGetWidget(Tmp,2),buff);
  kgUpdateWidget(kgGetWidget(Tmp,1));
  kgUpdateWidget(kgGetWidget(Tmp,3));
  kgUpdateWidget(kgGetWidget(Tmp,4));
  kgUpdateWidget(kgGetWidget(Tmp,5));
  kgUpdateOn(D);
  switch(butno) {
    case 1: 
      break;
  }
  kgSetCurrentWidget(Tmp,1);
  return ret;
}
void  linabutton1init(DIN *B,void *pt) {
}
int  linasplbutton2callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIL *B; 
  int n,ret=0; 
  D = (DIALOG *)Tmp;
  B = (DIL *) kgGetWidget(Tmp,i);
  n = B->nx;
  Action=butno;
  switch(butno) {
    case 1: 
      RunSessionDia(Tmp);
      TextLogin=0;
      Command = GetSessionCommand(&lc,Session);
      if(Command==NULL) {TextLogin=1;}
      if(TextLogin==1) kgSetExit(Tmp);
      break;
    case 2: 
      RunPdDia(Tmp);
      ret=1;
      if(PowerDown==3) ret=0;
      break;
    case 3:
      WirelessStatus();
      WC = Connected;
      WC = (WC+1)%2;
      WC=Wireless(WC,Tmp);
      Connected=WC;
      break;
    case 4:
      lc.KbVis = (lc.KbVis+1)%2;
      kgSetGrpVisibility(Tmp,lc.KbGrp,lc.KbVis);
      kgUpdateGrp(Tmp,lc.KbGrp);
      break;
    default:
      break;
  }
  kgSetCurrentWidget(Tmp,1);
  return ret;
}
void  linasplbutton2init(DIL *B,void *pt) {
}
int linainit(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  /* you add any initialisation here */
  int ret = 1;
  char buff[200];
  DIALOG *D;void *pt;
  D = (DIALOG *)Tmp;
  pt = D->pt;
  D->df = 1;
  CheckHdmi();
  sprintf(buff,"amixer set Master unmute");
  runjob(buff,NULL);
  sprintf(buff,"amixer set Master %-d\%",100);
  runjob(buff,NULL);
  sprintf(buff,"amixer set Master playback %-d",65536);
  runjob(buff,NULL);
  sprintf(buff,"amixer set Speaker playback %-d",65536);
  runjob(buff,NULL);
  if(GetWdev()) {
   if(!CheckProcess("wpa_supplicant") ){
     MakeConfigFile();
     sprintf(buff,"wpa_supplicant -i%-s -c/tmp/wpa.conf -B",Wdev);
     runjob(buff,WaitForProcess);
//     runjob("wpa_cli scan",NULL);
   }
#if 0
   else {
     runjob("wpa_cli scan on",NULL);
   }
#endif
   WC = WirelessStatus();
   printf("%s : %d\n",Wdev,WC);
  }
  else printf("Could not get Wdev\n");
  return ret;
}
int linacleanup(void *Tmp) {
  /* you add any cleanup/mem free here */
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  int ret = 1,i;
  DIALOG *D;void *pt;
  void **imgs;
  D = (DIALOG *)Tmp;
  imgs = D->pt;
  i=0;
  while(imgs[i]!= NULL) {kgFreeImage(imgs[i]);imgs[i]=NULL; i++;}
  printf("Calling keybrdcleanup\n");
  keybrdcleanup(Tmp,lc.KbGrp);
  printf("Calleg keybrdcleanup\n");
  fflush(stdout);
  return ret;
}
int linaCallBack(void *Tmp,void *tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    tmp :  Pointer to KBEVENT  
   ***********************************/ 
  int ret = 0;
  void *Img=NULL;
  char buff[100];
  DIP *p3;
  DIM *M;
  DIALOG *D;
  KBEVENT *kbe;
  D = (DIALOG *)Tmp;
  kbe = (KBEVENT *)tmp;
  if(kbe->event ==1) {
    if(kbe->button ==1) {
     p3 = (DIP *)kgGetLocationWidget(Tmp,kbe->x,kbe->y);
//     if(p3==NULL) printf("P3--NULL\n");
     if((p3!=NULL) &&((p3->code=='p')||(p3->code=='m'))){
//     if((p3!=NULL) &&((p3->code=='p'))){
       M = (DIM *)p3;
       if((strcmp(p3->Wid,"UserPic")==0)||(strcmp(M->Wid,"UserMsg")==0)) {
//       if((strcmp(p3->Wid,"UserPic")==0)) {
         RunUserDia(Tmp);
         UserName=GetRealName(&lc,Index);
         sprintf(buff,"!z21!f01%-s",UserName);
         kgUpdateWidget(kgGetWidget(Tmp,0));
         kgChangePixmapImage(kgGetWidget(Tmp,3),GetUserImage(&lc,Index));
         kgWrite(kgGetWidget(Tmp,2),buff);
         kgUpdateWidget(kgGetWidget(Tmp,1));
         kgUpdateWidget(kgGetWidget(Tmp,3));
         kgUpdateWidget(kgGetWidget(Tmp,4));
         kgUpdateWidget(kgGetWidget(Tmp,5));
         kgUpdateOn(D);
         return ret;
       }
     }
    }
    if(kbe->button ==3) {
#if 1
      Img = kgGetRootImage();
      kgWriteImage(Img,"/root/lina.jpg");
      kgFreeImage(Img);
#endif
    }
  }
  return ret;
}
int linaResizeCallBack(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  int ret = 0;
  int xres,yres,dx,dy;
  DIALOG *D;
  D = (DIALOG *)Tmp;
  kgGetWindowSize(D,&xres,&yres);
  dx = xres - D->xl;
  dy = yres - D->yl;
  /* extra code */
  D->xl= xres;
  D->yl= yres;
  kgRedrawDialog(D);
  return ret;
}
int linaWaitCallBack(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    Called while waiting for event  
    return value 1 will close the the UI  
   ***********************************/ 
  int ret = 0;
  static int Minute=-1;
  time_t tp,t;
  struct tm *dt;
  t=time(&tp);
  dt = localtime(&t);
  if( dt->tm_min != Minute ) {
    Minute= dt->tm_min;
    kgFreePixmapImage(kgGetWidget(Tmp,7));
    kgChangePixmapImage(kgGetWidget(Tmp,7),DateTimeImg(&lc,300,100));
    kgUpdateWidget(kgGetWidget(Tmp,7));
    kgUpdateOn(Tmp);
  }
  return ret;
}
