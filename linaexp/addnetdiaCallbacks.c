#include "kulina.h"
# include "netlist.h"
extern Dlink *Scanlist;
static  char ssid[200],psk[200],buff[500];
extern int wgrp,grp1,grp2;
extern int Textgrp;
extern int addnetgrp,pskgrp;
ThumbNail ** MakeScanThumbnails(Dlink *Scanlist);

static int ProcessText(DIT *T) {
   int i,ret=0;
   T_ELMT *e; 
   e = T->elmt;
   strcpy(buff,kgGetString(T,0));
   i=0; while(buff[i]== ' ') i++;
   if(buff[i]> ' ') {
      ret=1;
      strcpy(ssid,buff+i);
      strcpy(buff,kgGetString(T,1));
      i=0;while(buff[i]== ' ') i++;
      if(buff[i] > ' ') {
        strcpy(psk,buff+i);
      }
      else psk[0]='\0';
   }
   return ret;
}
static int AddNewNet(void *Tmp ) {
  ThumbNail **th;
  NETLIST *nt;
  FILE *fp;
  DIX *X;
  int Infoid;
  X = (DIX *)kgGetNamedWidget(Tmp,(char *)"wdiabrowser");
        nt = (NETLIST *) malloc(sizeof(NETLIST));
        strcpy(nt->ssid,ssid);
        strcpy(nt->psk,psk);
        nt->sig=-100;
        Resetlink(Scanlist);
        Dinsert(Scanlist,nt);
        sprintf(buff,"/usr/share/config/lina/Wireless/%-s",ssid);
        fp = fopen(buff,"w");
        if(psk[0]!='\0') fprintf(fp,"%s %d %d\n",psk,nt->proto,nt->group);
        fclose(fp);
        addnetwork(nt);
        th = (ThumbNail **)kgGetList(X);
        if(th != NULL) kgFreeThumbNails(th);
        kgSetList(X,(void **)MakeScanThumbnails(Scanlist));
        kgUpdateWidget(X);
//        kgUpdateOn(Tmp);
        return 1;
}
int  addnetdiatextbox1callback(int cellno,int i,void *Tmp) {
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
  ProcessText(T);
  return ret;
}
int  addnetdiasplbutton1callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIL *B; 
  DIT *T;T_ELMT *e; 
  int n,ret=0; 
  D = (DIALOG *)Tmp;
  B = (DIL *) kgGetWidget(Tmp,i);
  n = B->nx;
  T = (DIT *)kgGetNamedWidget(Tmp,"addnettext");
  e = T->elmt;
  kgSetGrpVisibility(Tmp,addnetgrp ,0);
  kgSetGrpVisibility(Tmp,wgrp ,1);
  kgUpdateGrp(Tmp,Textgrp);
  kgUpdateGrp(Tmp,addnetgrp);
  kgUpdateGrp(Tmp,wgrp);
  if(ProcessText(T)) AddNewNet(Tmp);
  return ret;
}
void  addnetdiasplbutton1init(DIL *B,void *pt) {
}
int addnetdiainit(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  /* you add any initialisation here */
  int ret = 1;
  DIALOG *D;void *pt;
  D = (DIALOG *)Tmp;
  pt = D->pt;
  D->df=1;
  return ret;
}
int addnetdiacleanup(void *Tmp) {
  /* you add any cleanup/mem free here */
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  int ret = 1;
  DIALOG *D;void *pt;
  DIP *P;
  D = (DIALOG *)Tmp;
  pt = D->pt;
  P = (DIP *) kgGetWidget(Tmp,0);
  kgFreeImage(P->xpm);
  P->xpm = NULL;
  return ret;
}
int addnetdiaCallBack(void *Tmp,void *tmp) {
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
int addnetdiaResizeCallBack(void *Tmp) {
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
int addnetdiaWaitCallBack(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    Called while waiting for event  
    return value 1 will close the the UI  
   ***********************************/ 
  int ret = 0;
  return ret;
}
