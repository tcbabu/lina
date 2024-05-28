#include <kulina.h>
#include "passwdfile.h"
  extern LINACONFIG lc;
  extern int Session , Action , PowerDown;
  int SessionDiabrowser1callback ( int item , int i , void *Tmp ) {
  /*********************************** 
    item : selected item (1 to max_item)  not any specific relevence
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
      DIRA *R;DIALOG *D;void *pt;
      ThumbNail **th;
      int ret = 1;
      D = ( DIALOG * ) Tmp;
      pt = D-> pt;
      R = ( DIRA * ) kgGetWidget ( Tmp , i ) ;
      th = ( ThumbNail ** ) R-> list;
      Session = item;
      return ret;
  }
  void SessionDiabrowser1init ( DIRA *R , void *pt ) {
      R-> list = ( void ** ) MakeSessionThumbNails ( & lc ) ;
  }
  int SessionDiainit ( void *Tmp ) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  /* you add any initialisation here */
      int ret = 1;
      DIALOG *D;void *pt;
      D = ( DIALOG * ) Tmp;
      pt = D-> pt;
      return ret;
  }
  int SessionDiacleanup ( void *Tmp ) {
  /* you add any cleanup/mem free here */
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
      int ret = 1;
      DIRA *R;
      DIALOG *D;void *pt;
      D = ( DIALOG * ) Tmp;
      pt = D-> pt;
      R = ( DIRA * ) kgGetWidget ( Tmp , 0 ) ;
      kgFreeThumbNails ( ( ThumbNail ** ) ( R-> list ) ) ;
      R-> list = NULL;
      return ret;
  }
  int SessionDiaCallBack ( void *Tmp , void *tmp ) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    tmp :  Pointer to KBEVENT  
   ***********************************/ 
      int ret = 0;
      DIALOG *D;
      KBEVENT *kbe;
      D = ( DIALOG * ) Tmp;
      kbe = ( KBEVENT * ) tmp;
      if ( kbe-> event == 1 ) {
          if ( kbe-> button == 1 ) {
//      printf("Session Call back\n");
              ret = 1;
          }
      }
      return ret;
  }
  int SessionDiaResizeCallBack ( void *Tmp ) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
      int ret = 0;
      int xres , yres , dx , dy;
      DIALOG *D;
      D = ( DIALOG * ) Tmp;
      kgGetWindowSize ( D , & xres , & yres ) ;
      dx = xres - D-> xl;
      dy = yres - D-> yl;
  /* extra code */
      D-> xl = xres;
      D-> yl = yres;
      kgRedrawDialog ( D ) ;
      return ret;
  }
  int SessionDiaWaitCallBack ( void *Tmp ) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    Called while waiting for event  
    return value 1 will close the the UI  
   ***********************************/ 
      int ret = 0;
      return ret;
  }
