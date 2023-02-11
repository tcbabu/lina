#include "kulina.h"
#include "wirediaCallbacks.h"
#include "headers.h"
#include "netlist.h"
#include "simages.c"
extern LINACONFIG lc;
int RetVal=0;
int grp1,grp2,wgrp;
int Connected=0;
extern Dlink *Scanlist;
extern int Wvis;
extern int Textgrp;
ThumbNail ** MakeScanThumbnails(Dlink *Scanlist) {
   int count,i;
   NETLIST *nt;
   ThumbNail **th=NULL;
   char **menu;
   if(Scanlist==NULL) {
     th = (ThumbNail **)malloc(sizeof(ThumbNail *));
     th[0]=NULL;
     return th;
   }
   count = Dcount(Scanlist)+1;
   Resetlink(Scanlist);
   i=0;
   menu = (char **)malloc(sizeof(char *)*count);
   menu[count-1]=NULL;
   while ((nt=(NETLIST *)Getrecord(Scanlist)) != NULL) {
     menu[i]=nt->ssid;
     i++;
   }
   th= kgStringToThumbNails(menu);
   Resetlink(Scanlist);
   i=0;
   while ((nt=(NETLIST *)Getrecord(Scanlist)) != NULL) {
     switch(nt->sig) {
       case 1:
        th[i]->img = kgGetImageCopy(NULL,(void *)&signal00_str);
        break;
       case 100:
        th[i]->img = kgGetImageCopy(NULL,(void *)&signal100_str);
        break;
       case 25:
        th[i]->img = kgGetImageCopy(NULL,(void *)&signal25_str);
        break;
       case 50:
        th[i]->img = kgGetImageCopy(NULL,(void *)&signal50_str);
        break;
       case 75:
        th[i]->img = kgGetImageCopy(NULL,(void *)&signal75_str);
        break;
       default:
        th[i]->img = kgGetImageCopy(NULL,(void *)&question_str);
        break;
     }
     i++;
   }
   free(menu);
   return th;
}
void ModifywirediaGc(Gclr *gc) {
/*
//  You may change default settings here 
//  probably you can allow the user to create a config in $HOME
//  and try to read that file (if exits); so dynamic configuration is possible
   gc->FontSize =8;
   gc->Font=23;
*/
}
int wirediaGroup( DIALOG *D,void **v,void *pt) {
  int GrpId=0,oitems=0,i,j;
  DIA *d=NULL,*dtmp;
  char *xpm0=   (char *)MakePixmapImage( 350,350,lc.Red,lc.Green,lc.Blue,0.05);
  DIP p0 = { 
    'p',
    5,5,  
    357,355,  
    (void *)xpm0,
    -1, /* bkgr colour */ 
      3,0,0.1 /* border hide */ 
  };
  strcpy(p0.Wid,(char *)"wdiabkgr");
  DII i1 = { 
    'i',
    20,230,  
    342,260,  
    35,11,0   
  };
  strcpy(i1.Wid,(char *)"wdiainfo");
  BUT_STR  *butn2=NULL; 
  butn2= (BUT_STR *)malloc(sizeof(BUT_STR)*3);
  butn2[0].sw=1;
  strcpy(butn2[0].title,(char *)"Scan");
  butn2[0].xpmn=NULL;
  butn2[0].xpmp=NULL;
  butn2[0].xpmh=NULL;
  butn2[0].bkgr=-1;
  butn2[0].butncode='';
  butn2[1].sw=1;
  strcpy(butn2[1].title,(char *)"Add");
  butn2[1].xpmn=NULL;
  butn2[1].xpmp=NULL;
  butn2[1].xpmh=NULL;
  butn2[1].bkgr=-1;
  butn2[1].butncode='';
  butn2[2].sw=1;
  strcpy(butn2[2].title,(char *)"Delete");
  butn2[2].xpmn=NULL;
  butn2[2].xpmp=NULL;
  butn2[2].xpmh=NULL;
  butn2[2].bkgr=-1;
  butn2[2].butncode='';
  DIN b2 = { 
    'n',
    77,276,  
    283,310,
    2,2,  
    64, 
    24, 
    3,1, 
    2,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn2, 
    wirediabutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b2.Wid,(char *)"wirediaWidget3");
  BUT_STR  *butn3=NULL; 
  butn3= (BUT_STR *)malloc(sizeof(BUT_STR)*2);
  butn3[0].sw=1;
  strcpy(butn3[0].title,(char *)"Okay");
  butn3[0].xpmn=NULL;
  butn3[0].xpmp=NULL;
  butn3[0].xpmh=NULL;
  butn3[0].bkgr=-1;
  butn3[0].butncode='';
  butn3[1].sw=1;
  strcpy(butn3[1].title,(char *)"Disconnect");
  butn3[1].xpmn=NULL;
  butn3[1].xpmp=NULL;
  butn3[1].xpmh=NULL;
  butn3[1].bkgr=-1;
  butn3[1].butncode='';
  DIL h3 = { 
    'h',
    102,312,  
    258,345,
    2,0,  
    72, 
    25, 
    2,1, 
    5,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn3, 
    wirediasplbutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(h3.Wid,(char *)"wirediaWidget4");
  BUT_STR  *butn4=NULL; 
  butn4= (BUT_STR *)malloc(sizeof(BUT_STR)*2);
  butn4[0].sw=1;
  strcpy(butn4[0].title,(char *)"Okay");
  butn4[0].xpmn=NULL;
  butn4[0].xpmp=NULL;
  butn4[0].xpmh=NULL;
  butn4[0].bkgr=-1;
  butn4[0].butncode='';
  butn4[1].sw=1;
  strcpy(butn4[1].title,(char *)"Connect");
  butn4[1].xpmn=NULL;
  butn4[1].xpmp=NULL;
  butn4[1].xpmh=NULL;
  butn4[1].bkgr=-1;
  butn4[1].butncode='';
  DIL h4 = { 
    'h',
    102,311,  
    258,344,
    2,0,  
    72, 
    25, 
    2,1, 
    5,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn4, 
    wirediasplbutton2callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(h4.Wid,(char *)"wirediaWidget5");
  DIX x5 = { 
    'x',
    20,20,  
    342,220,  
    10,2,  
    299, 
    40, 
    1,2, 
    0,4, 
    (int *)v[0], 
    NULL, 
    NULL, 
    NULL,wirediabrowser1callback, /* *args, callback */
    6,  /* Border Offset  */
     22,  /* Scroll width  */
     11,  /* Type  */
     1, /* item highlight */
    0, /* bordr */
    0, /* bkgr */
    0  /* =1 hide  */
   };
  strcpy(x5.Wid,(char *)"wdiabrowser");
  dtmp = D->d;
  i=0;
  if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
  dtmp = (DIA *)realloc(dtmp,sizeof(DIA )*(i+7));
  d =dtmp+i; 
  d[6].t=NULL;
  d[0].t = (DIT *)malloc(sizeof(DIP));
  *d[0].p = p0;
  d[0].p->item = -1;
  d[1].t = (DIT *)malloc(sizeof(DII));
  *d[1].i = i1;
  d[1].i->item = -1;
  d[2].t = (DIT *)malloc(sizeof(DIN));
  wirediabutton1init(&b2,pt) ;
  *d[2].N = b2;
  d[2].N->item = -1;
  d[3].t = (DIT *)malloc(sizeof(DIL));
  wirediasplbutton1init(&h3,pt) ;
  *d[3].h = h3;
  d[3].h->item = -1;
  d[4].t = (DIT *)malloc(sizeof(DIL));
  wirediasplbutton2init(&h4,pt) ;
  *d[4].h = h4;
  d[4].h->item = -1;
  d[5].t = (DIT *)malloc(sizeof(DIX));
  wirediabrowser1init(&x5,pt) ;
  *d[5].x = x5;
  d[5].x->item = -1;
  d[6].t = NULL;
  GrpId=kgOpenGrp(D);
  D->d = dtmp;
  j=0;
  while(d[j].t!=NULL){ kgAddtoGrp(D,GrpId,(void *)(d[j].t));j++;}
  return GrpId;
} 

/* One can also use the following code to add Widgets to an existing Dialog */

int MakewirediaGroup(DIALOG *D,void *arg) {
   int GrpId;
   DIA *d=NULL,*dtmp;
   WIDGETGRP *Gpt;
/*************************************************

    Selectmenu1  1 data value

*************************************************/
   int  *v0 ;
   int i,offset;
   
   d = D->d;
   i=0;
   while(d[i].t!= NULL) i++;
   offset =i;
   v0 = (int *)malloc(sizeof(int));
   *v0 = 1;
   void** v=(void **)malloc(sizeof(void*)*2);
   v[1]=NULL;
   v[0]=(void *)(v0);
   void *pt=NULL; /* pointer to send any extra information */
   GrpId = wirediaGroup(D,v,pt);
   d = D->d;
   wirediabrowser1init(d[offset+5].x,pt) ;
   Gpt = kgGetWidgetGrp(D,GrpId);
   Gpt->arg= v;
   grp1 =kgOpenGrp(D);
   kgAddtoGrp(D,grp1,d[offset+0].t);
   kgAddtoGrp(D,grp1,d[offset+1].t);
   kgAddtoGrp(D,grp1,d[offset+3].t);
   grp2 =kgOpenGrp(D);
   kgAddtoGrp(D,grp2,d[offset+0].t);
   kgAddtoGrp(D,grp2,d[offset+1].t);
   kgAddtoGrp(D,grp2,d[offset+2].t);
   kgAddtoGrp(D,grp2,d[offset+4].t);
   kgAddtoGrp(D,grp2,d[offset+5].t);
   D->wc = NULL;
#if 0
   if(Connected) {
     kgSetGrpVisibility(&D,grp2,0);
     kgSetGrpVisibility(&D,grp1,1);
   }
   else {
     kgSetGrpVisibility(&D,grp1,0);
     kgSetGrpVisibility(&D,grp2,1);
   }
#endif
   return GrpId;
}
int SetWirediaVisibility(DIALOG *D,int vis) {
   wirediainit(D);
   switch(vis) {
     case 0:
       kgSetGrpVisibility(D,wgrp,0);
       Wvis=0;
       kgSetGrpVisibility(D,Textgrp,1);
       kgSetKeybrdWidget(1);
       if(D->wc != NULL) {
           kgUpdateGrp(D,wgrp);
           kgUpdateGrp(D,Textgrp);
       }
       break;
     case 1:
     default:
       Wvis=1;
       kgSetGrpVisibility(D,Textgrp,0);
       kgSetKeybrdWidget(-1);
       if(Connected) {
         kgSetGrpVisibility(D,grp2,0);
         kgSetGrpVisibility(D,grp1,1);
         if(D->wc != NULL) {
           kgUpdateGrp(D,grp1);
         }
       }
       else {
         kgSetGrpVisibility(D,grp1,0);
         kgSetGrpVisibility(D,grp2,1);
         if(D->wc != NULL) {
           kgUpdateGrp(D,grp2);
         }
       }
       break;
   }
#if 0
   if(D->wc != NULL) {
      kgUpdateOn(D);
   }
#endif
   return vis;
}

int wiredia( void *parent,void **v,void *pt) {
  int ret=1,GrpId,k;
  DIALOG D;
  DIA *d=NULL;
  D.VerId=1401010200;
  kgInitUi(&D);
  D.d=NULL;
  GrpId = wirediaGroup(&D,v,pt);
  d = D.d;
  D.d = d;
  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 4;
  D.df = 5;
  D.tw = 4;
  D.bw = 4;
  D.lw = 4;
  D.rw = 4;
  D.xo = 522;   /* Position of Dialog */ 
  D.yo = 132;
  D.xl = 360;    /*  Length of Dialog */
  D.yl = 362;    /*  Width  of Dialog */
  D.Initfun = wirediainit;    /*   init fuction for Dialog */
  D.Cleanupfun = wirediacleanup;    /*   init fuction for Dialog */
  D.kbattn = 0;    /*  1 for drawing keyborad attention */
  D.butattn = 0;    /*  1 for drawing button attention */
  D.fullscreen = 0;    /*  1 for for fullscreen mode */
  D.Deco = 1;    /*  1 for Window Decorration */
  D.transparency = 1.000000;    /*  float 1.0 for full transparency */
  D.Newwin = 0;    /*  1 for new window not yet implemented */
  D.DrawBkgr = 0;    /*  1 for drawing background */
  D.Bkpixmap = NULL;    /*  background image */
  D.Sticky = 0;    /*  1 for stickyness */
  D.Resize = 0;    /*  1 for Resize option */
  D.MinWidth = 100;    /*   for Resize option */
  D.MinHeight = 100;    /*   for Resize option */
#if 1 
  D.Callback = wirediaCallBack;    /*  default callback */
#else 
  D.Callback = NULL;    
#endif
  D.ResizeCallback = wirediaResizeCallBack;  /*  Resize callback */
#if 1 
  D.WaitCallback = NULL;  /*  Wait callback */
#else 
  D.WaitCallback = wirediaWaitCallBack;  /*  Wait callback */
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
      // D.xo=D.yo=0; D.xl = xres-10; D.yl=yres-80;
     D.xo = (xres-D.xl)/2;
     D.yo = (yres -D.yl)/2-70;
  }
  else {    // for fullscreen
     int xres,yres; 
     kgDisplaySize(&xres,&yres); 
     D.xo=D.yo=0; D.xl = xres; D.yl=yres;
//     D.StackPos = 1; // you may need it
  }    /*  end of fullscreen mode */
  grp1 =kgOpenGrp(&D);
//  kgAddtoGrp(&D,grp1,d[1].t);
  kgAddtoGrp(&D,grp1,d[3].t);
  grp2 =kgOpenGrp(&D);
  kgAddtoGrp(&D,grp2,d[2].t);
  kgAddtoGrp(&D,grp2,d[4].t);
  kgAddtoGrp(&D,grp2,d[5].t);
  if(Connected) {
    kgSetGrpVisibility(&D,grp2,0);
    kgSetGrpVisibility(&D,grp1,1);
  }
  else {
    kgSetGrpVisibility(&D,grp1,0);
    kgSetGrpVisibility(&D,grp2,1);
  }
//  kgColorTheme(&D,210,210,210);    /*  set colors for gui*/
//  ModifywirediaGc(&(D.gc));    /*  set colors for gui*/
  D.gc = ((DIALOG *)(D.parent))->gc;
  ret= kgUi(&D);
  kgCleanUi(&D);
  return ret;
}
int Runwiredia(void *arg) {
/*************************************************

    Selectmenu1  1 data value

*************************************************/
   int ret;
   int   v0 = 1;
   void* v[1];
   v[0]=(void *)(&v0);
   void *pt=NULL; /* pointer to send any extra information */
   ret = wiredia(arg,v,pt );
   if(ret==1) ret=0;
   else ret=1;
   return RetVal;
}
