#include <kulina.h>
#include "UserDiaCallbacks.h"
#include "headers.h"
  extern LINACONFIG lc;
  extern int Index;
  void ModifyUserDiaGc ( Gclr *gc ) {
/*
//  You may change default settings here 
//  probably you can allow the user to create a config in $HOME
//  and try to read that file (if exits); so dynamic configuration is possible
   gc->menu_char = 101;
   gc->but_char  =101;
   gc->txt_char  =101;
   gc->char_clr = 101;
   gc->FontSize =8;
   gc->Font=23;
*/
  }
  int UserDiaGroup_o ( DIALOG *D , void **v , void *pt ) {
      int GrpId = 0 , oitems = 0 , i , j;
      DIA *d = NULL , *dtmp;
      DIX x0 = {
      'x' , 4 , 5 , 215 , 246 , 10 , 2 , 112 , 48 , 1 , 1 , 0 , 4 , ( int * ) v [ 0 ] , NULL , NULL , NULL , UserDiabrowser1callback , /* *args , callback */ 6 , /* Border Offset */ 18 , /* Scroll width */ 12 , /* Type */ 1 , /* item highlight */ 1 , /* bordr */ 0 , /* bkgr */ 0 /*= 1 hide */ };
          
      strcpy ( x0.Wid , ( char * ) "UserDiaWidget3" ) ;
      dtmp = D-> d;
      i = 0;
      if ( dtmp != NULL ) while ( dtmp [ i ] .t != NULL ) i++;
      dtmp = ( DIA * ) realloc ( dtmp , sizeof ( DIA ) * ( i+2 ) ) ;
      d = dtmp+i;
      d [ 1 ] .t = NULL;
      d [ 0 ] .t = ( DIT * ) malloc ( sizeof ( DIX ) ) ;
      UserDiabrowser1init ( & x0 , pt ) ;
      *d [ 0 ] .x = x0;
      d [ 0 ] .x-> item = -1;
      d [ 1 ] .t = NULL;
      GrpId = kgOpenGrp ( D ) ;
      D-> d = dtmp;
      j = 0;
      while ( d [ j ] .t != NULL ) { kgAddtoGrp ( D , GrpId , 
          ( void * ) ( d [ j ] .t ) ) ;j++;}
      return GrpId;
  }
  int UserDiaGroup ( DIALOG *D , void **v , void *pt ) {
      int GrpId = 0 , oitems = 0 , i , j;
      DIA *d = NULL , *dtmp;
//  xpm0 = (char *)MakePixmapImage( 210,242,lc.Red,lc.Green,lc.Blue,lc.fac);
      char *xpm0 = NULL; /* pixmap info */ xpm0 = ( char * ) MakePixmapImage 
          ( 210 , 242 , lc.Red , lc.Green , lc.Blue , 0.05 ) ;
      DIP p0 = {
      'p' , 3 , 4 , 217 , 246 , ( void * ) xpm0 , -1 , /* bkgr colour */ 3 , 0 , 0.1 /* border hide */ };
          
      strcpy ( p0.Wid , ( char * ) "UserDiaWidget2" ) ;
      DIX x1 = {
      'x' , 12 , 13 , 206 , 236 , 10 , 2 , 112 , 48 , 1 , 18254608 , 0 , 4 , 
          ( int * ) v [ 0 ] , NULL , NULL , NULL , UserDiabrowser1callback , /* *args , callback */ 6 , /* Border Offset */ 18 , /* Scroll width */ 12 , /* Type */ 1 , /* item highlight */ 1 , /* bordr */ 1 , /* bkgr */ 0 /*= 1 hide */ };
          
      strcpy ( x1.Wid , ( char * ) "UserDiaWidget3" ) ;
      dtmp = D-> d;
      i = 0;
      if ( dtmp != NULL ) while ( dtmp [ i ] .t != NULL ) i++;
      dtmp = ( DIA * ) realloc ( dtmp , sizeof ( DIA ) * ( i+3 ) ) ;
      d = dtmp+i;
      d [ 2 ] .t = NULL;
      d [ 0 ] .t = ( DIT * ) malloc ( sizeof ( DIP ) ) ;
      *d [ 0 ] .p = p0;
      d [ 0 ] .p-> item = -1;
      d [ 1 ] .t = ( DIT * ) malloc ( sizeof ( DIX ) ) ;
      UserDiabrowser1init ( & x1 , pt ) ;
      *d [ 1 ] .x = x1;
      d [ 1 ] .x-> item = -1;
      d [ 2 ] .t = NULL;
      GrpId = kgOpenGrp ( D ) ;
      D-> d = dtmp;
      j = 0;
      while ( d [ j ] .t != NULL ) { kgAddtoGrp ( D , GrpId , 
          ( void * ) ( d [ j ] .t ) ) ;j++;}
      return GrpId;
  }
/* One can also use the following code to add Widgets to an existing Dialog */
  int MakeUserDiaGroup ( DIALOG *D , void *arg ) {
      int GrpId;
      WIDGETGRP *Gpt;
/*************************************************

    Selectmenu1  1 data value

*************************************************/
      int *v0 ;
      v0 = ( int * ) malloc ( sizeof ( int ) ) ;
      *v0 = 1;
      void** v = ( void ** ) malloc ( sizeof ( void* ) *2 ) ;
      v [ 1 ] = NULL;
      v [ 0 ] = ( void * ) ( v0 ) ;
      void *pt = NULL; /* pointer to send any extra information */ GrpId = UserDiaGroup 
          ( D , v , pt ) ;
      Gpt = kgGetWidgetGrp ( D , GrpId ) ;
      Gpt-> arg = v;
      return GrpId;
  }
  int UserDia ( void *parent , void **v , void *pt ) {
      int ret = 1 , GrpId , k;
      DIALOG D;
      DIA *d = NULL;
      D.VerId = 1401010200;
      kgInitUi ( & D ) ;
      D.d = NULL;
      GrpId = UserDiaGroup ( & D , v , pt ) ;
      d = D.d;
      D.d = d;
      D.bkup = 1; /* set to 1 for backup */ D.bor_type = 4;
      D.df = 0;
      D.tw = 4;
      D.bw = 4;
      D.lw = 4;
      D.rw = 4;
      D.xo = 522; /* Position of Dialog */ D.yo = 218;
#if 1 
#else 
      D.xl = 219; /* Length of Dialog */ D.yl = 251; /* Width of Dialog */ D.Initfun = UserDiainit;
           /* init fuction for Dialog */ D.Cleanupfun = UserDiacleanup; /* init fuction for Dialog */ D.kbattn = 0;
           /* 1 for drawing keyborad attention */ D.butattn = 0; /* 1 for drawing button attention */ D.fullscreen = 0;
           /* 1 for for fullscreen mode */ D.Deco = 1; /* 1 for Window Decorration */ D.transparency = 1.000000;
           /* float 1.0 for full transparency */ D.Newwin = 0; /* 1 for new window not yet implemented */ D.DrawBkgr = 0;
           /* 1 for drawing background */ D.Bkpixmap = NULL; /* background image */ D.Sticky = 0;
           /* 1 for stickyness */ D.Resize = 0; /* 1 for Resize option */ D.MinWidth = 100;
           /* for Resize option */ D.MinHeight = 100; /* for Resize option */ D.Callback = UserDiaCallBack;
           /* default callback */ D.Callback = NULL;
#endif
#if 1 
#else 
#endif
//  strcpy(D.name,"Kulina Designer ver 1.0");    /*  Dialog name you may change */
      D.ResizeCallback = UserDiaResizeCallBack; /* Resize callback */ D.WaitCallback = NULL;
           /* Wait callback */ D.WaitCallback = UserDiaWaitCallBack; /* Wait callback */ D.Fixpos = 1;
           /* 1 for Fixing Position */ D.NoTaskBar = 0; /* 1 for not showing in task bar*/ D.StackPos = 0;
           /* -1 , 0 , 1 for for Stack Position -1:below 0:normal 1:above*/ D.Shapexpm = NULL;
           /* PNG/jpeg file for window shape;Black color will not be drawn */ D.parent = parent;
           /* 1 for not showing in task bar*/ D.pt = pt; /* any data to be passed by user*/ if 
          ( D.fullscreen != 1 ) { /* if not fullscreen mode */ int xres , yres;
          kgDisplaySize ( & xres , & yres ) ;
      // D.xo=D.yo=0; D.xl = xres-10; D.yl=yres-80;
          D.xo = ( xres-D.xl ) /2;
          D.yo = ( yres -D.yl ) /2;
      }
      else {
          int xres , yres;
          kgDisplaySize ( & xres , & yres ) ;
          D.xo = D.yo = 0; D.xl = xres; D.yl = yres;
//     D.StackPos = 1; // you may need it
//  kgColorTheme(&D,210,210,210);    /*  set colors for gui*/
//  ModifyUserDiaGc(&(D.gc));    /*  set colors for gui*/
      } /* end of fullscreen mode */ D.gc = ( ( DIALOG * ) ( D.parent ) ) -> gc;
      ret = kgUi ( & D ) ;
      kgCleanUi ( & D ) ;
      return ret;
  }
  void *RunUserDia ( void *arg ) {
/*************************************************

    Selectmenu1  1 data value

*************************************************/
      int v0 = 1;
      void* v [ 1 ] ;
      v [ 0 ] = ( void * ) ( & v0 ) ;
      void *pt = NULL; /* pointer to send any extra information */ v0 = Index;
      UserDia ( arg , v , pt ) ;
      return NULL;
  }
