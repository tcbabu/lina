#include "kulina.h"
#include <time.h>
#include  <sys/time.h>
#include <fcntl.h>
#include "headers.h"
void * DateTimeImg(LINACONFIG *lc,int xl,int yl) {
  int DateFont;
  int Dred,Dgreen,Dblue;
  unsigned long Waitsecs=0;
  int ret = 0,i,j,OK,count=0,k;
  DIG *fid;
  float ln,length=0,fac=1.0;
  float w_x1=0.0,w_y1=0.0,w_x2=686,w_y2=226,dyl,dxl,yy;
  float r,g,b, h, s, v,vorg;
  char *pt,buf[5000],flname[200],*str[500];;
  char *msg;
  Dred = lc->DateRed;
  Dgreen = lc->DateGreen;
  Dblue = lc->DateBlue;
  DateFont=lc->DateFont;
#if 0
  char *Month[]={(char *)"JAN",(char *)"FEB",(char *)"MAR",(char *)"APR",(char *)"MAY",(char *)"JUN",(char *)"JUL",(char *)"AUG",(char *)"SEP",(char *)"OCT",(char *)"NOV",(char *)"DEC"};
  char *Day[]=  {(char *)"SUN",(char *)"MON",(char *)"TUE",(char *)"WED",(char *)"THU",(char *)"FRI",(char *)"SAT"};
#else
#if 1
#if 1
  char *Month[]={(char *)"Jan",(char *)"Feb",(char *)"Mar",(char *)"Apr",(char *)"May",(char *)"Jun",(char *)"Jul",(char *)"Aug",(char *)"Sep",(char *)"Oct",(char *)"Nov",(char *)"Dec"};
#else
  char *Month[]={(char *)"January",(char *)"February",(char *)"March  ! ",(char *)"April  ! ",(char *)"May    ! ",(char *)"June   ! ",(char *)"July   ! ",(char *)"August ! ",(char *)"September",(char *)"October",(char *)"November",(char *)"December"};
#endif
#else
  char *Month[]={(char *)"January",(char *)"February",(char *)"March  ! ",(char *)"April  ! ",(char *)"May    ! ",(char *)"June   ! ",(char *)"July   ! ",(char *)"August ! ",(char *)"September",(char *)"October",(char *)"November",(char *)"December"};
#endif
  char *Day[]=  {(char *)"Sun",(char *)"Mon",(char *)"Tue",(char *)"Wed",(char *)"Thu",(char *)"Fri",(char *)"Sat"};
#endif
  char *AmPm[]={(char *)"AM",(char *)"PM"};
  time_t tp,t;
  int Hour,Minute;
  struct tm *dt;
  void *img,*img1,*img2;
  int Indx=0;
  w_x2 = xl;
  w_y2 = yl;
  {
    t=time(&tp);
    dt = localtime(&t);
    Hour = dt->tm_hour;
    Minute = dt->tm_min;
    Indx=0;
    if(Hour >=12) {Indx=1;}
    if(Hour > 12) {Hour -= 12;}
//    sprintf(buf,"!c15!f12!w81%-s\\n!c15!f12!w81!z43%2.2d %-s",Day[dt->tm_wday],dt->tm_mday,Month[dt->tm_mon]);
#if 0
    sprintf(buf,"%-s\\n!z43%2.2d %-s",Day[dt->tm_wday],dt->tm_mday,Month[dt->tm_mon]);
#else
    sprintf(buf,"!y!h32%-2.2d:%-2.2d!h23!h25!w35%-s!w53!h52!k!x!x!x!h45 %-s!r!z23  %-2.2d %-s %4.4d",
         Hour,Minute,AmPm[Indx],Day[dt->tm_wday],dt->tm_mday,Month[dt->tm_mon],dt->tm_year+1900);
#endif
    Waitsecs=24*3600-(dt->tm_hour*3600+dt->tm_min*60+dt->tm_sec);
//    printf("Waitsecs :%ld\n",Waitsecs);
    msg = buf;
    fid = (DIG *)kgInitImage((int)xl,(int)yl,4);
    kgUserFrame(fid,w_x1,w_y1,w_x2,w_y2);
    RGBtoHSV(Dred,Dgreen,Dblue,&h,&s,&vorg);
    v =0.6*vorg;
    HSVtoRGB(&r,&g,&b,h,s,v);
    kgChangeColor(fid,251,30,30,30);
    kgChangeColor(fid,253,(int)r,(int)g,(int)b);
    if(vorg < 0.9 ) {
       v = vorg*1.2;
       if(v>1.) v=1.;
       HSVtoRGB(&r,&g,&b,h,s,v);
       kgChangeColor(fid,252,(int)r,(int)g,(int)b);
    }
    else kgChangeColor(fid,252,250,250,250);
    kgChangeColor(fid,254,Dred,Dgreen,Dblue);
    kgTextFont(fid,DateFont);
    kgTextSize(fid,30.,25.,0.);
    pt=buf;
    j=1;
    k=0;
    while(1) {
      i=0;
      if(pt[i]=='\0') break;
//      gphMove2f(fid,40.,yl-j*45.);
      j++;
      OK=1;
      while(pt[i]!='\0') {
        OK=0;
        if((pt[i]=='\n')){ pt[i]='\0';break;}
        if(((pt[i]=='\\')&&(pt[i+1]=='n'))){ pt[i]='\0';i++;break;}
        i++;
        OK=1;
      }
      str[k++]=pt;
      ln = kgStringLength(fid,pt);
      if(length< ln)length=ln;
      if(OK) break;
      pt = pt+i+1;
    }
    if(k>0 ) {
      dyl = yl/k;
      fac = 0.9*xl/(length);
      dxl = fac*25;
      kgTextSize(fid,dyl*0.5,dxl,0.);
      yy= yl - dyl*0.7;
      kgTextColor(fid,251);
#if 1
      for(i=0;i<k;i++) {
         length=kgStringLength(fid,str[i]);
         kgMove2f(fid,(xl-length)*0.5+2,yy-2);
         kgWriteText(fid,str[i]);
         yy -=dyl;
      }
#endif
#if 1
      yy= yl - dyl*0.7;
      kgTextColor(fid,252);
      for(i=0;i<k;i++) {
         length=kgStringLength(fid,str[i]);
         kgMove2f(fid,(xl-length)*0.5-1,yy);
         kgWriteText(fid,str[i]);
         yy -=dyl;
      }
#endif
#if 1
      yy= yl - dyl*0.7;
      kgTextColor(fid,253);
      for(i=0;i<k;i++) {
         length=kgStringLength(fid,str[i]);
         kgMove2f(fid,(xl-length)*0.5+1,yy-1);
         kgWriteText(fid,str[i]);
         yy -=dyl;
      }
#endif
#if 1
      yy= yl - dyl*0.7;
      kgTextColor(fid,254);
      for(i=0;i<k;i++) {
         length=kgStringLength(fid,str[i]);
         kgMove2f(fid,(xl-length)*0.5,yy);
         kgWriteText(fid,str[i]);
         yy -=dyl;
      }
#endif
    }
    img = kgGetResizedImage(fid);
    kgCloseImage(fid);

  }
  return img;
}
