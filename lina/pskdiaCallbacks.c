#include <kulina.h>
  int pskdiatextbox1callback ( int cellno , int i , void *Tmp ) {
  /************************************************* 
   cellno: current cell counted along column strting with 0 
           ie 0 to (nx*ny-1) 
   i     : widget id starting from 0 
   Tmp   : Pointer to DIALOG 
   *************************************************/ 
      DIALOG *D;DIT *T;T_ELMT *e;
      int ret = 1;
      D = ( DIALOG * ) Tmp;
      T = ( DIT * ) kgGetWidget ( Tmp , i ) ;
      e = T-> elmt;
      return ret;
  }
  int pskdiasplbutton1callback ( int butno , int i , void *Tmp ) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
      DIALOG *D;DIL *B;
      int n , ret = 1;
      D = ( DIALOG * ) Tmp;
      B = ( DIL * ) kgGetWidget ( Tmp , i ) ;
      n = B-> nx;
      switch ( butno ) {
          case 1:
          break;
      }
      return ret;
  }
  void pskdiasplbutton1init ( DIL *B , void *pt ) {
  }
  int pskdiainit ( void *Tmp ) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  /* you add any initialisation here */
      int ret = 1;
      DIALOG *D;void *pt;
      DIP *P;
      D = ( DIALOG * ) Tmp;
      pt = D-> pt;
      P = ( DIP * ) kgGetWidget ( Tmp , 0 ) ;
      kgFreeImage ( P-> xpm ) ;
      P-> xpm = NULL;
      D-> df = 1;
      return ret;
  }
  int pskdiacleanup ( void *Tmp ) {
  /* you add any cleanup/mem free here */
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
      int ret = 1;
      DIALOG *D;void *pt;
      D = ( DIALOG * ) Tmp;
      pt = D-> pt;
      return ret;
  }
  int pskdiaCallBack ( void *Tmp , void *tmp ) {
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
          }
      }
      return ret;
  }
  int pskdiaResizeCallBack ( void *Tmp ) {
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
  int pskdiaWaitCallBack ( void *Tmp ) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    Called while waiting for event  
    return value 1 will close the the UI  
   ***********************************/ 
      int ret = 0;
      return ret;
  }
