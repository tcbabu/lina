#include "kulina.h"
#include "headers.h"
#include "passwdfile.h"
void *RunGetSessionDia(void *arg,SESSIONINFO *spt);
Dlink *Sessions=NULL;
DIX *XBrw;
int  AddDelSessionbrowser1callback(int item,int i,void *Tmp) {
  /*********************************** 
    item : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIX *X;void *pt; 
  int ret=1; 
  D = (DIALOG *)Tmp;
  pt = D->pt;
  X = (DIX *)kgGetWidget(Tmp,i);
  switch(item) {
    case 1: 
      break;
  }
  return ret;
}
void  AddDelSessionbrowser1init(DIX *X,void *pt) {
 // One may setup browser list here by setting X->list
 // if it need to be freed set it as X->pt also
 if(Sessions!=NULL) {Dempty(Sessions);}
 Sessions= ReadSessionsFile();
 X->list = (void *)MakeSessionFileThumbNails(Sessions);
}
int  AddDelSessionbutton1callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0,pos,count; 
  ThumbNail **th;
  SESSIONINFO *spt=NULL;
  XBrw=(DIX *)kgGetWidget(Tmp,0);
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  switch(butno) {
    case 1: 
      Resetlink(Sessions);
      spt = (SESSIONINFO *)malloc(sizeof(SESSIONINFO));
      spt->Name[0]='\0';
      spt->Command[0]='\0';
      strcpy(spt->Name," ");
      strcpy(spt->Command," ");
      RunGetSessionDia(Tmp,spt);
      Dposition(Sessions,kgGetSelection(XBrw));
      Dinsert(Sessions,spt);
      break;
    case 2: 
      Resetlink(Sessions);
      Dposition(Sessions,kgGetSelection(XBrw));
      spt=(SESSIONINFO *)Getrecord(Sessions);
      RunGetSessionDia(Tmp,spt);
      break;
    case 3: 
      if(Sessions != NULL) {
        Resetlink(Sessions);
        Dposition(Sessions,kgGetSelection(XBrw));
        Ddelete(Sessions);
      }
      break;
    case 4: 
      if(Sessions != NULL) {
        Resetlink(Sessions);
        pos = kgGetSelection(XBrw);
        Dposition(Sessions,pos);
        if(pos> 1) {
          spt=Dpick(Sessions);
          Dposition(Sessions,pos-1);
          Dinsert(Sessions,spt);
        }
      }
      break;
    case 5: 
      if(Sessions != NULL) {
        Resetlink(Sessions);
        count=Dcount(Sessions);
        pos = kgGetSelection(XBrw);
        Dposition(Sessions,pos);
        if(pos< count) {
          spt=Dpick(Sessions);
          Dadd(Sessions,spt);
        }
      }
      break;
  }
  WriteSessionsFile(Sessions);
  Dempty(Sessions);
  Sessions=ReadSessionsFile();
  th = (ThumbNail **)kgGetList(XBrw);
  if(th != NULL) kgFreeThumbNails(th);
  kgSetList(XBrw,(void **)MakeSessionFileThumbNails(Sessions));
  kgUpdateWidget(XBrw);
  kgUpdateOn(Tmp);
  return ret;
}
void  AddDelSessionbutton1init(DIN *B,void *pt) {
}
int  AddDelSessionsplbutton1callback(int butno,int i,void *Tmp) {
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
  }
  return ret;
}
void  AddDelSessionsplbutton1init(DIL *B,void *pt) {
}
int AddDelSessioninit(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  /* you add any initialisation here */
  int ret = 1;
  DIALOG *D;void *pt;
  D = (DIALOG *)Tmp;
  pt = D->pt;
  return ret;
}
int AddDelSessioncleanup(void *Tmp) {
  /* you add any cleanup/mem free here */
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  int ret = 1;
  DIALOG *D;void *pt;
  D = (DIALOG *)Tmp;
  ThumbNail **th;
  pt = D->pt;
  XBrw=(DIX *)kgGetWidget(Tmp,0);
  th = (ThumbNail **)kgGetList(XBrw);
  if(th != NULL) kgFreeThumbNails(th);
  kgSetList(XBrw,NULL);
  Dempty(Sessions);
  Sessions=NULL;
  return ret;
}
int AddDelSessionCallBack(void *Tmp,void *tmp) {
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
int AddDelSessionResizeCallBack(void *Tmp) {
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
int AddDelSessionWaitCallBack(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    Called while waiting for event  
    return value 1 will close the the UI  
   ***********************************/ 
  int ret = 0;
  return ret;
}
