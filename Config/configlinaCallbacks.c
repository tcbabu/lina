#include <kulina.h>
#include "passwdfile.h"
#include "headers.h"
extern int Enabled;
extern LINACONFIG lc;
int Systemd;
char Image[300];
int ir=255,ig=255,ib=255;
void *AddDelSession(void *);
void * kgColorImage(int size,int red,int green,int blue);
int SearchString(char *s1,char *s2);
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
int  configlinatextbox1callback(int cellno,int i,void *Tmp) {
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
  strcpy(lc.Bkgr,(char *)kgGetString(T,0));
  return ret;
}
int  configlinabutton1callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  strcpy(Image,kgGetString(kgGetWidget(D,0),0));
  kgSelectImage(NULL,100,100,136,Image);
  kgSetString(kgGetWidget(D,0),0,Image);
  strcpy(lc.Bkgr,Image);
  kgUpdateWidget(kgGetWidget(D,0));
  switch(butno) {
    case 1: 
      break;
  }
  return ret;
}
void  configlinabutton1init(DIN *B,void *pt) {
}
int  configlinabutton2callback_o(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  ir = lc.Red;
  ig = lc.Green;
  ib = lc.Blue;
  kgGetColor(NULL,100,100,&ir,&ig,&ib);
  lc.Red = ir;
  lc.Green = ig;
  lc.Blue = ib;
  kgChangeButtonColor(B,0,lc.Red,lc.Green,lc.Blue);
  kgUpdateWidget(B);
  switch(butno) {
    case 1: 
      break;
  }
  return ret;
}
void  configlinabutton2init_o(DIN *B,void *pt) {
   kgChangeButtonColor(B,0,lc.Red,lc.Green,lc.Blue);
}
int  configlinabutton3callback_o(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  ir = lc.HighRed;
  ig = lc.HighGreen;
  ib = lc.HighBlue;
  kgGetColor(NULL,100,100,&ir,&ig,&ib);
  lc.HighRed = ir;
  lc.HighGreen = ig;
  lc.HighBlue = ib;
  kgChangeButtonColor(B,0,lc.HighRed,lc.HighGreen,lc.HighBlue);
  kgUpdateWidget(B);
  switch(butno) {
    case 1: 
      break;
  }
  return ret;
}
void  configlinabutton3init_o(DIN *B,void *pt) {
   kgChangeButtonColor(B,0,lc.HighRed,lc.HighGreen,lc.HighBlue);
}
int  configlinabutton4callback_o(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  ir = lc.FontRed;
  ig = lc.FontGreen;
  ib = lc.FontBlue;
  kgGetColor(NULL,100,100,&ir,&ig,&ib);
  lc.FontRed = ir;
  lc.FontGreen = ig;
  lc.FontBlue = ib;
  kgChangeButtonColor(B,0,lc.FontRed,lc.FontGreen,lc.FontBlue);
  kgUpdateWidget(B);
  switch(butno) {
    case 1: 
      break;
  }
  return ret;
}
void  configlinabutton4init_o(DIN *B,void *pt) {
   kgChangeButtonColor(B,0,lc.FontRed,lc.FontGreen,lc.FontBlue);
}
int  configlinabrowser1callback(int item,int i,void *Tmp) {
  /*********************************** 
    item : selected item (1 to max_item)  not any specific relevence
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DICH *C;DIALOG *D;void *pt; 
  ThumbNail **th; 
  int ret=1; 
  D = (DIALOG *)Tmp;
  pt = D->pt;
  C = (DICH *)kgGetWidget(Tmp,i);
  th = (ThumbNail **) C->list;
  lc.NoRootLogin= (kgGetSwitch(C,0)+1)%2;
  lc.TextMode = (kgGetSwitch(C,1));
  lc.SafeMode = (kgGetSwitch(C,2));
  lc.ShowTime = (kgGetSwitch(C,3));
  return ret;
}
void  configlinabrowser1init(DICH *C,void *pt) {
  C->item=-1;
  kgSetSwitch(C,0,(lc.NoRootLogin+1)%2);
  kgSetSwitch(C,1,(lc.TextMode));
  kgSetSwitch(C,2,(lc.SafeMode));
  kgSetSwitch(C,3,(lc.ShowTime));
  printf("configlinabrowser1init\n");
}
int  configlinabrowser2callback(int item,int i,void *Tmp) {
  /*********************************** 
    item : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIW *B; 
  int ret=1; 
  D = (DIALOG *)Tmp;
  B = (DIW *) kgGetWidget(Tmp,i);
  strcpy(lc.DefUser,GetUserId(&lc,item));
  switch(item) {
    case 1: 
      break;
  }
  return ret;
}
int  configlinabrowser3callback(int item,int i,void *Tmp) {
  /*********************************** 
    item : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIW *B; 
  int ret=1; 
  D = (DIALOG *)Tmp;
  B = (DIW *) kgGetWidget(Tmp,i);
  strcpy(lc.DefSession,kgGetSelectedString(B));
  switch(item) {
    case 1: 
      break;
  }
  return ret;
}
int  configlinafslide1callback(double val,int i,void *Tmp) {
  /*********************************** 
    val : current value 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIF *F; 
  int ret=1; 
  D = (DIALOG *)Tmp;
  F = (DIF *) kgGetWidget(Tmp,i);
  lc.Transparency=val;
  return ret;
}
int  configlinafslide2callback(double val,int i,void *Tmp) {
  /*********************************** 
    val : current value 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIF *F; 
  int ret=1; 
  D = (DIALOG *)Tmp;
  F = (DIF *) kgGetWidget(Tmp,i);
  lc.fac=val;
  return ret;
}
int  configlinasplbutton1callback(int butno,int i,void *Tmp) {
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
  switch(butno) {
    case 1: 
      break;
    case 2: 
      strcpy(Image,kgGetString(kgGetWidget(D,0),0));
      strcpy(lc.Bkgr,(char *)Image);
      WriteConfig(&lc);
      break;
  }
  return ret;
}
void  configlinasplbutton1init(DIL *B,void *pt) {
}
int configlinainit(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  /* you add any initialisation here */
  int ret = 1;
  DIALOG *D;void *pt;
  D = (DIALOG *)Tmp;
  pt = D->pt;
  printf("Init Enabled= %d\n",Enabled);
  return ret;
}
int configlinacleanup(void *Tmp) {
  /* you add any cleanup/mem free here */
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  int ret = 1;
  DIALOG *D;void *pt;
  D = (DIALOG *)Tmp;
  pt = D->pt;
  return ret;
}
int configlinaCallBack(void *Tmp,void *tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    tmp :  Pointer to KBEVENT  
   ***********************************/ 
  int ret = 0;
  DIALOG *D;
  KBEVENT *kbe;
  D = (DIALOG *)Tmp;
  kbe = (KBEVENT *)tmp;
  if(kbe->event ==1) {
    if(kbe->button ==1) {
    }
  }
  return ret;
}
int configlinaResizeCallBack(void *Tmp) {
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
int configlinaWaitCallBack(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    Called while waiting for event  
    return value 1 will close the the UI  
   ***********************************/ 
  int ret = 0;
  return ret;
}
void  configlinabutton2init(DIL *B,void *pt) {
}
int  configlinabutton2callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  switch(butno) {
    case 1: 
      RunAddDelSession(Tmp);
      break;
  }
  return ret;
}
int  configlinabutton3callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  int ir,ig,ib;
  void *img;
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  switch(butno) {
    case 1: 
      ir = lc.Red;
      ig = lc.Green;
      ib = lc.Blue;
      kgGetColor(Tmp,20,60,&ir,&ig,&ib);
      lc.Red = ir;
      lc.Green = ig;
      lc.Blue = ib;
      kgChangeButtonColor(B,0,lc.Red,lc.Green,lc.Blue);
      img = kgChangeButtonNormalImage(B,0,kgColorImage(48,lc.Red,lc.Green,lc.Blue));
      kgUpdateWidget(B);
      kgFreeImage(img);
      break;
    case 2: 
      ir = lc.HighRed;
      ig = lc.HighGreen;
      ib = lc.HighBlue;
      kgGetColor(Tmp,20,60,&ir,&ig,&ib);
      lc.HighRed = ir;
      lc.HighGreen = ig;
      lc.HighBlue = ib;
      kgChangeButtonColor(B,1,lc.HighRed,lc.HighGreen,lc.HighBlue);
      img = kgChangeButtonNormalImage(B,1,kgColorImage(48,lc.HighRed,lc.HighGreen,lc.HighBlue));
      kgUpdateWidget(B);
      kgFreeImage(img);
      break;
    case 3: 
      ir = lc.FontRed;
      ig = lc.FontGreen;
      ib = lc.FontBlue;
      kgGetColor(Tmp,20,60,&ir,&ig,&ib);
      lc.FontRed = ir;
      lc.FontGreen = ig;
      lc.FontBlue = ib;
      kgChangeButtonColor(B,2,lc.FontRed,lc.FontGreen,lc.FontBlue);
      img = kgChangeButtonNormalImage(B,2,kgColorImage(48,lc.FontRed,lc.FontGreen,lc.FontBlue));
      kgUpdateWidget(B);
      kgFreeImage(img);
      break;
    case 4: 
      ir = lc.ButRed;
      ig = lc.ButGreen;
      ib = lc.ButBlue;
      kgGetColor(Tmp,20,60,&ir,&ig,&ib);
      lc.ButRed = ir;
      lc.ButGreen = ig;
      lc.ButBlue = ib;
      kgChangeButtonColor(B,3,lc.ButRed,lc.ButGreen,lc.ButBlue);
      img = kgChangeButtonNormalImage(B,3,kgColorImage(48,lc.ButRed,lc.ButGreen,lc.ButBlue));
      kgUpdateWidget(B);
      kgFreeImage(img);
      break;
    case 5: 
      ir = lc.DateRed;
      ig = lc.DateGreen;
      ib = lc.DateBlue;
      kgGetColor(Tmp,20,60,&ir,&ig,&ib);
      lc.DateRed = ir;
      lc.DateGreen = ig;
      lc.DateBlue = ib;
      kgChangeButtonColor(B,4,lc.DateRed,lc.DateGreen,lc.DateBlue);
      img = kgChangeButtonNormalImage(B,4,kgColorImage(48,lc.DateRed,lc.DateGreen,lc.DateBlue));
      kgUpdateWidget(B);
      kgFreeImage(img);
      break;
  }
  return ret;
}
void  configlinabutton3init(DIN *B,void *pt) {
   B->item=-1;
   kgChangeButtonColor(B,0,lc.Red,lc.Green,lc.Blue);
   kgChangeButtonColor(B,1,lc.HighRed,lc.HighGreen,lc.HighBlue);
   kgChangeButtonColor(B,2,lc.FontRed,lc.FontGreen,lc.FontBlue);
   kgChangeButtonColor(B,3,lc.ButRed,lc.ButGreen,lc.ButBlue);
   kgChangeButtonColor(B,4,lc.DateRed,lc.DateGreen,lc.DateBlue);
  printf("configlinabutton3init\n");
}
int  configlinabrowser4callback(int item,int i,void *Tmp) {
  /*********************************** 
    item : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIW *B; 
  int ret=1; 
  D = (DIALOG *)Tmp;
  B = (DIW *) kgGetWidget(Tmp,i);
  lc.DateFont=item-1;
  switch(item) {
    case 1: 
      break;
  }
  return ret;
}
int ProcessInittab(void) {
  int i;
  FILE *fp,*op;
  char buff[500];
  fp = fopen("/etc/inittab.org","r");
  op = fopen("/etc/inittab","w");
  if(fp==NULL) return 0;
  if(op==NULL) {fclose(fp);return 0;}
  while (fgets(buff,499,fp) != NULL) {
   i=0;
   while((buff[i]==' ')||(buff[i]=='\t')) i++;
   if(buff[i]=='#') {
     fprintf(op,"%s",buff+i);
     continue;
   }
   if(strncmp(buff+i,(char *)"kd:5:",5) ==0) {
     fprintf(op,"kd:5:respawn:%-s\n",kgWhich("lina"));
   }
   else {fprintf(op,"%s",buff+i);}
  }
  fclose(fp);
  fclose(op);
  return 1;
}
int CheckInittab(void) {
  int i,ret=0;
  FILE *fp,*op;
  char buff[500];
  fp = fopen("/etc/inittab","r");
  if(fp==NULL) return ret;
  while (fgets(buff,499,fp) != NULL) {
   i=0;
   while((buff[i]==' ')||(buff[i]=='\t')) i++;
   if(buff[i]=='#') {
     continue;
   }
   if(strncmp(buff+i,(char *)"kd:5:",5) ==0) {
     if(SearchString(buff,"lina") >=0 ) ret=1;
   }
  }
  fclose(fp);
  return ret;
}
int CheckSystemd(void) {
  int i,ret=0;
  FILE *fp,*op;
  char buff[500];
  fp = popen("ls -l /etc/systemd/system/display-manager.service","r");
  if(fp==NULL) return ret;
  while (fgets(buff,499,fp) != NULL) {
   i=0;
   while((buff[i]==' ')||(buff[i]=='\t')) i++;
   if(buff[i]=='#') {
     continue;
   }
   if(SearchString(buff,"lina") >=0 ) ret=1;
  }
  pclose(fp);
  return ret;
}
int  configlinabutton4callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/
  FILE *fp;
  DIALOG *D;DIN *B;
  int n,ret =0;
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  Enabled = (Enabled+1)%2;
  if(Enabled) {
    kgChangeButtonColor(B,0,120,0,50);
    kgChangeButtonTitle(B,0,(char *)"!c15!z32Disable Lina");
    if(Systemd) {
      fp = fopen("/etc/systemd/system/display-manager.service","r");
      if(fp != NULL) {
        fclose(fp);
        fp = fopen("/etc/systemd/system/display-manager.service.org","r");
        if(fp== NULL) 
          system("mv -f /etc/systemd/system/display-manager.service /etc/systemd/system/display-manager.service.org");
        else fclose(fp);
        system("ln -sf /lib/systemd/system/lina.service /etc/systemd/system/display-manager.service");
      }
      else {
       system("install -m 755  /usr/share/lina/lina.init /etc/init.d/lina");
      }
      fp = fopen("/etc/X11/default-display-manager","r");
      if(fp != NULL) {
        fclose(fp);
        fp = fopen("/etc/X11/default-display-manager.org","r");
        if(fp== NULL) 
          system("mv -f /etc/X11/default-display-manager /etc/X11/default-display-manager.org");
        else fclose(fp);
        fp = fopen("/etc/X11/default-display-manager","w");
        fprintf(fp,"/usr/sbin/lina\n");
        fclose(fp);
      }
    }
    else {
      fp = fopen("/etc/inittab.org","r");
      if(fp== NULL) system("cp -f /etc/inittab /etc/inittab.org");
      else fclose(fp);
      ProcessInittab();
    }
  }
  else {
    kgChangeButtonColor(B,0,0,220,50);
    kgChangeButtonTitle(B,0,(char *)"!c15!z32Enable Lina");
    if(Systemd) {
      system("mv -f /etc/systemd/system/display-manager.service.org /etc/systemd/system/display-manager.service");
    }
    else {
      system("cp -f /etc/inittab.org /etc/inittab");
    }
  }
  kgUpdateWidget(B);
  switch(butno) {
    case 1:
      break;
  }
  return ret;
}
int CheckLinaStatus(void) {
    if(Systemd) {
       return CheckSystemd();
    }
    else {
      return CheckInittab();
    }
}
void  configlinabutton4init(DIN *B,void *pt) {
  B->item=-1;
  Enabled=0;
  if(CheckProcess("systemd") >0) Systemd=1;
  else Systemd=0;
  if(CheckLinaStatus()) Enabled=1;
  printf("Enabled= %d\n",Enabled);
  if(Enabled) {
    kgChangeButtonColor(B,0,120,0,50);
    kgChangeButtonTitle(B,0,(char *)"!c15!z32Disable Lina");
  }
  else {
    kgChangeButtonColor(B,0,0,220,50);
    kgChangeButtonTitle(B,0,(char *)"!c15!z32Enable Lina");
  }
}

