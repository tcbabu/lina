#include <kulina.h>
#include "keybrd.h"
extern KEYBRD Kbrd;
int ProcessShift(void *D);
int kgSetKeybrdWidget(int CurWid) {
   Kbrd.CurWid = CurWid;
   return CurWid;
}
int ProcessLabel(void *D,int butno,char *Label) {
  int CurWid;
  char ch;
  CurWid = Kbrd.CurWid;
  if(CurWid< 0) return 0;
  kgSetCurrentWidget(D,CurWid);
  ch= Label[butno-1];
  kgSendKeyEvent(D,ch);
  if(Kbrd.ShiftPress) {
    Kbrd.ShiftPress=0;
    ProcessShift(D);
  }
  return 1;
}
int  keybrdbutton1callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  char Label[]="qwertyuiop";
  char ch;
  int CurWid;
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  CurWid = Kbrd.CurWid;
  if(CurWid< 0) return 0;
  kgSetCurrentWidget(D,CurWid);
  ch= Label[butno-1];
  kgSendKeyEvent(D,ch);
  if(Kbrd.ShiftPress) {
    Kbrd.ShiftPress=0;
    ProcessShift(D);
  }
  return ret;
}
void  keybrdbutton1init(DIN *B,void *pt) {
}
int  keybrdbutton2callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  char Label[]="{}|[]\\";
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  ProcessLabel(D,butno,Label);
  return ret;
}
void  keybrdbutton2init(DIN *B,void *pt) {
}
int  keybrdbutton3callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  char Label[]="~!@#$%^&*()_+`1234567890-=";
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  ProcessLabel(D,butno,Label);
  return ret;
}
void  keybrdbutton3init(DIN *B,void *pt) {
}
int  keybrdbutton4callback(int butno,int i,void *Tmp) {
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
  kgSetCurrentWidget(D,Kbrd.CurWid);
  kgSendBackspaceKeyEvent(Tmp);
  return ret;
}
void  keybrdbutton4init(DIN *B,void *pt) {
}
int  keybrdbutton5callback(int butno,int i,void *Tmp) {
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
  kgSetCurrentWidget(D,Kbrd.CurWid);
  kgSendEscapeKeyEvent(Tmp);
  return ret;
}
void  keybrdbutton5init(DIN *B,void *pt) {
}
int  keybrdbutton6callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  char Label[]="asdfghjkl";
  int n,ret =0; 
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  ProcessLabel(D,butno,Label);
  return ret;
}
void  keybrdbutton6init(DIN *B,void *pt) {
}
int  keybrdbutton7callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  D = (DIALOG *)Tmp;
  char Label[]="zxcvbnm";
  ProcessLabel(D,butno,Label);
  return ret;
}
void  keybrdbutton7init(DIN *B,void *pt) {
}
int ProcessCapsLock(void *D) {
  Kbrd.ShiftPress=0;
  switch(Kbrd.CapsLock) {
    case 0: 
      kgSetGrpVisibility(D,Kbrd.cgrp,0);
      kgSetGrpVisibility(D,Kbrd.ongrp,0);
      kgSetGrpVisibility(D,Kbrd.sgrp,1);
      kgSetGrpVisibility(D,Kbrd.offgrp,1);
#if 0
      kgSetWidgetVisibility(Kbrd.Con,0);
      kgUpdateWidget(Kbrd.Con);
      kgSetWidgetVisibility(Kbrd.Lon,0);
      kgUpdateWidget(Kbrd.Lon);
      kgSetWidgetVisibility(Kbrd.Coff,1);
      kgUpdateWidget(Kbrd.Coff);
      kgSetWidgetVisibility(Kbrd.Loff,1);
      kgUpdateWidget(Kbrd.Loff);
#endif
      break;
    case 1: 
      kgSetGrpVisibility(D,Kbrd.sgrp,0);
      kgSetGrpVisibility(D,Kbrd.offgrp,0);
      kgSetGrpVisibility(D,Kbrd.cgrp,1);
      kgSetGrpVisibility(D,Kbrd.ongrp,1);
#if 0
      kgSetWidgetVisibility(Kbrd.Loff,0);
      kgUpdateWidget(Kbrd.Loff);
      kgSetWidgetVisibility(Kbrd.Coff,0);
      kgUpdateWidget(Kbrd.Coff);
      kgSetWidgetVisibility(Kbrd.Con,1);
      kgUpdateWidget(Kbrd.Con);
      kgSetWidgetVisibility(Kbrd.Lon,1);
      kgUpdateWidget(Kbrd.Lon);
#endif
      break;
  }
  kgUpdateOn(D);
  return 1;
}
int ProcessShift(void *D) {
 switch(Kbrd.ShiftPress) {
    case 0:
      ProcessCapsLock(D);
      break;
    case 1:
      switch(Kbrd.CapsLock) {
        case 1: 
          kgSetGrpVisibility(D,Kbrd.cgrp,0);
          kgSetGrpVisibility(D,Kbrd.sgrp,1);
          break;
        case 0: 
          kgSetGrpVisibility(D,Kbrd.sgrp,0);
          kgSetGrpVisibility(D,Kbrd.cgrp,1);
          break;
      }
     break;
 }
 kgUpdateOn(D);
 return;
}
int  keybrdbutton8callback(int butno,int i,void *Tmp) {
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
  Kbrd.CapsLock = (Kbrd.CapsLock+1)%2;
  ProcessCapsLock(D);
  return ret;
}
void  keybrdbutton8init(DIN *B,void *pt) {
}
int  keybrdbutton19callback(int butno,int i,void *Tmp) {
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
  Kbrd.CapsLock = (Kbrd.CapsLock+1)%2;
  ProcessCapsLock(D);
  return ret;
}
void  keybrdbutton19init(DIN *B,void *pt) {
}
int  keybrdbutton9callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  D = (DIALOG *)Tmp;
  char Label[]="<>?,./";
#if 0
  if(butno==2) {
  kgSetCurrentWidget(D,Kbrd.CurWid);
  kgSendGreaterKeyEvent(D);
  }
  else ProcessLabel(D,butno,Label);
#else
  ProcessLabel(D,butno,Label);
#endif
  return ret;
}
void  keybrdbutton9init(DIN *B,void *pt) {
}
int  keybrdbutton10callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  D = (DIALOG *)Tmp;
  char Label[]=":\";'";
  if(butno==2) {
  kgSetCurrentWidget(D,Kbrd.CurWid);
  kgSendKeyEvent(Tmp,'>');
  }
  else ProcessLabel(D,butno,Label);
  return ret;
}
void  keybrdbutton10init(DIN *B,void *pt) {
}
int  keybrdbutton11callback(int butno,int i,void *Tmp) {
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
  kgSetCurrentWidget(D,Kbrd.CurWid);
  kgSendEnterKeyEvent(Tmp);
  return ret;
}
void  keybrdbutton11init(DIN *B,void *pt) {
}
int  keybrdbutton12callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  D = (DIALOG *)Tmp;
  kgSetCurrentWidget(D,Kbrd.CurWid);
  kgSendSpaceKeyEvent(Tmp);
  return ret;
}
void  keybrdbutton12init(DIN *B,void *pt) {
}
int  keybrdbutton13callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  D = (DIALOG *)Tmp;
  switch(butno) {
    case 1: 
      kgSetCurrentWidget(D,Kbrd.CurWid);
      kgSendRightKeyEvent(Tmp);
      break;
    case 2: 
      kgSetCurrentWidget(D,Kbrd.CurWid);
      kgSendLeftKeyEvent(Tmp);
      break;
  }
  return ret;
}
void  keybrdbutton13init(DIN *B,void *pt) {
}
int  keybrdbutton14callback(int butno,int i,void *Tmp) {
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
  Kbrd.ShiftPress = (Kbrd.ShiftPress+1)%2;
  ProcessShift(D);
  return ret;
}
void  keybrdbutton14init(DIN *B,void *pt) {
}
int  keybrdbutton15callback(int butno,int i,void *Tmp) {
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
      kgSetCurrentWidget(D,Kbrd.CurWid);
      kgSendEndKeyEvent(Tmp);
      break;
    case 2: 
      kgSetCurrentWidget(D,Kbrd.CurWid);
      kgSendHomeKeyEvent(Tmp);
      break;
  }
  return ret;
}
void  keybrdbutton15init(DIN *B,void *pt) {
}
int  keybrdbutton16callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  D = (DIALOG *)Tmp;
  char Label[]="QWERTYUIOP";
  ProcessLabel(D,butno,Label);
  return ret;
}
void  keybrdbutton16init(DIN *B,void *pt) {
}
int  keybrdbutton17callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  D = (DIALOG *)Tmp;
  char Label[]="ASDFGHJKL";
  ProcessLabel(D,butno,Label);
  return ret;
}
void  keybrdbutton17init(DIN *B,void *pt) {
}
int  keybrdbutton18callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  D = (DIALOG *)Tmp;
  char Label[]="ZXCVBNM";
  ProcessLabel(D,butno,Label);
  return ret;
}
void  keybrdbutton18init(DIN *B,void *pt) {
}
int keybrdinit(void *Tmp) {
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
int keybrdcleanup(void *Tmp) {
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
int keybrdCallBack(void *Tmp,void *tmp) {
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
int keybrdResizeCallBack(void *Tmp) {
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
int keybrdWaitCallBack(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    Called while waiting for event  
    return value 1 will close the the UI  
   ***********************************/ 
  int ret = 0;
  return ret;
}
