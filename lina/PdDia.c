#include <kulina.h>
#include "PdDiaCallbacks.h"
#include "pimages.c"
#include "headers.h"
  extern int PowerDown;
  extern int Xres , Yres;
  extern LINACONFIG lc;
  void ModifyPdDiaGc ( Gclr *gc ) {
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
  int PdDiaGroup ( DIALOG *D , void **v , void *pt ) {
      int GrpId = 0 , oitems = 0 , i , j;
      DIA *d = NULL , *dtmp;
      char *xpm0 = NULL; /* pixmap info */ xpm0 = ( char * ) MakePixmapImage 
          ( 266 , 96 , lc.Red , lc.Green , lc.Blue , lc.fac ) ;
      DIP p0 = {
      'p' , 2 , 2 , 268 , 98 , ( void * ) xpm0 , -1 , /* bkgr colour */ 3 , 0 , 0.08 /* border hide */ };
          
      strcpy ( p0.Wid , ( char * ) "PdDiaWidget2" ) ;
      BUT_STR *butn1 = NULL;
      butn1 = ( BUT_STR * ) malloc ( sizeof ( BUT_STR ) *3 ) ;
      butn1 [ 0 ] .sw = 1;
      strcpy ( butn1 [ 0 ] .title , ( char * ) "Shutdown" ) ;
      butn1 [ 0 ] .xpmn = ( void * ) & shutdown_str;
      butn1 [ 0 ] .xpmp = NULL;
      butn1 [ 0 ] .xpmh = NULL;
      butn1 [ 0 ] .bkgr = -180180180;
      butn1 [ 0 ] .butncode = '';
      butn1 [ 1 ] .sw = 1;
      strcpy ( butn1 [ 1 ] .title , ( char * ) "Reboot" ) ;
      butn1 [ 1 ] .xpmn = ( void * ) & reboot_str;
      butn1 [ 1 ] .xpmp = NULL;
      butn1 [ 1 ] .xpmh = NULL;
      butn1 [ 1 ] .bkgr = -180180180;
      butn1 [ 1 ] .butncode = '';
      butn1 [ 2 ] .sw = 1;
      strcpy ( butn1 [ 2 ] .title , ( char * ) "Cancel" ) ;
      butn1 [ 2 ] .xpmn = ( void * ) & cancel_str;
      butn1 [ 2 ] .xpmp = NULL;
      butn1 [ 2 ] .xpmh = NULL;
      butn1 [ 2 ] .bkgr = -180180180;
      butn1 [ 2 ] .butncode = '';
      DIL h1 = {
      'h' , 9 , 14 , 256 , 76 , 38 , 18 , 46 , 46 , 3 , 1 , 0 , 0.500000 , 0 , 0 , 0 , 1 , /* button type and roundinfg factor 
          ( 0-0.5 ) , bordr , hide , nodrawbkgr*/ butn1 , PdDiasplbutton1callback , /* Callbak */ NULL /* any args */ };
          
      strcpy ( h1.Wid , ( char * ) "PgDiaWidget1" ) ;
      dtmp = D-> d;
      i = 0;
      if ( dtmp != NULL ) while ( dtmp [ i ] .t != NULL ) i++;
      dtmp = ( DIA * ) realloc ( dtmp , sizeof ( DIA ) * ( i+3 ) ) ;
      d = dtmp+i;
      d [ 2 ] .t = NULL;
      d [ 0 ] .t = ( DIT * ) malloc ( sizeof ( DIP ) ) ;
      *d [ 0 ] .p = p0;
      d [ 0 ] .p-> item = -1;
      d [ 1 ] .t = ( DIT * ) malloc ( sizeof ( DIL ) ) ;
      PdDiasplbutton1init ( & h1 , pt ) ;
      *d [ 1 ] .h = h1;
      d [ 1 ] .h-> item = -1;
      d [ 2 ] .t = NULL;
      GrpId = kgOpenGrp ( D ) ;
      D-> d = dtmp;
      j = 0;
      while ( d [ j ] .t != NULL ) { kgAddtoGrp ( D , GrpId , 
          ( void * ) ( d [ j ] .t ) ) ;j++;}
      return GrpId;
  }
/* One can also use the following code to add Widgets to an existing Dialog */
  int MakePdDiaGroup ( DIALOG *D , void *arg ) {
      int GrpId;
      WIDGETGRP *Gpt;
/*************************************************


*************************************************/
      void **v = NULL;
      void *pt = NULL; /* pointer to send any extra information */ GrpId = PdDiaGroup 
          ( D , v , pt ) ;
      Gpt = kgGetWidgetGrp ( D , GrpId ) ;
      Gpt-> arg = v;
      return GrpId;
  }
  int PdDia ( void *parent , void **v , void *pt ) {
      int ret = 1 , GrpId , k;
      DIALOG D;
      DIA *d = NULL;
      D.VerId = 1401010200;
      kgInitUi ( & D ) ;
      D.d = NULL;
      GrpId = PdDiaGroup ( & D , v , pt ) ;
      d = D.d;
      D.d = d;
      D.bkup = 1; /* set to 1 for backup */ D.bor_type = 4;
      D.df = 0;
      D.tw = 4;
      D.bw = 4;
      D.lw = 4;
      D.rw = 4;
      D.xo = 466; /* Position of Dialog */ D.yo = 340;
#if 1 
#else 
      D.xl = 270; /* Length of Dialog */ D.yl = 100; /* Width of Dialog */ D.Initfun = PdDiainit;
           /* init fuction for Dialog */ D.Cleanupfun = PdDiacleanup; /* init fuction for Dialog */ D.kbattn = 0;
           /* 1 for drawing keyborad attention */ D.butattn = 0; /* 1 for drawing button attention */ D.fullscreen = 0;
           /* 1 for for fullscreen mode */ D.Deco = 1; /* 1 for Window Decorration */ D.transparency = 0.000000;
           /* float 1.0 for full transparency */ D.Newwin = 0; /* 1 for new window not yet implemented */ D.DrawBkgr = 0;
           /* 1 for drawing background */ D.Bkpixmap = NULL; /* background image */ D.Sticky = 0;
           /* 1 for stickyness */ D.Resize = 0; /* 1 for Resize option */ D.MinWidth = 40;
           /* for Resize option */ D.MinHeight = 40; /* for Resize option */ D.Callback = PdDiaCallBack;
           /* default callback */ D.Callback = NULL;
#endif
#if 1 
#else 
#endif
//  strcpy(D.name,"Kulina Designer ver 1.0");    /*  Dialog name you may change */
      D.ResizeCallback = PdDiaResizeCallBack; /* Resize callback */ D.WaitCallback = NULL;
           /* Wait callback */ D.WaitCallback = PdDiaWaitCallBack; /* Wait callback */ D.Fixpos = 1;
           /* 1 for Fixing Position */ D.NoTaskBar = 0; /* 1 for not showing in task bar*/ D.StackPos = 0;
           /* -1 , 0 , 1 for for Stack Position -1:below 0:normal 1:above*/ D.Shapexpm = NULL;
           /* PNG/jpeg file for window shape;Black color will not be drawn */ D.parent = parent;
           /* 1 for not showing in task bar*/ D.pt = pt; /* any data to be passed by user*/ if 
          ( D.fullscreen != 1 ) { /* if not fullscreen mode */ int xres , yres;
          kgDisplaySize ( & xres , & yres ) ;
      // D.xo=D.yo=0; D.xl = xres-10; D.yl=yres-80;
          D.xo = ( xres-D.xl ) /2;
          D.yo = ( yres -D.yl ) /2;
          if ( yres < 770 ) D.yo = yres -385 - D.yl/2;
      }
      else {
          int xres , yres;
          kgDisplaySize ( & xres , & yres ) ;
          D.xo = D.yo = 0; D.xl = xres; D.yl = yres;
//     D.StackPos = 1; // you may need it
//  kgColorTheme(&D,210,210,210);    /*  set colors for gui*/
//  ModifyPdDiaGc(&(D.gc));    /*  set colors for gui*/
//  Print_gui_data(&D,"/root/PdDia.rc");
      } /* end of fullscreen mode */ D.gc = ( ( DIALOG * ) ( D.parent ) ) -> gc;
      ret = kgUi ( & D ) ;
      kgCleanUi ( & D ) ;
      return ret;
  }
  void *RunPdDia ( void *arg ) {
/*************************************************


*************************************************/
      void **v = NULL;
      void *pt = NULL; /* pointer to send any extra information */ PdDia 
          ( arg , v , pt ) ;
      return NULL;
  }
