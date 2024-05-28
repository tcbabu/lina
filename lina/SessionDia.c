#include <kulina.h>
#include "SessionDiaCallbacks.h"
#include "headers.h"
  extern LINACONFIG lc;
  extern int Session , Action , PowerDown;
  void ModifySessionDiaGc ( Gclr *gc ) {
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
  int SessionDiaGroup ( DIALOG *D , void **v , void *pt ) {
      int GrpId = 0 , oitems = 0 , i , j;
      DIA *d = NULL , *dtmp;
      char *xpm0 = NULL; /* pixmap info */ xpm0 = ( char * ) MakePixmapImage 
          ( 162 , 161 , lc.Red , lc.Green , lc.Blue , 0.05 ) ;
      DIP p0 = {
      'p' , 1 , 2 , 169 , 168 , ( void * ) xpm0 , -1 , /* bkgr colour */ 3 , 0 , 0.1 /* border hide */ };
          
      strcpy ( p0.Wid , ( char * ) "SessionDiaWidget2" ) ;
      char *menu1 [ ] = {
       ( char * ) "KDE" , ( char * ) "XFCE" , ( char * ) "Text Mode" , NULL };
      ThumbNail **th0 ;
      DIRA r1 = {
      'r' , 10 , 10 , 160 , 158 , 8 , 0 , 90 , 25 , 1 , 3 , 0 , 3 , ( int * ) v [ 0 ] , NULL , NULL , NULL , SessionDiabrowser1callback , /* *args , callback */ 4 , /* Border Offset */ 16 , /* Scroll width */ 0 , /* Type */ 0 , /* item highlight */ 1 , /* bordr */ 0 , /* bkgr */ 0 /*= 1 hide */ };
          
      r1.list = NULL;
      strcpy ( r1.Wid , ( char * ) "SessionDiaWidget1" ) ;
      dtmp = D-> d;
      i = 0;
      if ( dtmp != NULL ) while ( dtmp [ i ] .t != NULL ) i++;
      dtmp = ( DIA * ) realloc ( dtmp , sizeof ( DIA ) * ( i+3 ) ) ;
      d = dtmp+i;
      d [ 2 ] .t = NULL;
      d [ 0 ] .t = ( DIT * ) malloc ( sizeof ( DIP ) ) ;
      *d [ 0 ] .p = p0;
      d [ 0 ] .p-> item = -1;
      d [ 1 ] .t = ( DIT * ) malloc ( sizeof ( DIRA ) ) ;
      SessionDiabrowser1init ( & r1 , pt ) ;
      *d [ 1 ] .r = r1;
      d [ 1 ] .r-> item = -1;
      d [ 2 ] .t = NULL;
      GrpId = kgOpenGrp ( D ) ;
      D-> d = dtmp;
      j = 0;
      while ( d [ j ] .t != NULL ) { kgAddtoGrp ( D , GrpId , 
          ( void * ) ( d [ j ] .t ) ) ;j++;}
      return GrpId;
  }
/* One can also use the following code to add Widgets to an existing Dialog */
  int MakeSessionDiaGroup ( DIALOG *D , void *arg ) {
      int GrpId;
      WIDGETGRP *Gpt;
/*************************************************

    RadioButtons1  1 data value

*************************************************/
      int *v0 ;
      v0 = ( int * ) malloc ( sizeof ( int ) ) ;
      *v0 = 1;
      void** v = ( void ** ) malloc ( sizeof ( void* ) *2 ) ;
      v [ 1 ] = NULL;
      v [ 0 ] = ( void * ) ( v0 ) ;
      void *pt = NULL; /* pointer to send any extra information */ GrpId = SessionDiaGroup 
          ( D , v , pt ) ;
      Gpt = kgGetWidgetGrp ( D , GrpId ) ;
      Gpt-> arg = v;
      return GrpId;
  }
  int SessionDia ( void *parent , void **v , void *pt ) {
      int ret = 1 , GrpId , k;
      DIALOG D;
      DIA *d = NULL;
      D.VerId = 1401010200;
      kgInitUi ( & D ) ;
      D.d = NULL;
      GrpId = SessionDiaGroup ( & D , v , pt ) ;
      d = D.d;
      D.d = d;
      D.bkup = 1; /* set to 1 for backup */ D.bor_type = 4;
      D.df = 0;
      D.tw = 4;
      D.bw = 4;
      D.lw = 4;
      D.rw = 4;
      D.xo = 716; /* Position of Dialog */ D.yo = 161;
#if 1 
#else 
      D.xl = 174; /* Length of Dialog */ D.yl = 174; /* Width of Dialog */ D.Initfun = SessionDiainit;
           /* init fuction for Dialog */ D.Cleanupfun = SessionDiacleanup; /* init fuction for Dialog */ D.kbattn = 0;
           /* 1 for drawing keyborad attention */ D.butattn = 0; /* 1 for drawing button attention */ D.fullscreen = 0;
           /* 1 for for fullscreen mode */ D.Deco = 1; /* 1 for Window Decorration */ D.transparency = 1.000000;
           /* float 1.0 for full transparency */ D.Newwin = 0; /* 1 for new window not yet implemented */ D.DrawBkgr = 0;
           /* 1 for drawing background */ D.Bkpixmap = NULL; /* background image */ D.Sticky = 0;
           /* 1 for stickyness */ D.Resize = 0; /* 1 for Resize option */ D.MinWidth = 100;
           /* for Resize option */ D.MinHeight = 100; /* for Resize option */ D.Callback = SessionDiaCallBack;
           /* default callback */ D.Callback = NULL;
#endif
#if 1 
#else 
#endif
//  strcpy(D.name,"Kulina Designer ver 1.0");    /*  Dialog name you may change */
      D.ResizeCallback = SessionDiaResizeCallBack; /* Resize callback */ D.WaitCallback = NULL;
           /* Wait callback */ D.WaitCallback = SessionDiaWaitCallBack; /* Wait callback */ D.Fixpos = 1;
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
//  ModifySessionDiaGc(&(D.gc));    /*  set colors for gui*/
      } /* end of fullscreen mode */ D.gc = ( ( DIALOG * ) ( D.parent ) ) -> gc;
      ret = kgUi ( & D ) ;
      kgCleanUi ( & D ) ;
      return ret;
  }
  void *RunSessionDia ( void *arg ) {
/*************************************************

    RadioButtons1  1 data value

*************************************************/
      int v0 = 1;
      void* v [ 1 ] ;
      v0 = Session;
      v [ 0 ] = ( void * ) ( & v0 ) ;
      void *pt = NULL; /* pointer to send any extra information */ SessionDia 
          ( arg , v , pt ) ;
      return NULL;
  }
