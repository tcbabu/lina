#include "headers.h"
#include <kulina.h>
#include "configlinaCallbacks.h"
int Enabled=0;

LINACONFIG lc;
int CheckString(char *s1,char *s2) {
   int ch,ln,ret=0;
   ln = strlen(s2);
   if(strlen(s1)<ln) ret=0;
   else {
     ch = s1[ln];
     s1[ln]='\0';
     if( strcmp(s1,s2)==0 ) ret=1;
     s1[ln]=ch;
   }
   return ret;
}
int SearchString(char *s1,char *s2) {
   int ch,ln,ret=-1,ln1,i=0;
   ln = strlen(s2);
   ln1 = strlen(s1);
   if(ln1<ln) ret=-1;
   else {
    i=0;
    for(i=0;i<=(ln1-ln);i++) {
      if(CheckString(s1+i,s2)){
        ret=i;
        break;
      }
    }
   }
   return ret;
}
void ModifyconfiglinaGc(Gclr *gc) {
/*
//  You may change default settings here 
//  probably you can allow the user to create a config in $HOME
//  and try to read that file (if exits); so dynamic configuration is possible
   gc->FontSize =8;
   gc->Font=23;
*/
}
void * kgColorImage(int size,int red,int green,int blue) {
   void *fid,*img=NULL;
   float R;
   R = size;
   fid = kgInitImage(size,size,4);
   kgUserFrame(fid,0.0,0.0,+R,+R);
   kgChangeColor(fid,1001,red,green,blue);
   kgRoundedRectangleFill(fid,R*0.5,R*0.5,R-2.,R-2.,0,1001,0.15);
   img = kgGetResizedImage(fid);
   kgCloseImage(fid);
   return img;
}
int configlinaGroup( DIALOG *D,void **v,void *pt) {
  int GrpId=0,oitems=0,i,j;
  int count;
  SESSIONINFO *spt;
  USERINFO *cpt;
  DIA *d=NULL,*dtmp;
  T_ELMT *e0  ; 
  e0 =(T_ELMT *)malloc(sizeof(T_ELMT)*1);
  e0[0].fmt = (char *)malloc(20);
  strcpy(e0[0].fmt,(char *)"BackgroundImage%30s");
  e0[0].v=(void *)v[0];
  e0[0].sw=1;
  DIT t0 = { 
    't',
    12,13,  
    470,47,
    20, 
    1,1, 
    e0,
    1,1,
    NULL,configlinatextbox1callback,1,0,18,9 /* args,Call back */
  };
  strcpy(t0.Wid,(char *)"configlinaWidget2");
  t0.pt=NULL;
  BUT_STR  *butn1=NULL; 
  butn1= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn1[0].sw=1;
  strcpy(butn1[0].title,(char *)"Browse");
  butn1[0].xpmn=NULL;
  butn1[0].xpmp=NULL;
  butn1[0].xpmh=NULL;
  butn1[0].imgn=NULL;
  butn1[0].imgp=NULL;
  butn1[0].imgh=NULL;
  butn1[0].Bimg=NULL;
  butn1[0].bkgr=-1;
  butn1[0].butncode='b';
  DIN b1 = { 
    'n',
    471,13,  
    545,47,
    2,2,  
    64, 
    24, 
    1,1, 
    2,0.150000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn1, 
    configlinabutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b1.Wid,(char *)"configlinaWidget3");
  char *menu2[]  = { 
    (char *)"Allow Root Login",
    (char *)"Add Text Session",
    (char *)"Add Failsafe Session",
    (char *)"Show Date/Time",
    NULL 
  };
  ThumbNail **th0 ;
  DICH c2 = { 
    'c',
    6,54,  
    550,87,   
    8,0,  
    125, 
    25, 
    1,4, 
    0,1, 
    (int *)v[1], 
    NULL, 
    NULL, 
    NULL,configlinabrowser1callback, /* *args, callback */
    1,  /* Border Offset  */
     2,  /* Scroll width  */
     0,  /* Type  */
     0, /* item highlight */
    0, /* bordr */
    0, /* bkgr */
    0  /* =1 hide  */
   };
  th0 = (ThumbNail **)kgStringToThumbNails((char **)menu2);
  c2.list=(void **)th0;
  strcpy(c2.Wid,(char *)"configlinaWidget7");
  char **menu3 ; 
  count = Dcount(lc.Ulist)+1;
  if(lc.Ulist==NULL) count=1;
  else count = Dcount(lc.Ulist)+1;
  printf ("Ulist count= %d\n",count);
  menu3= (char **)malloc(sizeof(char *)*count);
  menu3[count-1]=NULL;
  Resetlink(lc.Ulist);
  j=0;
  while ((cpt= Getrecord(lc.Ulist))!= NULL) {
   menu3[j] = (char *)malloc(strlen(cpt->LoginId)+1);
   strcpy(menu3[j],cpt->LoginId);
   j++;
  }
  char *prompt3 ; 
  prompt3=(char *)malloc(13);
  strcpy(prompt3,(char *)"Default User");
  DIW w3 = { 
    'w',
    43,170,  
    245,199,   
    4,  
    (int *)v[2],
    prompt3 ,
    menu3 ,
    NULL,configlinabrowser2callback, /* *args, callback */
    0 
  };
  strcpy(w3.Wid,(char *)"configlinaWidget10");
  if(w3.size >= count ) w3.size = count-1;
  printf ("Slist count= %d\n",count);
  char **menu4 ; 
  if(lc.Slist==NULL) count=1;
  else count = Dcount(lc.Slist)+1;
  menu4= (char **)malloc(sizeof(char *)*count);
  menu4[count-1]=NULL;
  Resetlink(lc.Slist);
  j=0;
  while ((spt= Getrecord(lc.Slist))!= NULL) {
   menu4[j] = (char *)malloc(strlen(spt->Name)+1);
   strcpy(menu4[j],spt->Name);
   j++;
  }

  char *prompt4 ; 
  prompt4=(char *)malloc(16);
  strcpy(prompt4,(char *)"Default Session");
  DIW w4 = { 
    'w',
    25,208,  
    254,237,   
    4,  
    (int *)v[3],
    prompt4 ,
    menu4 ,
    NULL,configlinabrowser3callback, /* *args, callback */
    0 
  };
  strcpy(w4.Wid,(char *)"configlinaWidget11");
  if(w4.size >= count ) w4.size = count-1;
  DIF f5 = { 
    'f',
    404,206,  
    504,252,   
    0.000000,1.000000,  
    50,  
    (double *)v[4],
    NULL,
    NULL,configlinafslide1callback /* *args, callback */
  };
  strcpy(f5.Wid,(char *)"configlinaWidget12");
  DIF f6 = { 
    'f',
    399,259,  
    499,305,   
    0.000000,0.500000,  
    50,  
    (double *)v[5],
    NULL,
    NULL,configlinafslide2callback /* *args, callback */
  };
  strcpy(f6.Wid,(char *)"configlinaWidget13");
  BUT_STR  *butn7=NULL; 
  butn7= (BUT_STR *)malloc(sizeof(BUT_STR)*2);
  butn7[0].sw=1;
  strcpy(butn7[0].title,(char *)"Cancel");
  butn7[0].xpmn=NULL;
  butn7[0].xpmp=NULL;
  butn7[0].xpmh=NULL;
  butn7[0].bkgr=-1;
  butn7[0].butncode='';
  butn7[1].sw=1;
  strcpy(butn7[1].title,(char *)"Save");
  butn7[1].xpmn=NULL;
  butn7[1].xpmp=NULL;
  butn7[1].xpmh=NULL;
  butn7[1].bkgr=-1;
  butn7[1].butncode='';
  DIL h7 = { 
    'h',
    379,329,
    535,362,
    2,0,  
    72, 
    25, 
    2,1, 
    4,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn7, 
    configlinasplbutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(h7.Wid,(char *)"configlinaWidget14");
  DIM m8 = { 
    'm',
    300,226,  
    400,250,  
    1,0  
  };
  strncpy(m8.msg,(char *)"Transparency",499);
  strcpy(m8.Wid,(char *)"configlinaWidget15");
  DIM m9 = { 
    'm',
    304,278,  
    404,302,  
    1,0  
  };
  strncpy(m9.msg,(char *)"Rounding Fac",499);
  strcpy(m9.Wid,(char *)"configlinaWidget16");
  BUT_STR  *butn10=NULL; 
  butn10= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn10[0].sw=1;
  strcpy(butn10[0].title,(char *)"Add/Delete Sessions");
  butn10[0].xpmn=NULL;
  butn10[0].xpmp=NULL;
  butn10[0].xpmh=NULL;
  butn10[0].imgn=NULL;
  butn10[0].imgp=NULL;
  butn10[0].imgh=NULL;
  butn10[0].Bimg=NULL;
  butn10[0].bkgr=-1;
  butn10[0].butncode='';
  DIN b10 = { 
    'n',
    62,245,  
    227,281,
    0,0,  
    154, 
    26, 
    1,1, 
    4,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn10, 
    configlinabutton2callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b10.Wid,(char *)"configlinaWidget18");
  BUT_STR  *butn11=NULL; 
  butn11= (BUT_STR *)malloc(sizeof(BUT_STR)*5);
  butn11[0].sw=1;
  strcpy(butn11[0].title,(char *)"Base Color");
  butn11[0].xpmn=kgColorImage(48,lc.Red,lc.Green,lc.Blue);
  butn11[0].xpmp=NULL;
  butn11[0].xpmh=NULL;
  butn11[0].imgn=NULL;
  butn11[0].imgp=NULL;
  butn11[0].imgh=NULL;
  butn11[0].Bimg=NULL;
  butn11[0].bkgr=-1;
  butn11[0].butncode='';
  butn11[1].sw=1;
  strcpy(butn11[1].title,(char *)"Highlight Color");
  butn11[1].xpmn=kgColorImage(48,lc.HighRed,lc.HighGreen,lc.HighBlue);
  butn11[1].xpmp=NULL;
  butn11[1].xpmh=NULL;
  butn11[1].imgn=NULL;
  butn11[1].imgp=NULL;
  butn11[1].imgh=NULL;
  butn11[1].Bimg=NULL;
  butn11[1].bkgr=-1;
  butn11[1].butncode='';
  butn11[2].sw=1;
  strcpy(butn11[2].title,(char *)"Font Color");
  butn11[2].xpmn=kgColorImage(48,lc.FontRed,lc.FontGreen,lc.FontBlue);
  butn11[2].xpmp=NULL;
  butn11[2].xpmh=NULL;
  butn11[2].imgn=NULL;
  butn11[2].imgp=NULL;
  butn11[2].imgh=NULL;
  butn11[2].Bimg=NULL;
  butn11[2].bkgr=-1;
  butn11[2].butncode='';
  butn11[3].sw=1;
  strcpy(butn11[3].title,(char *)"Button Color");
  butn11[3].xpmn=kgColorImage(48,lc.ButRed,lc.ButGreen,lc.ButBlue);
  butn11[3].xpmp=NULL;
  butn11[3].xpmh=NULL;
  butn11[3].imgn=NULL;
  butn11[3].imgp=NULL;
  butn11[3].imgh=NULL;
  butn11[3].Bimg=NULL;
  butn11[3].bkgr=-1;
  butn11[3].butncode='';
  butn11[4].sw=1;
  strcpy(butn11[4].title,(char *)"Date Color");
  butn11[4].xpmn=kgColorImage(48,lc.DateRed,lc.DateGreen,lc.DateBlue);
  butn11[4].xpmp=NULL;
  butn11[4].xpmh=NULL;
  butn11[4].imgn=NULL;
  butn11[4].imgp=NULL;
  butn11[4].imgh=NULL;
  butn11[4].Bimg=NULL;
  butn11[4].bkgr=-1;
  butn11[4].butncode='';
  DIN b11 = { 
    'n',
    36,90,  
    525,162,
//    42,20,  
    42,20,  
//    48, 
    48, 
    48, 
    5,1, 
    0,0.150000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn11, 
    configlinabutton3callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b11.Wid,(char *)"configlinaWidget18");
  char *Fonts[]  = {
    (char *)"!f1800 !f00 Times-Roman",
    (char *)"!f1801 !f01 Times-Italic",
    (char *)"!f1802 !f02 Times-Bold",
    (char *)"!f1803 !f03 Times-BoldItalic",
    (char *)"!f1804 !f04 Bookman-Light",
    (char *)"!f1805 !f05 Bookman-LightItalic",
    (char *)"!f1806 !f06 Bookman-Demi",
    (char *)"!f1807 !f07 Bookman-DemiItalic",
    (char *)"!f1808 !f08 Helvetica",
    (char *)"!f1809 !f09 Helvetica-Oblique",
    (char *)"!f1810 !f10 Helvetica-Bold",
    (char *)"!f1811 !f11 Helvetica-BoldOblique",
    (char *)"!f1812 !f12 Helvetica-Narrow",
    (char *)"!f1813 !f13 Helvetica-Narrow-Oblique",
    (char *)"!f1814 !f14 Helvetica-Narrow-Bold",
    (char *)"!f1815 !f15 Helvetica-Narrow-BoldOblique",
    (char *)"!f1816 !f16 Courier",
    (char *)"!f1817 !f17 Courier-Oblique",
    (char *)"!f1818 !f18 Courier-Bold",
    (char *)"!f1819 !f19 Courier-BoldOblique",
    (char *)"!f1820 !f20 Charter-Roman",
    (char *)"!f1821 !f21 Charter-Italic",
    (char *)"!f1822 !f22 Charter-Bold",
    (char *)"!f1823 !f23 Charter-BoldItalic",
    (char *)"!f1824 !f24 Palatino-Roman",
    (char *)"!f1825 !f25 Palatino-Italic",
    (char *)"!f1826 !f26 Palatino-Bold",
    (char *)"!f1827 !f27 Palatino-BoldItalic",
    (char *)"!f1828 !f28 NewCenturySchlbk-Roman",
    (char *)"!f1829 !f29 NewCenturySchlbk-Italic",
    (char *)"!f1830 !f30 NewCenturySchlbk-Bold",
    (char *)"!f1831 !f31 NewCenturySchlbk-BoldItalic",
    (char *)"!f1832 !f32 AvantGarde-BookOblique",
    (char *)"!f1833 !f33 AvantGarde-Demi",
    (char *)"!f1834 !f34 ZapfChancery-MediumItalic",
    NULL
  };

  char **menu12;
  menu12 = (char **)malloc(sizeof(char *)*36);
  for(i=0;i<35;i++) {
    menu12[i]=(char *)malloc(strlen(Fonts[i])+1);
    strcpy(menu12[i],Fonts[i]);
  }
  menu12[35]=NULL;

  char *prompt12 ; 
  prompt12=(char *)malloc(10);
  strcpy(prompt12,(char *)"Date Font");
  DIW w12 = { 
    'w',
    283,170,  
    537,199,   
    9,  
    (int *)v[6],
    prompt12 ,
    menu12 ,
    NULL,configlinabrowser4callback, /* *args, callback */
    0 
  };
  strcpy(w12.Wid,(char *)"configlinaWidget13");
  
  BUT_STR  *butn13=NULL; 
  butn13= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn13[0].sw=1;
  strcpy(butn13[0].title,(char *)"Enable Lina");
  butn13[0].xpmn=NULL;
  butn13[0].xpmp=NULL;
  butn13[0].xpmh=NULL;
  butn13[0].imgn=NULL;
  butn13[0].imgp=NULL;
  butn13[0].imgh=NULL;
  butn13[0].Bimg=NULL;
  butn13[0].bkgr=-221221221;
  butn13[0].butncode='';
  DIN b13 = { 
    'n',
    24,309,
    170,363,
    2,2,  
    140, 
    44, 
    1,1, 
    2,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn13, 
    configlinabutton4callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b13.Wid,(char *)"configlinaWidget15");
  dtmp = D->d;
  i=0;
  if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
  dtmp = (DIA *)realloc(dtmp,sizeof(DIA )*(i+15));
  d =dtmp+i; 
  d[14].t=NULL;
  d[0].t = (DIT *)malloc(sizeof(DIT));
  *d[0].t = t0;
  d[0].t->item = -1;
  d[1].t = (DIT *)malloc(sizeof(DIN));
  configlinabutton1init(&b1,pt) ;
  *d[1].N = b1;
  d[1].N->item = -1;
  d[2].t = (DIT *)malloc(sizeof(DICH));
  configlinabrowser1init(&c2,pt) ;
  *d[2].c = c2;
  d[2].c->item = -1;
  d[3].t = (DIT *)malloc(sizeof(DIW));
  *d[3].w = w3;
  d[3].w->item = -1;
  d[4].t = (DIT *)malloc(sizeof(DIW));
  *d[4].w = w4;
  d[4].w->item = -1;
  d[5].t = (DIT *)malloc(sizeof(DIF));
  *d[5].f = f5;
  d[5].f->item = -1;
  d[6].t = (DIT *)malloc(sizeof(DIF));
  *d[6].f = f6;
  d[6].f->item = -1;
  d[7].t = (DIT *)malloc(sizeof(DIL));
  configlinasplbutton1init(&h7,pt) ;
  *d[7].h = h7;
  d[7].h->item = -1;
  d[8].t = (DIT *)malloc(sizeof(DIM));
  *d[8].m = m8;
  d[8].m->item = -1;
  d[9].t = (DIT *)malloc(sizeof(DIM));
  *d[9].m = m9;
  d[9].m->item = -1;
  d[10].t = (DIT *)malloc(sizeof(DIN));
  configlinabutton2init(&b10,pt) ;
  *d[10].N = b10;
  d[10].N->item = -1;
  d[11].t = (DIT *)malloc(sizeof(DIN));
  configlinabutton3init(&b11,pt) ;
  *d[11].N = b11;
  d[11].N->item = -1;
  d[12].t = (DIT *)malloc(sizeof(DIW));
  *d[12].w = w12;
  d[12].w->item = -1;
  d[13].t = (DIT *)malloc(sizeof(DIN));
  configlinabutton4init(&b13,pt) ;
  *d[13].N = b13;
  d[13].N->item = -1; 
  d[14].t = NULL;
  GrpId=kgOpenGrp(D);
  D->d = dtmp;
  j=0;
  while(d[j].t!=NULL){ kgAddtoGrp(D,GrpId,(void *)(d[j].t));j++;}
  return GrpId;
} 

/* One can also use the following code to add Widgets to an existing Dialog */

int MakeconfiglinaGroup(DIALOG *D,void *arg) {
   int GrpId;
   WIDGETGRP *Gpt;
/*************************************************

    Text_Box1  1 data values
    CheckBox1  1 data value
    Browser2  1 data value
    Browser3  1 data value
    Floatslidebar1  1 data value
    Floatslidebar2  1 data value
    Browser4  1 data value

*************************************************/
   char  *v0 ;
   v0 = (char *)malloc(sizeof(100));
   v0[0] = '\0';
   int  *v1 ;
   v1 = (int *)malloc(sizeof(int));
   *v1 = 1;
   int  *v2 ;
   v2 = (int *)malloc(sizeof(int));
   *v2 = 1;
   int  *v3 ;
   v3 = (int *)malloc(sizeof(int));
   *v3 = 1;
   double *v4 ;
   v4 = (double *)malloc(sizeof(double));
   *v4 = 0.0;
   double *v5 ;
   v5 = (double *)malloc(sizeof(double));
   *v5 = 0.0;
   int  *v6 ;
   v6 = (int *)malloc(sizeof(int));
   *v6 = 1;
   void** v=(void **)malloc(sizeof(void*)*8);
   v[7]=NULL;
   v[0]=(void *)(v0);
   v[1]=(void *)(v1);
   v[2]=(void *)(v2);
   v[3]=(void *)(v3);
   v[4]=(void *)(v4);
   v[5]=(void *)(v5);
   v[6]=(void *)(v6);
   void *pt=NULL; /* pointer to send any extra information */
   GrpId = configlinaGroup(D,v,pt);
   Gpt = kgGetWidgetGrp(D,GrpId);
   Gpt->arg= v;
   return GrpId;
}

int configlina( void *parent,void **v,void *pt) {
  int ret=1,GrpId,k;
  DIALOG D;
  DIA *d=NULL;
  D.VerId=1401010200;
  kgInitUi(&D);
  D.d=NULL;
  GrpId = configlinaGroup(&D,v,pt);
  d = D.d;
  D.d = d;
  D.bkup = 0; /* set to 1 for backup */
  D.bor_type = 4;
  D.df = 12;
  D.tw = 4;
  D.bw = 4;
  D.lw = 4;
  D.rw = 4;
  D.xo = 0;   /* Position of Dialog */ 
  D.yo = 0;
  D.xl = 559;    /*  Length of Dialog */
  D.yl = 373;    /*  Width  of Dialog */
  D.Initfun = configlinainit;    /*   init fuction for Dialog */
  D.Cleanupfun = configlinacleanup;    /*   init fuction for Dialog */
  D.kbattn = 0;    /*  1 for drawing keyborad attention */
  D.butattn = 0;    /*  1 for drawing button attention */
  D.fullscreen = 0;    /*  1 for for fullscreen mode */
  D.Deco = 1;    /*  1 for Window Decorration */
  D.transparency = 0.000000;    /*  float 1.0 for full transparency */
  D.Newwin = 1;    /*  1 for new window not yet implemented */
  D.DrawBkgr = 1;    /*  1 for drawing background */
  D.Bkpixmap = NULL;    /*  background image */
  D.Sticky = 0;    /*  1 for stickyness */
  D.Resize = 0;    /*  1 for Resize option */
  D.MinWidth = 100;    /*   for Resize option */
  D.MinHeight = 100;    /*   for Resize option */
#if 1 
  D.Callback = configlinaCallBack;    /*  default callback */
#else 
  D.Callback = NULL;    
#endif
  D.ResizeCallback = configlinaResizeCallBack;  /*  Resize callback */
#if 1 
  D.WaitCallback = NULL;  /*  Wait callback */
#else 
  D.WaitCallback = configlinaWaitCallBack;  /*  Wait callback */
#endif
  D.Fixpos = 1;    /*  1 for Fixing Position */
  D.NoTaskBar = 0;    /*  1 for not showing in task bar*/
  D.StackPos = 0;    /* -1,0,1 for for Stack Position -1:below 0:normal 1:above*/
  D.Shapexpm = NULL;    /*  PNG/jpeg file for window shape;Black color will not be drawn */
  D.parent = parent;    /*  1 for not showing in task bar*/
  D.pt = pt;    /*  any data to be passed by user*/
  strcpy(D.name,"Configure Lina");    /*  Dialog name you may change */
  if(D.fullscreen!=1) {    /*  if not fullscreen mode */
     int xres,yres; 
     kgDisplaySize(&xres,&yres); 
      // D.xo=D.yo=0; D.xl = xres-10; D.yl=yres-80;
  }
  else {    // for fullscreen
     int xres,yres; 
     kgDisplaySize(&xres,&yres); 
     D.xo=D.yo=0; D.xl = xres; D.yl=yres;
//     D.StackPos = 1; // you may need it
  }    /*  end of fullscreen mode */
//  kgColorTheme(&D,210,210,210);    /*  set colors for gui*/
//  ModifyconfiglinaGc(&(D.gc));    /*  set colors for gui*/
//  Print_gui_data(&D,"configlina.rc");
  ret= kgUi(&D);
  kgCleanUi(&D);
  return ret;
}
void *Runconfiglina(void *arg) {
/*************************************************

    Text_Box1  1 data values
    CheckBox1  1 data value
    Browser2  1 data value
    Browser3  1 data value
    Floatslidebar1  1 data value
    Floatslidebar2  1 data value
    Browser4  1 data value

*************************************************/
   char  v0[300]=" " ;
   int   v1 = 1;
   int   v2 = 1;
   int   v3 = 1;
   double v4 = 0.0;
   double v5 = 0.0;
   int   v6 = 1;
   void* v[7];
   void *pt=NULL; /* pointer to send any extra information */
   ReadConfig(&lc);
   strcpy(v0,lc.Bkgr);
   v4 = lc.Transparency;
   v5 = lc.fac;
   v2 = GetIndexFromLogin(&lc,lc.DefUser);
   v3 = GetSessionFromName(&lc,lc.DefSession);
   v[0]=(void *)(v0);
   v[1]=(void *)(&v1);
   v[2]=(void *)(&v2);
   v[3]=(void *)(&v3);
   v[4]=(void *)(&v4);
   v[5]=(void *)(&v5);
   v[6]=(void *)(&v6);
   v6 = lc.DateFont+1;

   configlina(NULL,v,pt );
   if(geteuid()!=  0) return NULL;
   return NULL;
}
