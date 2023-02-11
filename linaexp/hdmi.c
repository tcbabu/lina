#include "kulina.h"
#define _GNU_SOURCE
typedef struct _DevRec {
  char device[50];
  int hdmi;
  int Xres,Yres;
  int pos;
  Dlink *Resolist;
} DEVREC;
Dlink *Devlist=NULL;
char PrimDev[50],HdmiDev[50];
char **Plist=NULL,**Hlist=NULL;
int PReso,HReso,PVis=1,HVis=1,Hpos=0;
int ProcessXrandr(int pip0,int pip1,int Pid);
static int HDMI=0;
int Device=0;
int Xres,Yres;
int Cxres,Cyres,Dxres,Dyres,Pxres=-1,Pyres,Hxres=-1,Hyres;
int runjob(char *job,int (*ProcessOut)(int,int,int));
int ProcessXrandr(int pip0,int pip1,int Pid) {
     char buff[1000],device[20],connection[20],reso[30],*rpt;
     int ch,i=0,j,found=0,Vis;
     Dlink *Resolist;
     DEVREC *dpt;
     if(Devlist!= NULL) {
        Resetlink(Devlist);
        while((dpt=(DEVREC *)Getrecord(Devlist))!= NULL) {
           Dempty(dpt->Resolist);
        }
        Dempty(Devlist);
     }
     Devlist=Dopen();
//     close(pip1);
     Device=0;
     HDMI=0;
     while((ch=GetLine(pip0,buff)) ) {
         if(ch< 0) continue;
         i++;
         if(i==1) {
//          if(SearchString(buff,(char *)"Screen")>= 0) {
             j = SearchString(buff,(char *)"current");
             j+=7;
             i=j;
             while((buff[i]!='\n')&&(buff[i]!='\r')) {if(!isdigit(buff[i])) buff[i]=' ';i++;}
             sscanf(buff+j,"%d %d",&Cxres,&Cyres);
//             printf("Cxres:Cyres %d %d\n",Cxres,Cyres);
             Dxres=Cxres;
             Dyres=Cyres;
             if(Hxres==-1) {Hxres=Cxres;Hyres=Cyres;}
             if(Pxres==-1) {Pxres=Cxres;Pyres=Cyres;}
             continue;
         }
         if(buff[0]==' '){
           if(found) {
             rpt=(char *)malloc(30);
             sscanf(buff,"%s",rpt);
             Dadd(Resolist,rpt);
           }
           continue;
         }
         found=0;
         if(ch=='\n') {
           sscanf(buff,"%s%s%s",device,connection,reso);
           if(strcmp(connection,(char *)"connected")==0) {
             found=1;
             dpt= (DEVREC *)malloc(sizeof(DEVREC));
             Resolist=Dopen();
             dpt->Resolist=Resolist;
             strcpy(dpt->device,device);
             j=0;
             if(reso[0]=='(') Vis=0;
             else Vis=1;
             if(Vis) {
               while(reso[j]>' ') {
                 if(!isdigit(reso[j])) reso[j]=' ';
                 j++;
               }
               sscanf(reso,"%d%d",&Xres,&Yres);
             }
             device[4]='\0';
             dpt->Xres=Xres;
             dpt->Yres=Yres;
             dpt->pos=0;
             dpt->hdmi=0;
             if( strcmp(device,(char *)"HDMI")==0) {
               HDMI=1;
               dpt->hdmi=1;
               if(Vis) {
                 Hxres=Xres;
                 Hyres=Yres;
                 Dxres=Xres;
                 Dyres=Yres;
               }
               HVis=Vis;
             }
             else {
               PVis=Vis;
               if(Vis) {
                 Pxres=Xres;
                 Pyres=Yres;
               }
             }
             Dadd(Devlist,dpt);
             j=0; while(buff[j]!= '(')j++;
             buff[j]='\0';
           }
         }
     }
     if( (Cyres!=Hyres)&&(Cyres !=Pyres)) Hpos=1;
     else Hpos=0;
     return 1;
}
int CurrDevInfo(void) {
  int i,xr,yr,j,count;
  DEVREC *dpt;
  Dlink *Rlist;
  char *reso,buf[50];
  if(Plist != NULL) free(Plist);
  if(Hlist != NULL) free(Hlist);
  Plist=Hlist=NULL;
//  runjob("xrandr",ProcessXrandr);
  if(Devlist != NULL) {
    Resetlink(Devlist);
    while( (dpt=(DEVREC *)Getrecord(Devlist))!= NULL) {
      Rlist=dpt->Resolist;
      count=Dcount(Rlist);
      Resetlink(Rlist);
      if(dpt->hdmi==0) {
        if(Plist != NULL) continue;
        Plist = (char **)malloc(sizeof(char *)*(count+1));
        Plist[count]=NULL;
        strcpy(PrimDev,dpt->device);
        PReso=0;
        j=0;
        while((reso=(char *)Getrecord(Rlist))!= NULL) {
          strcpy(buf,reso);
          Plist[j]=(char *)malloc(strlen(reso)+1);
          strcpy(Plist[j],reso);
          i=0;
          while(buf[i]!='\0') {
           if(!isdigit(buf[i])) buf[i]=' ';
           i++;
          }
          sscanf(buf,"%d%d",&xr,&yr);
          if( (xr==dpt->Xres)&&(yr==dpt->Yres)) {PReso=j;}
          j++;
        }
      }
      else {
        if(Hlist != NULL) continue;
        Hlist = (char **)malloc(sizeof(char *)*(count+1));
        Hlist[count]=NULL;
        strcpy(HdmiDev,dpt->device);
        HReso=0;
        j=0;
        while((reso=(char *)Getrecord(Rlist))!= NULL) {
          strcpy(buf,reso);
          Hlist[j]=(char *)malloc(strlen(reso)+1);
          strcpy(Hlist[j],reso);
          i=0;
          while(buf[i]!='\0') {
           if(!isdigit(buf[i])) buf[i]=' ';
           i++;
          }
          sscanf(buf,"%d%d",&xr,&yr);
          if( (xr==dpt->Xres)&&(yr==dpt->Yres)) {HReso=j;}
          j++;
        }
      }
    }
  }
  return 1;
}
void  ResetHdmi(void) {
  int i=0,j,xr,yr;
  char buff[200];
  CurrDevInfo();
//  printf("%s %s\n",HdmiDev,Hlist[HReso]);
  if(Hlist==NULL) return;
  if(HVis==0 ) {
    HReso=0;
      sprintf(buff,"xrandr --output %-s --mode \"720x480\"",HdmiDev,Hlist[i]);
      runjob(buff,NULL);
    sprintf(buff,"xrandr --output %-s --mode %s",HdmiDev,Hlist[HReso]);
    runjob(buff,NULL);
  }
  return ;
}
int CheckHdmi(void) {
  if(!HDMI) {
    runjob("xrandr",ProcessXrandr);
    if(HDMI) ResetHdmi;
  }
  return HDMI;
}
