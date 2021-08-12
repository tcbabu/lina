#include "kulina.h"
#include "addnetdiaCallbacks.h"
#include "headers.h"
extern LINACONFIG lc;
void ModifyaddnetdiaGc(Gclr *gc) {
/*
//  You may change default settings here 
//  probably you can allow the user to create a config in $HOME
//  and try to read that file (if exits); so dynamic configuration is possible
   gc->FontSize =8;
   gc->Font=23;
*/
}
int addnetdiaGroup( DIALOG *D,void **v,void *pt) {
  int GrpId=0,oitems=0,i,j;
  DIA *d=NULL,*dtmp;
//  char *xpm0=   (char *)MakePixmapImage( 378,125,lc.Red,lc.Green,lc.Blue,0.05);
  char *xpm0=   (char *)MakePixmapImage( 518,330,lc.Red,lc.Green,lc.Blue,0.05);
  DIP p0 = { 
    'p',
    2,2,  
//    383,130,  
    520,332,  
    (void *)xpm0,
    -1, /* bkgr colour */ 
      3,0,0.1 /* border hide */ 
  };
  strcpy(p0.Wid,(char *)"addnetdiaWidget5");
  T_ELMT *e1  ; 
  e1 =(T_ELMT *)malloc(sizeof(T_ELMT)*2);
  e1[0].fmt = (char *)malloc(9);
  strcpy(e1[0].fmt,(char *)"SSID%30s");
  e1[0].v=(void *)v[0];
  e1[0].sw=1;
  e1[1].fmt = (char *)malloc(8);
  strcpy(e1[1].fmt,(char *)"PSK%30s");
  e1[1].v=(void *)v[1];
  e1[1].sw=1;
  e1[1].noecho=1;
  DIT t1 = { 
    't',
    13,16,  
    372,80,
    20, 
    1,2, 
    e1,
    1,1,
    NULL,addnetdiatextbox1callback,0,0,18,9 /* args,Call back */
  };
  strcpy(t1.Wid,(char *)"addnetdiaWidget7");
  t1.type =1;
  t1.pt=NULL;
  BUT_STR  *butn2=NULL; 
  butn2= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn2[0].sw=1;
  strcpy(butn2[0].title,(char *)"Okay");
  butn2[0].xpmn=NULL;
  butn2[0].xpmp=NULL;
  butn2[0].xpmh=NULL;
  butn2[0].bkgr=-1;
  butn2[0].butncode='';
  DIL h2 = { 
    'h',
    157,90,  
    239,123,
    2,0,  
    72, 
    25, 
    1,1, 
    4,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn2, 
    addnetdiasplbutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(h2.Wid,(char *)"addnetdiaWidget8");
  dtmp = D->d;
  i=0;
  if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
  dtmp = (DIA *)realloc(dtmp,sizeof(DIA )*(i+4));
  d =dtmp+i; 
  d[3].t=NULL;
  d[0].t = (DIT *)malloc(sizeof(DIP));
  *d[0].p = p0;
  d[0].p->item = -1;
  d[1].t = (DIT *)malloc(sizeof(DIT));
  *d[1].t = t1;
  d[1].t->item = -1;
  d[2].t = (DIT *)malloc(sizeof(DIL));
  addnetdiasplbutton1init(&h2,pt) ;
  *d[2].h = h2;
  d[2].h->item = -1;
  d[3].t = NULL;
  GrpId=kgOpenGrp(D);
  D->d = dtmp;
  j=0;
  while(d[j].t!=NULL){ kgAddtoGrp(D,GrpId,(void *)(d[j].t));j++;}
  return GrpId;
} 

/* One can also use the following code to add Widgets to an existing Dialog */

int MakeaddnetdiaGroup(DIALOG *D,void *arg) {
   int GrpId;
   WIDGETGRP *Gpt;
/*************************************************

    Text_Box1  2 data values

*************************************************/
   char  *v0 ;
   v0 = (char *)malloc(sizeof(100));
   v0[0] = '\0';
   char  *v1 ;
   v1 = (char *)malloc(sizeof(100));
   v1[0] = '\0';
   void** v=(void **)malloc(sizeof(void*)*3);
   v[2]=NULL;
   v[0]=(void *)(v0);
   v[1]=(void *)(v1);
   void *pt=NULL; /* pointer to send any extra information */
   GrpId = addnetdiaGroup(D,v,pt);
   Gpt = kgGetWidgetGrp(D,GrpId);
   Gpt->arg= v;
   return GrpId;
}

int addnetdia( void *parent,void **v,void *pt) {
  int ret=1,GrpId,k;
  DIALOG D;
  DIA *d=NULL;
  D.VerId=1401010200;
  kgInitUi(&D);
  D.d=NULL;
  GrpId = addnetdiaGroup(&D,v,pt);
  d = D.d;
  D.d = d;
  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 0;
  D.df = 2;
  D.tw = 4;
  D.bw = 4;
  D.lw = 4;
  D.rw = 4;
  D.xo = 475;   /* Position of Dialog */ 
  D.yo = 174;
  D.xl = 522;    /*  Length of Dialog */
  D.yl = 334;    /*  Width  of Dialog */
  D.Initfun = addnetdiainit;    /*   init fuction for Dialog */
  D.Cleanupfun = addnetdiacleanup;    /*   init fuction for Dialog */
  D.kbattn = 0;    /*  1 for drawing keyborad attention */
  D.butattn = 0;    /*  1 for drawing button attention */
  D.fullscreen = 0;    /*  1 for for fullscreen mode */
  D.Deco = 1;    /*  1 for Window Decorration */
  D.transparency = 1.0;
  D.Newwin = 0;    /*  1 for new window not yet implemented */
  D.DrawBkgr = 0;    /*  1 for drawing background */
  D.Bkpixmap = NULL;    /*  background image */
  D.Sticky = 0;    /*  1 for stickyness */
  D.Resize = 0;    /*  1 for Resize option */
  D.MinWidth = 100;    /*   for Resize option */
  D.MinHeight = 100;    /*   for Resize option */
#if 1 
  D.Callback = addnetdiaCallBack;    /*  default callback */
#else 
  D.Callback = NULL;    
#endif
  D.ResizeCallback = addnetdiaResizeCallBack;  /*  Resize callback */
#if 1 
  D.WaitCallback = NULL;  /*  Wait callback */
#else 
  D.WaitCallback = addnetdiaWaitCallBack;  /*  Wait callback */
#endif
  D.Fixpos = 1;    /*  1 for Fixing Position */
  D.NoTaskBar = 0;    /*  1 for not showing in task bar*/
  D.StackPos = 0;    /* -1,0,1 for for Stack Position -1:below 0:normal 1:above*/
  D.Shapexpm = NULL;    /*  PNG/jpeg file for window shape;Black color will not be drawn */
  D.parent = parent;    /*  1 for not showing in task bar*/
  D.pt = pt;    /*  any data to be passed by user*/
//  strcpy(D.name,"Kulina Designer ver 1.0");    /*  Dialog name you may change */
  if(D.fullscreen!=1) {    /*  if not fullscreen mode */
     int xres,yres; 
     kgDisplaySize(&xres,&yres); 
     D.xo = (xres-D.xl)/2;
     D.yo = (yres -D.yl)/2;
      // D.xo=D.yo=0; D.xl = xres-10; D.yl=yres-80;
  }
  else {    // for fullscreen
     int xres,yres; 
     kgDisplaySize(&xres,&yres); 
     D.xo=D.yo=0; D.xl = xres; D.yl=yres;
//     D.StackPos = 1; // you may need it
  }    /*  end of fullscreen mode */
//  kgColorTheme(&D,210,210,210);    /*  set colors for gui*/
//  ModifyaddnetdiaGc(&(D.gc));    /*  set colors for gui*/
  D.gc = ((DIALOG *)(D.parent))->gc;
  {
    int xs,ys;
    kgMakeKeybrd1(&D,0,0,1,1,25,-40045040,501,201,0.0,0.05);
    kgSetKeybrdWidget(&D,1);
    kgGetKeybrdSize(&D,&xs,&ys);
    kgShiftKeybrd(&D,(D.xl-xs)/2,(D.yl-ys-20));
  }
  ret= kgUi(&D);
  kgCleanUi(&D);
  return ret;
}
int Runaddnetdia(void *arg,char *ssid,char *psk) {
/*************************************************

    Text_Box1  2 data values

*************************************************/
   int i,ret=0;
   char  v0[200]=" " ;
   char  v1[200]=" " ;
   void* v[2];
   v[0]=(void *)(v0);
   v[1]=(void *)(v1);
   v0[0]='\0';
   v1[0]='\0';
   void *pt=NULL; /* pointer to send any extra information */
   addnetdia(arg,v,pt );
   i=0; while(v0[i]== ' ') i++;
   if(v0[i]> ' ') {
      ret=1;
      strcpy(ssid,v0+i);
      i=0;while(v1[i]== ' ') i++;
      if(v1[i] > ' ') {
        strcpy(psk,v1+i);
      }
      else psk[0]='\0';
   }
   return ret;
}
