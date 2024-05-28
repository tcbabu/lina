#include <kulina.h>
//#include "keybrdCallbacks.h"
  static void *GetButTopImage ( int l , int h , int Red , int Green , int Blue ) 
      {
      GMIMG *img;
      void *rzimg , *fid , *tmp , *mask;
      int Color = 68 , rsize;
      int IconShape = 3;
      float rf , gf , bf , H , S , V , Vb , rfac = 0.2;
      float x , y;
      rfac = 0.0;
      fid = kgInitImage ( l , h , 1 ) ;
      l = ( l ) /2;
      h = h/2;
      kgUserFrame ( fid , -l-1.0 , -h-1.0 , l+1. , h+1. ) ;
      RGBtoHSV ( ( float ) Red , ( float ) Green , 
          ( float ) Blue , & H , & S , & V ) ;
      Vb = 0.8*V;
      if ( Vb > 1. ) Vb = 1.;
      HSVtoRGB ( & rf , & gf , & bf , H , S , Vb ) ;
      kgChangeColor ( fid , 252 , ( int ) rf , ( int ) gf , ( int ) bf ) ;
      Vb = 1.2*V;
      if ( Vb > 1. ) Vb = 1.;
      HSVtoRGB ( & rf , & gf , & bf , H , S , Vb ) ;
      kgChangeColor ( fid , 251 , ( int ) rf , ( int ) gf , ( int ) bf ) ;
      kgRoundedRectangleFill ( fid , 0. , 0. , 2.*l , 2.*l , 0 , 252 , rfac ) ;
      kgLineColor ( fid , 251 ) ;
      x = -l;
      y = -h-1;
      kgMove2f ( fid , x , y ) ;
      y = h+1;
      kgDraw2f ( fid , x , y ) ;
      x = l;
      kgDraw2f ( fid , x , y ) ;
      kgLineColor ( fid , 0 ) ;
      x = l;
      y = -h-1;
      kgDraw2f ( fid , x , y ) ;
//   kgRoundedRectangleFill(fid,0.,0.,2*l-4.,2*l-4.,0,251,rfac);
      rzimg = kgGetResizedImage ( fid ) ;
      kgCloseImage ( fid ) ;
      return rzimg;
  }
  static void *GetButBottomImage ( int l , int h , int Red , int Green , int Blue ) 
      {
      GMIMG *img;
      void *rzimg , *fid , *tmp , *mask;
      int Color = 68 , rsize;
      int IconShape = 3;
      float rf , gf , bf , H , S , V , Vb , rfac = 0.2;
      float x , y;
      rfac = 0.0;
      fid = kgInitImage ( l , h , 1 ) ;
      l = ( l ) /2;
      h = h/2;
      kgUserFrame ( fid , -l-1.0 , -h-1.0 , l+1. , h+1. ) ;
      RGBtoHSV ( ( float ) Red , ( float ) Green , 
          ( float ) Blue , & H , & S , & V ) ;
      Vb = 0.8*V;
      if ( Vb > 1. ) Vb = 1.;
      HSVtoRGB ( & rf , & gf , & bf , H , S , Vb ) ;
      kgChangeColor ( fid , 352 , ( int ) rf , ( int ) gf , ( int ) bf ) ;
      Vb = 1.2*V;
      if ( Vb > 1. ) Vb = 1.;
      HSVtoRGB ( & rf , & gf , & bf , H , S , Vb ) ;
      kgChangeColor ( fid , 351 , ( int ) rf , ( int ) gf , ( int ) bf ) ;
      kgRoundedRectangleFill ( fid , 0. , 0. , 2.*l , 2.*l , 0 , 352 , rfac ) ;
      kgLineColor ( fid , 351 ) ;
      x = -l;
      y = -h;
      kgMove2f ( fid , x , y ) ;
      y = h+1;
      kgDraw2f ( fid , x , y ) ;
      x = l;
      kgMove2f ( fid , x , y ) ;
      kgLineColor ( fid , 0 ) ;
      x = l;
      y = -h-1;
      kgDraw2f ( fid , x , y ) ;
      x = -l;
      kgDraw2f ( fid , x , y ) ;
//   kgRoundedRectangleFill(fid,0.,0.,2*l-4.,2*l-4.,0,351,rfac);
      rzimg = kgGetResizedImage ( fid ) ;
      kgCloseImage ( fid ) ;
      return rzimg;
  }
  static void *GetButImage ( int l , int h , int Red , int Green , int Blue ) {
      GMIMG *img;
      void *rzimg , *fid , *tmp , *mask;
      int Color = 68 , rsize;
      int IconShape = 3;
      float rf , gf , bf , H , S , V , Vb , rfac = 0.2;
      float x , y;
      rfac = 0.0;
      fid = kgInitImage ( l , h , 1 ) ;
      l = ( l ) /2;
      h = h/2;
      kgUserFrame ( fid , -l-1.0 , -h-1.0 , l+1. , h+1. ) ;
      RGBtoHSV ( ( float ) Red , ( float ) Green , 
          ( float ) Blue , & H , & S , & V ) ;
      Vb = 0.8*V;
      if ( Vb > 1. ) Vb = 1.;
      HSVtoRGB ( & rf , & gf , & bf , H , S , Vb ) ;
      kgChangeColor ( fid , 452 , ( int ) rf , ( int ) gf , ( int ) bf ) ;
      Vb = 1.2*V;
      if ( Vb > 1. ) Vb = 1.;
      HSVtoRGB ( & rf , & gf , & bf , H , S , Vb ) ;
      kgChangeColor ( fid , 451 , ( int ) rf , ( int ) gf , ( int ) bf ) ;
      kgRoundedRectangleFill ( fid , 0. , 0. , 2.*l , 2.*l , 0 , 452 , rfac ) ;
      kgLineColor ( fid , 451 ) ;
      x = -l;
      y = -h-1;
      kgMove2f ( fid , x , y ) ;
      y = h+1;
      kgDraw2f ( fid , x , y ) ;
      x = l;
      kgDraw2f ( fid , x , y ) ;
      kgLineColor ( fid , 0 ) ;
      x = l;
      y = -h-1;
      kgDraw2f ( fid , x , y ) ;
      x = -l;
      kgDraw2f ( fid , x , y ) ;
//   kgRoundedRectangleFill(fid,0.,0.,2*l-4.,2*l-4.,0,451,rfac);
      rzimg = kgGetResizedImage ( fid ) ;
      kgCloseImage ( fid ) ;
      return rzimg;
  }
  static void *MakePixmapImage ( int xl , int yl , int r , int g , int b , float fac ) 
      {
      void *Img , *fid , *Img1;
      float h , s , v , rf , gf , bf , vm;
      fid = kgInitImage ( xl , yl , 4 ) ;
      kgUserFrame ( fid , -2. , -2. , ( float ) xl+2 , ( float ) yl+2 ) ;
      RGBtoHSV ( ( float ) r , ( float ) g , ( float ) b , & h , & s , & v ) ;
      vm = 1.2*v;
      if ( vm > 1.0 ) vm = 1.0;
      HSVtoRGB ( & rf , & gf , & bf , h , s , vm ) ;
      kgChangeColor ( fid , 552 , ( int ) rf , ( int ) gf , ( int ) bf ) ;
      HSVtoRGB ( & rf , & gf , & bf , h , s , 0.6*v ) ;
      kgChangeColor ( fid , 553 , ( int ) rf , ( int ) gf , ( int ) bf ) ;
      kgChangeColor ( fid , 551 , ( int ) r , ( int ) g , ( int ) b ) ;
      kgRoundedRectangleFill ( fid , ( float ) xl*0.5 , ( float ) yl*0.5 , 
          ( float ) xl+2 , ( float ) yl+2 , 0 , 253 , fac ) ;
      HSVtoRGB ( & rf , & gf , & bf , h , s , 0.9*v ) ;
      kgChangeColor ( fid , 553 , ( int ) rf , ( int ) gf , ( int ) bf ) ;
      kgRoundedRectangleFill ( fid , ( float ) xl*0.5 , ( float ) yl*0.5 , 
          ( float ) xl+1 , ( float ) yl+1 , 0 , 553 , fac ) ;
      kgRoundedRectangleFill ( fid , ( float ) xl*0.5 , ( float ) yl*0.5 , 
          ( float ) xl , ( float ) yl , 0 , 552 , fac ) ;
      kgRoundedRectangleFill ( fid , ( float ) xl*0.5 , ( float ) yl*0.5 , 
          ( float ) xl-4 , ( float ) yl-4 , 0 , 551 , fac ) ;
      Img = kgGetSharpImage ( fid ) ;
      kgCloseImage ( fid ) ;
      return Img;
  }
  int keybrdbutton1callback ( int butno , int i , void *Tmp ) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
      DIALOG *D;DIN *B;
      int n , ret = 0;
      int CurWid;
      D = ( DIALOG * ) Tmp;
      B = ( DIN * ) kgGetWidget ( Tmp , i ) ;
      n = B-> nx*B-> ny;
      CurWid = * ( ( int * ) ( B-> arg ) ) ;
      kgSetCurrentWidget ( D , CurWid ) ;
      char *title;
      title = kgGetButtonTitle ( B , butno-1 ) ;
      if ( title != NULL ) {
          if ( title [ 0 ] == '!' ) title += 4;
          if ( strlen ( title ) == 1 ) {
              int ch;
              ch = title [ 0 ] ;
              kgSendKeyEvent ( D , ch ) ;
          }
      }
      switch ( butno ) {
          case 1:
          break;
          case 2:
          break;
          case 3:
          break;
          case 4:
          break;
          case 5:
          break;
          case 6:
          break;
          case 7:
          break;
          case 8:
          break;
          case 9:
          kgSendKeyEvent ( D , '*' ) ;
          break;
          case 10:
          break;
          case 11:
          break;
          case 12:
          break;
          case 13:
          break;
          case 14:
          break;
          case 15:
          break;
          case 16:
          break;
          case 17:
          break;
          case 18:
          break;
          case 19:
          break;
          case 20:
          break;
          case 21:
          break;
          case 22:
          break;
          case 23:
          break;
          case 24:
          break;
          case 25:
          break;
          case 26:
          break;
      }
      return ret;
  }
  void keybrdbutton1init ( DIN *B , void *pt ) {
      B-> arg = pt;
  }
  int keybrdbutton2callback ( int butno , int i , void *Tmp ) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
      DIALOG *D;DIN *B;
      int n , ret = 0;
      int CurWid;
      D = ( DIALOG * ) Tmp;
      B = ( DIN * ) kgGetWidget ( Tmp , i ) ;
      n = B-> nx*B-> ny;
      CurWid = * ( ( int * ) ( B-> arg ) ) ;
      kgSetCurrentWidget ( D , CurWid ) ;
      char *title;
      title = kgGetButtonTitle ( B , butno-1 ) ;
      if ( title != NULL ) {
          if ( title [ 0 ] == '!' ) title += 4;
          if ( strlen ( title ) == 1 ) {
              int ch;
              ch = title [ 0 ] ;
              kgSendKeyEvent ( D , ch ) ;
          }
      }
      switch ( butno ) {
          case 1:
          break;
          case 2:
          break;
          case 3:
          break;
          case 4:
          break;
          case 5:
          break;
          case 6:
          break;
          case 7:
          break;
          case 8:
          break;
          case 9:
          break;
          case 10:
          break;
          case 11:
          break;
          case 12:
          break;
          case 13:
          break;
          case 14:
          break;
          case 15:
          break;
          case 16:
          break;
          case 17:
          break;
          case 18:
          break;
          case 19:
          break;
          case 20:
          break;
          case 21:
          break;
          case 22:
          break;
          case 23:
          break;
          case 24:
          break;
          case 25:
          break;
          case 26:
          break;
      }
      return ret;
  }
  void keybrdbutton2init ( DIN *B , void *pt ) {
      B-> arg = pt;
  }
  int keybrdbutton3callback ( int butno , int i , void *Tmp ) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
      DIALOG *D;DIN *B;
      int n , ret = 0;
      int CurWid;
      D = ( DIALOG * ) Tmp;
      B = ( DIN * ) kgGetWidget ( Tmp , i ) ;
      n = B-> nx*B-> ny;
      CurWid = * ( ( int * ) ( B-> arg ) ) ;
      kgSetCurrentWidget ( D , CurWid ) ;
      char *title;
      title = kgGetButtonTitle ( B , butno-1 ) ;
      if ( title != NULL ) {
          if ( title [ 0 ] == '!' ) title += 4;
          if ( strlen ( title ) == 1 ) {
              int ch;
              ch = title [ 0 ] ;
              kgSendKeyEvent ( D , ch ) ;
              return ret;
          }
//    else printf("strlen = %d %s\n",strlen(title),title);
      }
      switch ( butno ) {
          case 1:
          kgSendClearKeyEvent ( D ) ;
          break;
          case 2:
          break;
          case 3:
          break;
          case 4:
          break;
          case 5:
          break;
          case 6:
          break;
          case 7:
          break;
          case 8:
          break;
          case 9:
          break;
          case 10:
          break;
          case 11:
          break;
          case 12:
          break;
          case 13:
          kgSendLinefeedKeyEvent ( D ) ;
          break;
          case 14:
          kgSendEscapeKeyEvent ( D ) ;
          break;
          case 15:
          break;
          case 16:
          break;
          case 17:
          break;
          case 18:
          break;
          case 19:
          break;
          case 20:
          break;
          case 21:
          break;
          case 22:
          break;
          case 23:
          break;
          case 24:
          break;
          case 25:
          break;
          case 26:
          kgSendLinefeedKeyEvent ( D ) ;
          break;
      }
      return ret;
  }
  void keybrdbutton3init ( DIN *B , void *pt ) {
      B-> arg = pt;
  }
  int keybrdbutton4callback ( int butno , int i , void *Tmp ) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
      DIALOG *D;DIN *B;
      int n , ret = 0;
      int CurWid;
      D = ( DIALOG * ) Tmp;
      B = ( DIN * ) kgGetWidget ( Tmp , i ) ;
      n = B-> nx*B-> ny;
      CurWid = * ( ( int * ) ( B-> arg ) ) ;
      kgSetCurrentWidget ( D , CurWid ) ;
      char *title;
      title = kgGetButtonTitle ( B , butno-1 ) ;
      if ( title != NULL ) {
          if ( title [ 0 ] == '!' ) title += 4;
          if ( strlen ( title ) == 1 ) {
              int ch;
              ch = title [ 0 ] ;
              kgSendKeyEvent ( D , ch ) ;
          }
      }
      switch ( butno ) {
          case 1:
          kgSendPageupKeyEvent ( D ) ;
          break;
          case 2:
          break;
          case 3:
          break;
          case 4:
          break;
          case 5:
          break;
          case 6:
          break;
          case 7:
          break;
          case 8:
          break;
          case 9:
          kgSendInsertKeyEvent ( D ) ;
//      kgSendKeyEvent(D,' ');
          break;
          case 10:
          kgSendLeftKeyEvent ( D ) ;
          break;
          case 11:
          kgSendUpKeyEvent ( D ) ;
          break;
          case 12:
          kgSendBackspaceKeyEvent ( D ) ;
          break;
          case 13:
          kgSendHomeKeyEvent ( D ) ;
          break;
          case 14:
          kgSendPagedownKeyEvent ( D ) ;
          break;
          case 15:
          break;
          case 16:
          break;
          case 17:
          break;
          case 18:
          break;
          case 19:
          break;
          case 20:
          break;
          case 21:
          break;
          case 22:
//      kgSendSpaceKeyEvent(D);
          kgSendKeyEvent ( D , ' ' ) ;
          break;
          case 23:
          kgSendRightKeyEvent ( D ) ;
          break;
          case 24:
          kgSendDownKeyEvent ( D ) ;
          break;
          case 25:
          kgSendDeleteKeyEvent ( D ) ;
          break;
          case 26:
          kgSendEndKeyEvent ( D ) ;
          break;
      }
      return ret;
  }
  void keybrdbutton4init ( DIN *B , void *pt ) {
      B-> arg = pt;
  }
  int keybrdinit ( void *Tmp ) {
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
  int keybrdcleanup ( void *Tmp , int Grpid ) {
  /* you add any cleanup/mem free here */
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
      WIDGETGRP *Grp;
      int ret = 1 , i;
      void **v;
      DIALOG *D;void *pt;
      D = ( DIALOG * ) Tmp;
      pt = D-> pt;
      if ( Grpid > 0 ) {
          Grp = kgGetWidgetGrp ( Tmp , Grpid ) ;
          if ( Grp != NULL ) {
              v = Grp-> arg;
              if ( v != NULL ) {
                  i = 0;
                  while ( v [ i ] != NULL ) kgFreeImage ( v [ i++ ] ) ;
                  free ( v ) ;
              }
          }
      }
      return ret;
  }
  int keybrdCallBack ( void *Tmp , void *tmp ) {
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
  int keybrdResizeCallBack ( void *Tmp ) {
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
  int keybrdWaitCallBack ( void *Tmp ) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    Called while waiting for event  
    return value 1 will close the the UI  
   ***********************************/ 
      int ret = 0;
      return ret;
  }
  void ModifykeybrdGc ( Gclr *gc ) {
/*
//  You may change default settings here 
//  probably you can allow the user to create a config in $HOME
//  and try to read that file (if exits); so dynamic configuration is possible
   gc->FontSize =8;
   gc->Font=23;
*/
  }
  int keybrdGroup ( DIALOG *D , void **v , void *pt ) {
      void *Bimgu , *Bimgl;
      int GrpId = 0 , oitems = 0 , i , j;
      DIA *d = NULL , *dtmp;
      Gclr *gc;
      char *xpm0;
      int Red , Green , Blue;
      Bimgu = v [ 0 ] ;
      gc = & ( D-> gc ) ;
      kgGetDefaultRGB ( gc-> fill_clr , & Red , & Green , & Blue ) ;
      xpm0 = ( char * ) MakePixmapImage ( 829 , 252 , Red , Green , Blue , 0.1 ) ;
      DIP p0 = {
      'p' , 1 , 1 , 840 , 252 , ( void * ) xpm0 , -1 , /* bkgr colour */ 0 , 0 , 0.2 /* border hide */ };
          
      strcpy ( p0.Wid , ( char * ) "keybrdWidget1" ) ;
      BUT_STR *butn1 = NULL;
      butn1 = ( BUT_STR * ) malloc ( sizeof ( BUT_STR ) *26 ) ;
      butn1 [ 0 ] .sw = 1;
      strcpy ( butn1 [ 0 ] .title , ( char * ) "!z32~" ) ;
      butn1 [ 0 ] .xpmn = Bimgu;
      butn1 [ 0 ] .xpmp = NULL;
      butn1 [ 0 ] .xpmh = NULL;
      butn1 [ 0 ] .bkgr = -1;
      butn1 [ 0 ] .butncode = '';
      butn1 [ 1 ] .sw = 1;
      strcpy ( butn1 [ 1 ] .title , ( char * ) "!z32!!" ) ;
      butn1 [ 1 ] .xpmn = Bimgu;
      butn1 [ 1 ] .xpmp = NULL;
      butn1 [ 1 ] .xpmh = NULL;
      butn1 [ 1 ] .bkgr = -1;
      butn1 [ 1 ] .butncode = '';
      butn1 [ 2 ] .sw = 1;
      strcpy ( butn1 [ 2 ] .title , ( char * ) "!z32@" ) ;
      butn1 [ 2 ] .xpmn = Bimgu;
      butn1 [ 2 ] .xpmp = NULL;
      butn1 [ 2 ] .xpmh = NULL;
      butn1 [ 2 ] .bkgr = -1;
      butn1 [ 2 ] .butncode = '';
      butn1 [ 3 ] .sw = 1;
      strcpy ( butn1 [ 3 ] .title , ( char * ) "!z32#" ) ;
      butn1 [ 3 ] .xpmn = Bimgu;
      butn1 [ 3 ] .xpmp = NULL;
      butn1 [ 3 ] .xpmh = NULL;
      butn1 [ 3 ] .bkgr = -1;
      butn1 [ 3 ] .butncode = '';
      butn1 [ 4 ] .sw = 1;
      strcpy ( butn1 [ 4 ] .title , ( char * ) "!z32$" ) ;
      butn1 [ 4 ] .xpmn = Bimgu;
      butn1 [ 4 ] .xpmp = NULL;
      butn1 [ 4 ] .xpmh = NULL;
      butn1 [ 4 ] .bkgr = -1;
      butn1 [ 4 ] .butncode = '';
      butn1 [ 5 ] .sw = 1;
      strcpy ( butn1 [ 5 ] .title , ( char * ) "!z32%" ) ;
      butn1 [ 5 ] .xpmn = Bimgu;
      butn1 [ 5 ] .xpmp = NULL;
      butn1 [ 5 ] .xpmh = NULL;
      butn1 [ 5 ] .bkgr = -1;
      butn1 [ 5 ] .butncode = '';
      butn1 [ 6 ] .sw = 1;
      strcpy ( butn1 [ 6 ] .title , ( char * ) "!z32^" ) ;
      butn1 [ 6 ] .xpmn = Bimgu;
      butn1 [ 6 ] .xpmp = NULL;
      butn1 [ 6 ] .xpmh = NULL;
      butn1 [ 6 ] .bkgr = -1;
      butn1 [ 6 ] .butncode = '';
      butn1 [ 7 ] .sw = 1;
      strcpy ( butn1 [ 7 ] .title , ( char * ) "!z32&" ) ;
      butn1 [ 7 ] .xpmn = Bimgu;
      butn1 [ 7 ] .xpmp = NULL;
      butn1 [ 7 ] .xpmh = NULL;
      butn1 [ 7 ] .bkgr = -1;
      butn1 [ 7 ] .butncode = '';
      butn1 [ 8 ] .sw = 1;
      strcpy ( butn1 [ 8 ] .title , ( char * ) "!z31!y!y!y*" ) ;
      butn1 [ 8 ] .xpmn = Bimgu;
      butn1 [ 8 ] .xpmp = NULL;
      butn1 [ 8 ] .xpmh = NULL;
      butn1 [ 8 ] .bkgr = -1;
      butn1 [ 8 ] .butncode = '';
      butn1 [ 9 ] .sw = 1;
      strcpy ( butn1 [ 9 ] .title , ( char * ) "!z32(" ) ;
      butn1 [ 9 ] .xpmn = Bimgu;
      butn1 [ 9 ] .xpmp = NULL;
      butn1 [ 9 ] .xpmh = NULL;
      butn1 [ 9 ] .bkgr = -1;
      butn1 [ 9 ] .butncode = '';
      butn1 [ 10 ] .sw = 1;
      strcpy ( butn1 [ 10 ] .title , ( char * ) "!z32)" ) ;
      butn1 [ 10 ] .xpmn = Bimgu;
      butn1 [ 10 ] .xpmp = NULL;
      butn1 [ 10 ] .xpmh = NULL;
      butn1 [ 10 ] .bkgr = -1;
      butn1 [ 10 ] .butncode = '';
      butn1 [ 11 ] .sw = 1;
      strcpy ( butn1 [ 11 ] .title , ( char * ) "!z32_" ) ;
      butn1 [ 11 ] .xpmn = Bimgu;
      butn1 [ 11 ] .xpmp = NULL;
      butn1 [ 11 ] .xpmh = NULL;
      butn1 [ 11 ] .bkgr = -1;
      butn1 [ 11 ] .butncode = '';
      butn1 [ 12 ] .sw = 1;
      strcpy ( butn1 [ 12 ] .title , ( char * ) "!z32+" ) ;
      butn1 [ 12 ] .xpmn = Bimgu;
      butn1 [ 12 ] .xpmp = NULL;
      butn1 [ 12 ] .xpmh = NULL;
      butn1 [ 12 ] .bkgr = -1;
      butn1 [ 12 ] .butncode = '';
      butn1 [ 13 ] .sw = 1;
      strcpy ( butn1 [ 13 ] .title , ( char * ) "!z32`" ) ;
      butn1 [ 13 ] .xpmn = Bimgu;
      butn1 [ 13 ] .xpmp = NULL;
      butn1 [ 13 ] .xpmh = NULL;
      butn1 [ 13 ] .bkgr = -1;
      butn1 [ 13 ] .butncode = '';
      butn1 [ 14 ] .sw = 1;
      strcpy ( butn1 [ 14 ] .title , ( char * ) "!z321" ) ;
      butn1 [ 14 ] .xpmn = Bimgu;
      butn1 [ 14 ] .xpmp = NULL;
      butn1 [ 14 ] .xpmh = NULL;
      butn1 [ 14 ] .bkgr = -1;
      butn1 [ 14 ] .butncode = '';
      butn1 [ 15 ] .sw = 1;
      strcpy ( butn1 [ 15 ] .title , ( char * ) "!z322" ) ;
      butn1 [ 15 ] .xpmn = Bimgu;
      butn1 [ 15 ] .xpmp = NULL;
      butn1 [ 15 ] .xpmh = NULL;
      butn1 [ 15 ] .bkgr = -1;
      butn1 [ 15 ] .butncode = '';
      butn1 [ 16 ] .sw = 1;
      strcpy ( butn1 [ 16 ] .title , ( char * ) "!z323" ) ;
      butn1 [ 16 ] .xpmn = Bimgu;
      butn1 [ 16 ] .xpmp = NULL;
      butn1 [ 16 ] .xpmh = NULL;
      butn1 [ 16 ] .bkgr = -1;
      butn1 [ 16 ] .butncode = '';
      butn1 [ 17 ] .sw = 1;
      strcpy ( butn1 [ 17 ] .title , ( char * ) "!z324" ) ;
      butn1 [ 17 ] .xpmn = Bimgu;
      butn1 [ 17 ] .xpmp = NULL;
      butn1 [ 17 ] .xpmh = NULL;
      butn1 [ 17 ] .bkgr = -1;
      butn1 [ 17 ] .butncode = '';
      butn1 [ 18 ] .sw = 1;
      strcpy ( butn1 [ 18 ] .title , ( char * ) "!z325" ) ;
      butn1 [ 18 ] .xpmn = Bimgu;
      butn1 [ 18 ] .xpmp = NULL;
      butn1 [ 18 ] .xpmh = NULL;
      butn1 [ 18 ] .bkgr = -1;
      butn1 [ 18 ] .butncode = '';
      butn1 [ 19 ] .sw = 1;
      strcpy ( butn1 [ 19 ] .title , ( char * ) "!z326" ) ;
      butn1 [ 19 ] .xpmn = Bimgu;
      butn1 [ 19 ] .xpmp = NULL;
      butn1 [ 19 ] .xpmh = NULL;
      butn1 [ 19 ] .bkgr = -1;
      butn1 [ 19 ] .butncode = '';
      butn1 [ 20 ] .sw = 1;
      strcpy ( butn1 [ 20 ] .title , ( char * ) "!z327" ) ;
      butn1 [ 20 ] .xpmn = Bimgu;
      butn1 [ 20 ] .xpmp = NULL;
      butn1 [ 20 ] .xpmh = NULL;
      butn1 [ 20 ] .bkgr = -1;
      butn1 [ 20 ] .butncode = '';
      butn1 [ 21 ] .sw = 1;
      strcpy ( butn1 [ 21 ] .title , ( char * ) "!z328" ) ;
      butn1 [ 21 ] .xpmn = Bimgu;
      butn1 [ 21 ] .xpmp = NULL;
      butn1 [ 21 ] .xpmh = NULL;
      butn1 [ 21 ] .bkgr = -1;
      butn1 [ 21 ] .butncode = '';
      butn1 [ 22 ] .sw = 1;
      strcpy ( butn1 [ 22 ] .title , ( char * ) "!z329" ) ;
      butn1 [ 22 ] .xpmn = Bimgu;
      butn1 [ 22 ] .xpmp = NULL;
      butn1 [ 22 ] .xpmh = NULL;
      butn1 [ 22 ] .bkgr = -1;
      butn1 [ 22 ] .butncode = '';
      butn1 [ 23 ] .sw = 1;
      strcpy ( butn1 [ 23 ] .title , ( char * ) "!z320" ) ;
      butn1 [ 23 ] .xpmn = Bimgu;
      butn1 [ 23 ] .xpmp = NULL;
      butn1 [ 23 ] .xpmh = NULL;
      butn1 [ 23 ] .bkgr = -1;
      butn1 [ 23 ] .butncode = '';
      butn1 [ 24 ] .sw = 1;
      strcpy ( butn1 [ 24 ] .title , ( char * ) "!z32-" ) ;
      butn1 [ 24 ] .xpmn = Bimgu;
      butn1 [ 24 ] .xpmp = NULL;
      butn1 [ 24 ] .xpmh = NULL;
      butn1 [ 24 ] .bkgr = -1;
      butn1 [ 24 ] .butncode = '';
      butn1 [ 25 ] .sw = 1;
      strcpy ( butn1 [ 25 ] .title , ( char * ) "!z32=" ) ;
      butn1 [ 25 ] .xpmn = Bimgu;
      butn1 [ 25 ] .xpmp = NULL;
      butn1 [ 25 ] .xpmh = NULL;
      butn1 [ 25 ] .bkgr = -1;
      butn1 [ 25 ] .butncode = '';
      DIN b1 = {
      'n' , 12 , 8 , 825 , 70 , 10 , 0 , 52 , 24 , 13 , 2 , 9 , 0.000000 , 0 , 0 , 0 , 1 , /* button type and roundinfg factor 
          ( 0-0.5 ) , bordr , hide , nodrawbkgr*/ butn1 , keybrdbutton1callback , /* Callbak */ NULL /* any args */ };
          
      strcpy ( b1.Wid , ( char * ) "keybrdWidget2" ) ;
      BUT_STR *butn2 = NULL;
      butn2 = ( BUT_STR * ) malloc ( sizeof ( BUT_STR ) *26 ) ;
      butn2 [ 0 ] .sw = 1;
      strcpy ( butn2 [ 0 ] .title , ( char * ) "!z32Q" ) ;
      butn2 [ 0 ] .xpmn = Bimgu;
      butn2 [ 0 ] .xpmp = NULL;
      butn2 [ 0 ] .xpmh = NULL;
      butn2 [ 0 ] .bkgr = -1;
      butn2 [ 0 ] .butncode = '';
      butn2 [ 1 ] .sw = 1;
      strcpy ( butn2 [ 1 ] .title , ( char * ) "!z32W" ) ;
      butn2 [ 1 ] .xpmn = Bimgu;
      butn2 [ 1 ] .xpmp = NULL;
      butn2 [ 1 ] .xpmh = NULL;
      butn2 [ 1 ] .bkgr = -1;
      butn2 [ 1 ] .butncode = '';
      butn2 [ 2 ] .sw = 1;
      strcpy ( butn2 [ 2 ] .title , ( char * ) "!z32E" ) ;
      butn2 [ 2 ] .xpmn = Bimgu;
      butn2 [ 2 ] .xpmp = NULL;
      butn2 [ 2 ] .xpmh = NULL;
      butn2 [ 2 ] .bkgr = -1;
      butn2 [ 2 ] .butncode = '';
      butn2 [ 3 ] .sw = 1;
      strcpy ( butn2 [ 3 ] .title , ( char * ) "!z32R" ) ;
      butn2 [ 3 ] .xpmn = Bimgu;
      butn2 [ 3 ] .xpmp = NULL;
      butn2 [ 3 ] .xpmh = NULL;
      butn2 [ 3 ] .bkgr = -1;
      butn2 [ 3 ] .butncode = '';
      butn2 [ 4 ] .sw = 1;
      strcpy ( butn2 [ 4 ] .title , ( char * ) "!z32T" ) ;
      butn2 [ 4 ] .xpmn = Bimgu;
      butn2 [ 4 ] .xpmp = NULL;
      butn2 [ 4 ] .xpmh = NULL;
      butn2 [ 4 ] .bkgr = -1;
      butn2 [ 4 ] .butncode = '';
      butn2 [ 5 ] .sw = 1;
      strcpy ( butn2 [ 5 ] .title , ( char * ) "!z32Y" ) ;
      butn2 [ 5 ] .xpmn = Bimgu;
      butn2 [ 5 ] .xpmp = NULL;
      butn2 [ 5 ] .xpmh = NULL;
      butn2 [ 5 ] .bkgr = -1;
      butn2 [ 5 ] .butncode = '';
      butn2 [ 6 ] .sw = 1;
      strcpy ( butn2 [ 6 ] .title , ( char * ) "!z32U" ) ;
      butn2 [ 6 ] .xpmn = Bimgu;
      butn2 [ 6 ] .xpmp = NULL;
      butn2 [ 6 ] .xpmh = NULL;
      butn2 [ 6 ] .bkgr = -1;
      butn2 [ 6 ] .butncode = '';
      butn2 [ 7 ] .sw = 1;
      strcpy ( butn2 [ 7 ] .title , ( char * ) "!z32I" ) ;
      butn2 [ 7 ] .xpmn = Bimgu;
      butn2 [ 7 ] .xpmp = NULL;
      butn2 [ 7 ] .xpmh = NULL;
      butn2 [ 7 ] .bkgr = -1;
      butn2 [ 7 ] .butncode = '';
      butn2 [ 8 ] .sw = 1;
      strcpy ( butn2 [ 8 ] .title , ( char * ) "!z32O" ) ;
      butn2 [ 8 ] .xpmn = Bimgu;
      butn2 [ 8 ] .xpmp = NULL;
      butn2 [ 8 ] .xpmh = NULL;
      butn2 [ 8 ] .bkgr = -1;
      butn2 [ 8 ] .butncode = '';
      butn2 [ 9 ] .sw = 1;
      strcpy ( butn2 [ 9 ] .title , ( char * ) "!z32P" ) ;
      butn2 [ 9 ] .xpmn = Bimgu;
      butn2 [ 9 ] .xpmp = NULL;
      butn2 [ 9 ] .xpmh = NULL;
      butn2 [ 9 ] .bkgr = -1;
      butn2 [ 9 ] .butncode = '';
      butn2 [ 10 ] .sw = 1;
      strcpy ( butn2 [ 10 ] .title , ( char * ) "!z32{" ) ;
      butn2 [ 10 ] .xpmn = Bimgu;
      butn2 [ 10 ] .xpmp = NULL;
      butn2 [ 10 ] .xpmh = NULL;
      butn2 [ 10 ] .bkgr = -1;
      butn2 [ 10 ] .butncode = '';
      butn2 [ 11 ] .sw = 1;
      strcpy ( butn2 [ 11 ] .title , ( char * ) "!z32}" ) ;
      butn2 [ 11 ] .xpmn = Bimgu;
      butn2 [ 11 ] .xpmp = NULL;
      butn2 [ 11 ] .xpmh = NULL;
      butn2 [ 11 ] .bkgr = -1;
      butn2 [ 11 ] .butncode = '';
      butn2 [ 12 ] .sw = 1;
      strcpy ( butn2 [ 12 ] .title , ( char * ) "!z32|" ) ;
      butn2 [ 12 ] .xpmn = Bimgu;
      butn2 [ 12 ] .xpmp = NULL;
      butn2 [ 12 ] .xpmh = NULL;
      butn2 [ 12 ] .bkgr = -1;
      butn2 [ 12 ] .butncode = '';
      butn2 [ 13 ] .sw = 1;
      strcpy ( butn2 [ 13 ] .title , ( char * ) "!z32q" ) ;
      butn2 [ 13 ] .xpmn = Bimgu;
      butn2 [ 13 ] .xpmp = NULL;
      butn2 [ 13 ] .xpmh = NULL;
      butn2 [ 13 ] .bkgr = -1;
      butn2 [ 13 ] .butncode = '';
      butn2 [ 14 ] .sw = 1;
      strcpy ( butn2 [ 14 ] .title , ( char * ) "!z32w" ) ;
      butn2 [ 14 ] .xpmn = Bimgu;
      butn2 [ 14 ] .xpmp = NULL;
      butn2 [ 14 ] .xpmh = NULL;
      butn2 [ 14 ] .bkgr = -1;
      butn2 [ 14 ] .butncode = '';
      butn2 [ 15 ] .sw = 1;
      strcpy ( butn2 [ 15 ] .title , ( char * ) "!z32e" ) ;
      butn2 [ 15 ] .xpmn = Bimgu;
      butn2 [ 15 ] .xpmp = NULL;
      butn2 [ 15 ] .xpmh = NULL;
      butn2 [ 15 ] .bkgr = -1;
      butn2 [ 15 ] .butncode = '';
      butn2 [ 16 ] .sw = 1;
      strcpy ( butn2 [ 16 ] .title , ( char * ) "!z32r" ) ;
      butn2 [ 16 ] .xpmn = Bimgu;
      butn2 [ 16 ] .xpmp = NULL;
      butn2 [ 16 ] .xpmh = NULL;
      butn2 [ 16 ] .bkgr = -1;
      butn2 [ 16 ] .butncode = '';
      butn2 [ 17 ] .sw = 1;
      strcpy ( butn2 [ 17 ] .title , ( char * ) "!z32t" ) ;
      butn2 [ 17 ] .xpmn = Bimgu;
      butn2 [ 17 ] .xpmp = NULL;
      butn2 [ 17 ] .xpmh = NULL;
      butn2 [ 17 ] .bkgr = -1;
      butn2 [ 17 ] .butncode = '';
      butn2 [ 18 ] .sw = 1;
      strcpy ( butn2 [ 18 ] .title , ( char * ) "!z32y" ) ;
      butn2 [ 18 ] .xpmn = Bimgu;
      butn2 [ 18 ] .xpmp = NULL;
      butn2 [ 18 ] .xpmh = NULL;
      butn2 [ 18 ] .bkgr = -1;
      butn2 [ 18 ] .butncode = '';
      butn2 [ 19 ] .sw = 1;
      strcpy ( butn2 [ 19 ] .title , ( char * ) "!z32u" ) ;
      butn2 [ 19 ] .xpmn = Bimgu;
      butn2 [ 19 ] .xpmp = NULL;
      butn2 [ 19 ] .xpmh = NULL;
      butn2 [ 19 ] .bkgr = -1;
      butn2 [ 19 ] .butncode = '';
      butn2 [ 20 ] .sw = 1;
      strcpy ( butn2 [ 20 ] .title , ( char * ) "!z32i" ) ;
      butn2 [ 20 ] .xpmn = Bimgu;
      butn2 [ 20 ] .xpmp = NULL;
      butn2 [ 20 ] .xpmh = NULL;
      butn2 [ 20 ] .bkgr = -1;
      butn2 [ 20 ] .butncode = '';
      butn2 [ 21 ] .sw = 1;
      strcpy ( butn2 [ 21 ] .title , ( char * ) "!z32o" ) ;
      butn2 [ 21 ] .xpmn = Bimgu;
      butn2 [ 21 ] .xpmp = NULL;
      butn2 [ 21 ] .xpmh = NULL;
      butn2 [ 21 ] .bkgr = -1;
      butn2 [ 21 ] .butncode = '';
      butn2 [ 22 ] .sw = 1;
      strcpy ( butn2 [ 22 ] .title , ( char * ) "!z32p" ) ;
      butn2 [ 22 ] .xpmn = Bimgu;
      butn2 [ 22 ] .xpmp = NULL;
      butn2 [ 22 ] .xpmh = NULL;
      butn2 [ 22 ] .bkgr = -1;
      butn2 [ 22 ] .butncode = '';
      butn2 [ 23 ] .sw = 1;
      strcpy ( butn2 [ 23 ] .title , ( char * ) "!z32[" ) ;
      butn2 [ 23 ] .xpmn = Bimgu;
      butn2 [ 23 ] .xpmp = NULL;
      butn2 [ 23 ] .xpmh = NULL;
      butn2 [ 23 ] .bkgr = -1;
      butn2 [ 23 ] .butncode = '';
      butn2 [ 24 ] .sw = 1;
      strcpy ( butn2 [ 24 ] .title , ( char * ) "!z32]" ) ;
      butn2 [ 24 ] .xpmn = Bimgu;
      butn2 [ 24 ] .xpmp = NULL;
      butn2 [ 24 ] .xpmh = NULL;
      butn2 [ 24 ] .bkgr = -1;
      butn2 [ 24 ] .butncode = '';
      butn2 [ 25 ] .sw = 1;
      strcpy ( butn2 [ 25 ] .title , ( char * ) "!z32\\" ) ;
      butn2 [ 25 ] .xpmn = Bimgu;
      butn2 [ 25 ] .xpmp = NULL;
      butn2 [ 25 ] .xpmh = NULL;
      butn2 [ 25 ] .bkgr = -1;
      butn2 [ 25 ] .butncode = '';
      DIN b2 = {
      'n' , 12 , 68 , 825 , 130 , 10 , 0 , 52 , 24 , 13 , 2 , 9 , 0.000000 , 0 , 0 , 0 , 1 , /* button type and roundinfg factor 
          ( 0-0.5 ) , bordr , hide , nodrawbkgr*/ butn2 , keybrdbutton2callback , /* Callbak */ NULL /* any args */ };
          
      strcpy ( b2.Wid , ( char * ) "keybrdWidget2" ) ;
      BUT_STR *butn3 = NULL;
      butn3 = ( BUT_STR * ) malloc ( sizeof ( BUT_STR ) *26 ) ;
      butn3 [ 0 ] .sw = 1;
      strcpy ( butn3 [ 0 ] .title , ( char * ) "!z32Clear" ) ;
      butn3 [ 0 ] .xpmn = Bimgu;
      butn3 [ 0 ] .xpmp = NULL;
      butn3 [ 0 ] .xpmh = NULL;
      butn3 [ 0 ] .bkgr = -1;
      butn3 [ 0 ] .butncode = '';
      butn3 [ 1 ] .sw = 1;
      strcpy ( butn3 [ 1 ] .title , ( char * ) "!z32A" ) ;
      butn3 [ 1 ] .xpmn = Bimgu;
      butn3 [ 1 ] .xpmp = NULL;
      butn3 [ 1 ] .xpmh = NULL;
      butn3 [ 1 ] .bkgr = -1;
      butn3 [ 1 ] .butncode = '';
      butn3 [ 2 ] .sw = 1;
      strcpy ( butn3 [ 2 ] .title , ( char * ) "!z32S" ) ;
      butn3 [ 2 ] .xpmn = Bimgu;
      butn3 [ 2 ] .xpmp = NULL;
      butn3 [ 2 ] .xpmh = NULL;
      butn3 [ 2 ] .bkgr = -1;
      butn3 [ 2 ] .butncode = '';
      butn3 [ 3 ] .sw = 1;
      strcpy ( butn3 [ 3 ] .title , ( char * ) "!z32D" ) ;
      butn3 [ 3 ] .xpmn = Bimgu;
      butn3 [ 3 ] .xpmp = NULL;
      butn3 [ 3 ] .xpmh = NULL;
      butn3 [ 3 ] .bkgr = -1;
      butn3 [ 3 ] .butncode = '';
      butn3 [ 4 ] .sw = 1;
      strcpy ( butn3 [ 4 ] .title , ( char * ) "!z32F" ) ;
      butn3 [ 4 ] .xpmn = Bimgu;
      butn3 [ 4 ] .xpmp = NULL;
      butn3 [ 4 ] .xpmh = NULL;
      butn3 [ 4 ] .bkgr = -1;
      butn3 [ 4 ] .butncode = '';
      butn3 [ 5 ] .sw = 1;
      strcpy ( butn3 [ 5 ] .title , ( char * ) "!z32G" ) ;
      butn3 [ 5 ] .xpmn = Bimgu;
      butn3 [ 5 ] .xpmp = NULL;
      butn3 [ 5 ] .xpmh = NULL;
      butn3 [ 5 ] .bkgr = -1;
      butn3 [ 5 ] .butncode = '';
      butn3 [ 6 ] .sw = 1;
      strcpy ( butn3 [ 6 ] .title , ( char * ) "!z32H" ) ;
      butn3 [ 6 ] .xpmn = Bimgu;
      butn3 [ 6 ] .xpmp = NULL;
      butn3 [ 6 ] .xpmh = NULL;
      butn3 [ 6 ] .bkgr = -1;
      butn3 [ 6 ] .butncode = '';
      butn3 [ 7 ] .sw = 1;
      strcpy ( butn3 [ 7 ] .title , ( char * ) "!z32J" ) ;
      butn3 [ 7 ] .xpmn = Bimgu;
      butn3 [ 7 ] .xpmp = NULL;
      butn3 [ 7 ] .xpmh = NULL;
      butn3 [ 7 ] .bkgr = -1;
      butn3 [ 7 ] .butncode = '';
      butn3 [ 8 ] .sw = 1;
      strcpy ( butn3 [ 8 ] .title , ( char * ) "!z32K" ) ;
      butn3 [ 8 ] .xpmn = Bimgu;
      butn3 [ 8 ] .xpmp = NULL;
      butn3 [ 8 ] .xpmh = NULL;
      butn3 [ 8 ] .bkgr = -1;
      butn3 [ 8 ] .butncode = '';
      butn3 [ 9 ] .sw = 1;
      strcpy ( butn3 [ 9 ] .title , ( char * ) "!z32L" ) ;
      butn3 [ 9 ] .xpmn = Bimgu;
      butn3 [ 9 ] .xpmp = NULL;
      butn3 [ 9 ] .xpmh = NULL;
      butn3 [ 9 ] .bkgr = -1;
      butn3 [ 9 ] .butncode = '';
      butn3 [ 10 ] .sw = 1;
      strcpy ( butn3 [ 10 ] .title , ( char * ) "!z32:" ) ;
      butn3 [ 10 ] .xpmn = Bimgu;
      butn3 [ 10 ] .xpmp = NULL;
      butn3 [ 10 ] .xpmh = NULL;
      butn3 [ 10 ] .bkgr = -1;
      butn3 [ 10 ] .butncode = '';
      butn3 [ 11 ] .sw = 1;
      strcpy ( butn3 [ 11 ] .title , ( char * ) "!z32\"" ) ;
      butn3 [ 11 ] .xpmn = Bimgu;
      butn3 [ 11 ] .xpmp = NULL;
      butn3 [ 11 ] .xpmh = NULL;
      butn3 [ 11 ] .bkgr = -1;
      butn3 [ 11 ] .butncode = '';
      butn3 [ 12 ] .sw = 1;
      strcpy ( butn3 [ 12 ] .title , ( char * ) "" ) ;
      butn3 [ 12 ] .xpmn = v [ 3 ] ;
      butn3 [ 12 ] .xpmp = NULL;
      butn3 [ 12 ] .xpmh = NULL;
      butn3 [ 12 ] .bkgr = -1;
      butn3 [ 12 ] .butncode = '';
      butn3 [ 13 ] .sw = 1;
      strcpy ( butn3 [ 13 ] .title , ( char * ) "!z32Escape" ) ;
      butn3 [ 13 ] .xpmn = Bimgu;
      butn3 [ 13 ] .xpmp = NULL;
      butn3 [ 13 ] .xpmh = NULL;
      butn3 [ 13 ] .bkgr = -1;
      butn3 [ 13 ] .butncode = '';
      butn3 [ 14 ] .sw = 1;
      strcpy ( butn3 [ 14 ] .title , ( char * ) "!z32a" ) ;
      butn3 [ 14 ] .xpmn = Bimgu;
      butn3 [ 14 ] .xpmp = NULL;
      butn3 [ 14 ] .xpmh = NULL;
      butn3 [ 14 ] .bkgr = -1;
      butn3 [ 14 ] .butncode = '';
      butn3 [ 15 ] .sw = 1;
      strcpy ( butn3 [ 15 ] .title , ( char * ) "!z32s" ) ;
      butn3 [ 15 ] .xpmn = Bimgu;
      butn3 [ 15 ] .xpmp = NULL;
      butn3 [ 15 ] .xpmh = NULL;
      butn3 [ 15 ] .bkgr = -1;
      butn3 [ 15 ] .butncode = '';
      butn3 [ 16 ] .sw = 1;
      strcpy ( butn3 [ 16 ] .title , ( char * ) "!z32d" ) ;
      butn3 [ 16 ] .xpmn = Bimgu;
      butn3 [ 16 ] .xpmp = NULL;
      butn3 [ 16 ] .xpmh = NULL;
      butn3 [ 16 ] .bkgr = -1;
      butn3 [ 16 ] .butncode = '';
      butn3 [ 17 ] .sw = 1;
      strcpy ( butn3 [ 17 ] .title , ( char * ) "!z32f" ) ;
      butn3 [ 17 ] .xpmn = Bimgu;
      butn3 [ 17 ] .xpmp = NULL;
      butn3 [ 17 ] .xpmh = NULL;
      butn3 [ 17 ] .bkgr = -1;
      butn3 [ 17 ] .butncode = '';
      butn3 [ 18 ] .sw = 1;
      strcpy ( butn3 [ 18 ] .title , ( char * ) "!z32g" ) ;
      butn3 [ 18 ] .xpmn = Bimgu;
      butn3 [ 18 ] .xpmp = NULL;
      butn3 [ 18 ] .xpmh = NULL;
      butn3 [ 18 ] .bkgr = -1;
      butn3 [ 18 ] .butncode = '';
      butn3 [ 19 ] .sw = 1;
      strcpy ( butn3 [ 19 ] .title , ( char * ) "!z32h" ) ;
      butn3 [ 19 ] .xpmn = Bimgu;
      butn3 [ 19 ] .xpmp = NULL;
      butn3 [ 19 ] .xpmh = NULL;
      butn3 [ 19 ] .bkgr = -1;
      butn3 [ 19 ] .butncode = '';
      butn3 [ 20 ] .sw = 1;
      strcpy ( butn3 [ 20 ] .title , ( char * ) "!z32j" ) ;
      butn3 [ 20 ] .xpmn = Bimgu;
      butn3 [ 20 ] .xpmp = NULL;
      butn3 [ 20 ] .xpmh = NULL;
      butn3 [ 20 ] .bkgr = -1;
      butn3 [ 20 ] .butncode = '';
      butn3 [ 21 ] .sw = 1;
      strcpy ( butn3 [ 21 ] .title , ( char * ) "!z32k" ) ;
      butn3 [ 21 ] .xpmn = Bimgu;
      butn3 [ 21 ] .xpmp = NULL;
      butn3 [ 21 ] .xpmh = NULL;
      butn3 [ 21 ] .bkgr = -1;
      butn3 [ 21 ] .butncode = '';
      butn3 [ 22 ] .sw = 1;
      strcpy ( butn3 [ 22 ] .title , ( char * ) "!z32l" ) ;
      butn3 [ 22 ] .xpmn = Bimgu;
      butn3 [ 22 ] .xpmp = NULL;
      butn3 [ 22 ] .xpmh = NULL;
      butn3 [ 22 ] .bkgr = -1;
      butn3 [ 22 ] .butncode = '';
      butn3 [ 23 ] .sw = 1;
      strcpy ( butn3 [ 23 ] .title , ( char * ) "!z32;" ) ;
      butn3 [ 23 ] .xpmn = Bimgu;
      butn3 [ 23 ] .xpmp = NULL;
      butn3 [ 23 ] .xpmh = NULL;
      butn3 [ 23 ] .bkgr = -1;
      butn3 [ 23 ] .butncode = '';
      butn3 [ 24 ] .sw = 1;
      strcpy ( butn3 [ 24 ] .title , ( char * ) "!z32." ) ;
      butn3 [ 24 ] .xpmn = Bimgu;
      butn3 [ 24 ] .xpmp = NULL;
      butn3 [ 24 ] .xpmh = NULL;
      butn3 [ 24 ] .bkgr = -1;
      butn3 [ 24 ] .butncode = '';
      butn3 [ 25 ] .sw = 1;
      strcpy ( butn3 [ 25 ] .title , ( char * ) "!z32Enter" ) ;
      butn3 [ 25 ] .xpmn = v [ 4 ] ;
      butn3 [ 25 ] .xpmp = NULL;
      butn3 [ 25 ] .xpmh = NULL;
      butn3 [ 25 ] .bkgr = -1;
      butn3 [ 25 ] .butncode = '';
      DIN b3 = {
      'n' , 12 , 128 , 825 , 190 , 10 , 0 , 52 , 24 , 13 , 2 , 9 , 0.000000 , 0 , 0 , 0 , 1 , /* button type and roundinfg factor 
          ( 0-0.5 ) , bordr , hide , nodrawbkgr*/ butn3 , keybrdbutton3callback , /* Callbak */ NULL /* any args */ };
          
      strcpy ( b3.Wid , ( char * ) "keybrdWidget3" ) ;
      BUT_STR *butn4 = NULL;
      butn4 = ( BUT_STR * ) malloc ( sizeof ( BUT_STR ) *26 ) ;
      butn4 [ 0 ] .sw = 1;
      strcpy ( butn4 [ 0 ] .title , ( char * ) "!z32PgUp" ) ;
      butn4 [ 0 ] .xpmn = Bimgu;
      butn4 [ 0 ] .xpmp = NULL;
      butn4 [ 0 ] .xpmh = NULL;
      butn4 [ 0 ] .bkgr = -1;
      butn4 [ 0 ] .butncode = '';
      butn4 [ 1 ] .sw = 1;
      strcpy ( butn4 [ 1 ] .title , ( char * ) "!z32Z" ) ;
      butn4 [ 1 ] .xpmn = Bimgu;
      butn4 [ 1 ] .xpmp = NULL;
      butn4 [ 1 ] .xpmh = NULL;
      butn4 [ 1 ] .bkgr = -1;
      butn4 [ 1 ] .butncode = '';
      butn4 [ 2 ] .sw = 1;
      strcpy ( butn4 [ 2 ] .title , ( char * ) "!z32X" ) ;
      butn4 [ 2 ] .xpmn = Bimgu;
      butn4 [ 2 ] .xpmp = NULL;
      butn4 [ 2 ] .xpmh = NULL;
      butn4 [ 2 ] .bkgr = -1;
      butn4 [ 2 ] .butncode = '';
      butn4 [ 3 ] .sw = 1;
      strcpy ( butn4 [ 3 ] .title , ( char * ) "!z32C" ) ;
      butn4 [ 3 ] .xpmn = Bimgu;
      butn4 [ 3 ] .xpmp = NULL;
      butn4 [ 3 ] .xpmh = NULL;
      butn4 [ 3 ] .bkgr = -1;
      butn4 [ 3 ] .butncode = '';
      butn4 [ 4 ] .sw = 1;
      strcpy ( butn4 [ 4 ] .title , ( char * ) "!z32V" ) ;
      butn4 [ 4 ] .xpmn = Bimgu;
      butn4 [ 4 ] .xpmp = NULL;
      butn4 [ 4 ] .xpmh = NULL;
      butn4 [ 4 ] .bkgr = -1;
      butn4 [ 4 ] .butncode = '';
      butn4 [ 5 ] .sw = 1;
      strcpy ( butn4 [ 5 ] .title , ( char * ) "!z32B" ) ;
      butn4 [ 5 ] .xpmn = Bimgu;
      butn4 [ 5 ] .xpmp = NULL;
      butn4 [ 5 ] .xpmh = NULL;
      butn4 [ 5 ] .bkgr = -1;
      butn4 [ 5 ] .butncode = '';
      butn4 [ 6 ] .sw = 1;
      strcpy ( butn4 [ 6 ] .title , ( char * ) "!z32N" ) ;
      butn4 [ 6 ] .xpmn = Bimgu;
      butn4 [ 6 ] .xpmp = NULL;
      butn4 [ 6 ] .xpmh = NULL;
      butn4 [ 6 ] .bkgr = -1;
      butn4 [ 6 ] .butncode = '';
      butn4 [ 7 ] .sw = 1;
      strcpy ( butn4 [ 7 ] .title , ( char * ) "!z32M" ) ;
      butn4 [ 7 ] .xpmn = Bimgu;
      butn4 [ 7 ] .xpmp = NULL;
      butn4 [ 7 ] .xpmh = NULL;
      butn4 [ 7 ] .bkgr = -1;
      butn4 [ 7 ] .butncode = '';
      butn4 [ 8 ] .sw = 1;
      strcpy ( butn4 [ 8 ] .title , ( char * ) "!z32Insert" ) ;
      butn4 [ 8 ] .xpmn = Bimgu;
      butn4 [ 8 ] .xpmp = NULL;
      butn4 [ 8 ] .xpmh = NULL;
      butn4 [ 8 ] .bkgr = -1;
      butn4 [ 8 ] .butncode = '';
      butn4 [ 9 ] .sw = 1;
      strcpy ( butn4 [ 9 ] .title , ( char * ) "" ) ;
      butn4 [ 9 ] .xpmn = ( char * ) v [ 1 ] ;
      butn4 [ 9 ] .xpmp = NULL;
      butn4 [ 9 ] .xpmh = NULL;
      butn4 [ 9 ] .bkgr = -1;
      butn4 [ 9 ] .butncode = '';
      butn4 [ 10 ] .sw = 1;
      strcpy ( butn4 [ 10 ] .title , ( char * ) "!z11!f35s" ) ;
      butn4 [ 10 ] .xpmn = Bimgu;
      butn4 [ 10 ] .xpmp = NULL;
      butn4 [ 10 ] .xpmh = NULL;
      butn4 [ 10 ] .bkgr = -1;
      butn4 [ 10 ] .butncode = '';
      butn4 [ 11 ] .sw = 1;
      strcpy ( butn4 [ 11 ] .title , ( char * ) "BackSpace" ) ;
      butn4 [ 11 ] .xpmn = Bimgu;
      butn4 [ 11 ] .xpmp = NULL;
      butn4 [ 11 ] .xpmh = NULL;
      butn4 [ 11 ] .bkgr = -1;
      butn4 [ 11 ] .butncode = '';
      butn4 [ 12 ] .sw = 1;
      strcpy ( butn4 [ 12 ] .title , ( char * ) "!z32Home" ) ;
      butn4 [ 12 ] .xpmn = Bimgu;
      butn4 [ 12 ] .xpmp = NULL;
      butn4 [ 12 ] .xpmh = NULL;
      butn4 [ 12 ] .bkgr = -1;
      butn4 [ 12 ] .butncode = '';
      butn4 [ 13 ] .sw = 1;
      strcpy ( butn4 [ 13 ] .title , ( char * ) "!z32PgDown" ) ;
      butn4 [ 13 ] .xpmn = Bimgu;
      butn4 [ 13 ] .xpmp = NULL;
      butn4 [ 13 ] .xpmh = NULL;
      butn4 [ 13 ] .bkgr = -1;
      butn4 [ 13 ] .butncode = '';
      butn4 [ 14 ] .sw = 1;
      strcpy ( butn4 [ 14 ] .title , ( char * ) "!z32z" ) ;
      butn4 [ 14 ] .xpmn = Bimgu;
      butn4 [ 14 ] .xpmp = NULL;
      butn4 [ 14 ] .xpmh = NULL;
      butn4 [ 14 ] .bkgr = -1;
      butn4 [ 14 ] .butncode = '';
      butn4 [ 15 ] .sw = 1;
      strcpy ( butn4 [ 15 ] .title , ( char * ) "!z32x" ) ;
      butn4 [ 15 ] .xpmn = Bimgu;
      butn4 [ 15 ] .xpmp = NULL;
      butn4 [ 15 ] .xpmh = NULL;
      butn4 [ 15 ] .bkgr = -1;
      butn4 [ 15 ] .butncode = '';
      butn4 [ 16 ] .sw = 1;
      strcpy ( butn4 [ 16 ] .title , ( char * ) "!z32c" ) ;
      butn4 [ 16 ] .xpmn = Bimgu;
      butn4 [ 16 ] .xpmp = NULL;
      butn4 [ 16 ] .xpmh = NULL;
      butn4 [ 16 ] .bkgr = -1;
      butn4 [ 16 ] .butncode = '';
      butn4 [ 17 ] .sw = 1;
      strcpy ( butn4 [ 17 ] .title , ( char * ) "!z32v" ) ;
      butn4 [ 17 ] .xpmn = Bimgu;
      butn4 [ 17 ] .xpmp = NULL;
      butn4 [ 17 ] .xpmh = NULL;
      butn4 [ 17 ] .bkgr = -1;
      butn4 [ 17 ] .butncode = '';
      butn4 [ 18 ] .sw = 1;
      strcpy ( butn4 [ 18 ] .title , ( char * ) "!z32b" ) ;
      butn4 [ 18 ] .xpmn = Bimgu;
      butn4 [ 18 ] .xpmp = NULL;
      butn4 [ 18 ] .xpmh = NULL;
      butn4 [ 18 ] .bkgr = -1;
      butn4 [ 18 ] .butncode = '';
      butn4 [ 19 ] .sw = 1;
      strcpy ( butn4 [ 19 ] .title , ( char * ) "!z32n" ) ;
      butn4 [ 19 ] .xpmn = Bimgu;
      butn4 [ 19 ] .xpmp = NULL;
      butn4 [ 19 ] .xpmh = NULL;
      butn4 [ 19 ] .bkgr = -1;
      butn4 [ 19 ] .butncode = '';
      butn4 [ 20 ] .sw = 1;
      strcpy ( butn4 [ 20 ] .title , ( char * ) "!z32m" ) ;
      butn4 [ 20 ] .xpmn = Bimgu;
      butn4 [ 20 ] .xpmp = NULL;
      butn4 [ 20 ] .xpmh = NULL;
      butn4 [ 20 ] .bkgr = -1;
      butn4 [ 20 ] .butncode = '';
      butn4 [ 21 ] .sw = 1;
      strcpy ( butn4 [ 21 ] .title , ( char * ) "!z32Space" ) ;
      butn4 [ 21 ] .xpmn = Bimgu;
      butn4 [ 21 ] .xpmp = NULL;
      butn4 [ 21 ] .xpmh = NULL;
      butn4 [ 21 ] .bkgr = -1;
      butn4 [ 21 ] .butncode = '';
      butn4 [ 22 ] .sw = 1;
      strcpy ( butn4 [ 22 ] .title , ( char * ) "" ) ;
      butn4 [ 22 ] .xpmn = v [ 2 ] ;
      butn4 [ 22 ] .xpmp = NULL;
      butn4 [ 22 ] .xpmh = NULL;
      butn4 [ 22 ] .bkgr = -1;
      butn4 [ 22 ] .butncode = '';
      butn4 [ 23 ] .sw = 1;
      strcpy ( butn4 [ 23 ] .title , ( char * ) "!z11!f35t" ) ;
      butn4 [ 23 ] .xpmn = Bimgu;
      butn4 [ 23 ] .xpmp = NULL;
      butn4 [ 23 ] .xpmh = NULL;
      butn4 [ 23 ] .bkgr = -1;
      butn4 [ 23 ] .butncode = '';
      butn4 [ 24 ] .sw = 1;
      strcpy ( butn4 [ 24 ] .title , ( char * ) "!z32Del" ) ;
      butn4 [ 24 ] .xpmn = Bimgu;
      butn4 [ 24 ] .xpmp = NULL;
      butn4 [ 24 ] .xpmh = NULL;
      butn4 [ 24 ] .bkgr = -1;
      butn4 [ 24 ] .butncode = '';
      butn4 [ 25 ] .sw = 1;
      strcpy ( butn4 [ 25 ] .title , ( char * ) "!z32End" ) ;
      butn4 [ 25 ] .xpmn = Bimgu;
      butn4 [ 25 ] .xpmp = NULL;
      butn4 [ 25 ] .xpmh = NULL;
      butn4 [ 25 ] .bkgr = -1;
      butn4 [ 25 ] .butncode = '';
      DIN b4 = {
      'n' , 12 , 188 , 825 , 250 , 10 , 0 , 52 , 24 , 13 , 2 , 9 , 0.000000 , 0 , 0 , 0 , 1 , /* button type and roundinfg factor 
          ( 0-0.5 ) , bordr , hide , nodrawbkgr*/ butn4 , keybrdbutton4callback , /* Callbak */ NULL /* any args */ };
          
      strcpy ( b4.Wid , ( char * ) "keybrdWidget4" ) ;
      dtmp = D-> d;
      i = 0;
      if ( dtmp != NULL ) while ( dtmp [ i ] .t != NULL ) i++;
      dtmp = ( DIA * ) realloc ( dtmp , sizeof ( DIA ) * ( i+6 ) ) ;
      d = dtmp+i;
      d [ 5 ] .t = NULL;
      d [ 0 ] .t = ( DIT * ) malloc ( sizeof ( DIP ) ) ;
      *d [ 0 ] .p = p0;
      d [ 0 ] .p-> item = -1;
      d [ 1 ] .t = ( DIT * ) malloc ( sizeof ( DIN ) ) ;
      keybrdbutton1init ( & b1 , pt ) ;
      *d [ 1 ] .N = b1;
      d [ 1 ] .N-> item = -1;
      d [ 2 ] .t = ( DIT * ) malloc ( sizeof ( DIN ) ) ;
      keybrdbutton2init ( & b2 , pt ) ;
      *d [ 2 ] .N = b2;
      d [ 2 ] .N-> item = -1;
      d [ 3 ] .t = ( DIT * ) malloc ( sizeof ( DIN ) ) ;
      keybrdbutton3init ( & b3 , pt ) ;
      *d [ 3 ] .N = b3;
      d [ 3 ] .N-> item = -1;
      d [ 4 ] .t = ( DIT * ) malloc ( sizeof ( DIN ) ) ;
      keybrdbutton4init ( & b4 , pt ) ;
      *d [ 4 ] .N = b4;
      d [ 4 ] .N-> item = -1;
      d [ 5 ] .t = NULL;
      GrpId = kgOpenGrp ( D ) ;
      D-> d = dtmp;
      j = 0;
      while ( d [ j ] .t != NULL ) { kgAddtoGrp ( D , GrpId , 
          ( void * ) ( d [ j ] .t ) ) ;j++;}
      return GrpId;
  }
/* One can also use the following code to add Widgets to an existing Dialog */
  int MakekeybrdGroup ( DIALOG *D , void *arg , int Red , int Green , int Blue , int Xo , int Yo ) 
      {
      int GrpId;
      void *Bimg = NULL;
      WIDGETGRP *Gpt;
/*************************************************


*************************************************/
      void **v = NULL;
      void *pt = NULL; /* pointer to send any extra information */ void *img , *Limg , *Rimg;
          
      v = ( void ** ) malloc ( sizeof ( void * ) *6 ) ;
      Bimg = GetButImage ( 52 , 24 , Red , Green , Blue ) ;
      v [ 3 ] = GetButTopImage ( 52 , 24 , Red , Green , Blue ) ;
      v [ 4 ] = GetButBottomImage ( 52 , 24 , Red , Green , Blue ) ;
      kgGetDefaultRGB ( D-> gc.but_char , & Red , & Green , & Blue ) ;
      Limg = kgLeftImage ( 18 , Red , Green , Blue ) ;
      img = kgGetImageCopy ( NULL , Bimg ) ;
      img = kgMergeImages ( img , Limg , 0 , 0 ) ;
      kgFreeImage ( Limg ) ;
      Limg = img;
      Rimg = kgRightImage ( 18 , Red , Green , Blue ) ;
      img = kgGetImageCopy ( NULL , Bimg ) ;
      img = kgMergeImages ( img , Rimg , 0 , 0 ) ;
      kgFreeImage ( Rimg ) ;
      Rimg = img;
      v [ 0 ] = Bimg;
      v [ 1 ] = Limg;
      v [ 2 ] = Rimg;
      v [ 5 ] = NULL;
//   kgWriteImage(Bimg,"/root/but.png");
      GrpId = keybrdGroup ( D , v , arg ) ;
      Gpt = kgGetWidgetGrp ( D , GrpId ) ;
      Gpt-> arg = v;
      kgShiftGrp ( D , GrpId , Xo , Yo ) ;
      return GrpId;
  }
