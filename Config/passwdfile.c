#include <kulina.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <shadow.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <crypt.h>
#include <grp.h>
#include "headers.h"
#include "passwdfile.h"
#include "uimages.c"
int WriteSessionsFile(Dlink *Slist);
int CheckString(char *s1,char *s2);
int SearchString(char *s1,char *s2);
void *MakeXsessionList(void);
#define Isize 42
#define Tsize 32
//Dlink *Ulist=NULL;
#define Skip {\
  while(buff[i]!=':')i++;\
  i++;\
}
#define GetField {\
  j=0;\
  while(buff[i]!=':') {\
    dummy[j]=buff[i];\
    i++;j++;\
  }\
  dummy[j]='\0';\
}
#define GetShellField {\
  j=0;\
  while((buff[i]!=':')&&(buff[i]>=' ')) {\
    dummy[j]=buff[i];\
    i++;j++;\
  }\
  dummy[j]='\0';\
}
#define CheckLine {\
  i=0;\
  if(buff== NULL) return NULL;\
  while(buff[i]==' ')i++;\
  if(buff[i]=='#') return NULL;\
  if(buff[i]=='+') return NULL;\
}
#define ProcessValuePointer(pt)  {\
  int j=0;\
  while (*pt==' ')pt++; \
  while(pt[j]>=' ') j++;\
  pt[j]='\0'; \
}
void * MakeMask(int w,int h,float fac) {
   void *fid,*img;

   fid = kgInitImage(w,h,1);
   kgUserFrame(fid,0.,0.,(float)(w+2),(float)(h+2));
   kgRoundedRectangleFill(fid,w*0.5,h*0.5,w+3.0,h+3.,0,0,0.0);
   kgRoundedRectangleFill(fid,w*0.5,h*0.5,w+1.0,h+1.,0,15,fac);
   img = kgGetResizedImage(fid);
   kgCloseImage(fid);
   return img;
}
char *GetLoginId(char *buff) {
  char dummy[500];
  char *Lid;
  int i=0,j=0;
  CheckLine;
  GetField;
  Lid=(char *)malloc(strlen(dummy)+1);
  strcpy(Lid,dummy);
  return Lid;
}
int GetUserNo(char *buff) {
  int UserId=-1;
  char dummy[500];
  char *Lid;
  int i=0,j=0;
//  CheckLine;
  i=0;
  Skip;
  Skip;
  GetField;
  sscanf(dummy,"%d",&UserId);
  return UserId;
}
int GetGrpId(char *buff) {
  int GrpId=-1;
  char dummy[500];
  char *Lid;
  int i=0,j=0;
//  CheckLine;
  i=0;
  Skip;
  Skip;
  Skip;
  GetField;
  sscanf(dummy,"%d",&GrpId);
  return GrpId;
}
char *GetUserName(char *buff) {
  char dummy[500];
  char *Name;
  int i=0,j=0;
  CheckLine;
  Skip;
  Skip;
  Skip;
  Skip;
  GetField;
  Name=(char *)malloc(strlen(dummy)+1);
  strcpy(Name,dummy);
  return Name;
}
char *GetUserHome(char *buff) {
  char dummy[500];
  char *Home;
  int i=0,j=0;
  CheckLine;
  Skip;
  Skip;
  Skip;
  Skip;
  Skip;
  GetField;
  if(dummy[0]=='\0') strcpy(dummy,"/");
  Home=(char *)malloc(strlen(dummy)+1);
  strcpy(Home,dummy);
  return Home;
}
char *GetUserShell(char *buff) {
  char dummy[500];
  char *Home;
  int i=0,j=0;
  CheckLine;
  Skip;
  Skip;
  Skip;
  Skip;
  Skip;
  Skip;
  GetShellField;
  if(dummy[0]=='\0') strcpy(dummy,"/");
  Home=(char *)malloc(strlen(dummy)+1);
  strcpy(Home,dummy);
  return Home;
}
char *GetUserPasswd(char *buff) {
  char dummy[500];
  char *Home;
  int i=0,j=0;
  CheckLine;
  Skip;
  GetField;
  Home=(char *)malloc(strlen(dummy)+1);
  strcpy(Home,dummy);
  return Home;
}

USERINFO * InitUserInfo(void) {
  USERINFO *Usr=NULL;
  Usr = (USERINFO *)malloc(sizeof(USERINFO));
  if(Usr==NULL) {
    fprintf(stderr,"Failed to Alloc Memory(InitUserInfo)\n");
    return NULL;
  }
  Usr->UserNo=-1;
  Usr->LoginId=NULL;
  Usr->Name=NULL;
  Usr->Passwd=NULL;
  Usr->Home=NULL;
  Usr->Shell=NULL;
  Usr->img=NULL;
  Usr->uthumb=NULL;
  Usr->Picture[0]='\0';
  return Usr;
}
void FreeUserInfo(USERINFO *Usr) {
   if(Usr==NULL) return;
   if(Usr->LoginId !=NULL) {free(Usr->LoginId);Usr->LoginId=NULL;}
   if(Usr->Name !=NULL) {free(Usr->Name);Usr->Name=NULL;}
   if(Usr->Passwd !=NULL) {free(Usr->Passwd);Usr->Passwd=NULL;}
   if(Usr->Home !=NULL) {free(Usr->Home);Usr->Home=NULL;}
   if(Usr->Shell !=NULL) {free(Usr->Shell);Usr->Shell=NULL;}
   if(Usr->img !=NULL) {kgFreeImage(Usr->img);Usr->img=NULL;}
//   if(Usr->rimg !=NULL) {kgFreeImage(Usr->rimg);Usr->rimg=NULL;}
   if(Usr->uthumb !=NULL) {kgFreeImage(Usr->uthumb);Usr->uthumb=NULL;}
   return;
}
int CompId(void *tmp1,void *tmp2) {
    char *id1,*id2;
    id1 = ((USERINFO *)tmp1)->LoginId;
    id2 = ((USERINFO *)tmp2)->LoginId;
    if(strcmp(id1,id2)>0) return 1;
    else return 0;
   
}
int CompExec(void *tmp1,void *tmp2) {
    char *id1,*id2;
    id1 = ((SESSIONINFO *)tmp1)->Command;
    id2 = ((SESSIONINFO *)tmp2)->Command;
    if(strcmp(id1,id2)==0) return 1;
    else return 0;
}
void MakeUserImages(LINACONFIG *lc,USERINFO *Usr) {
  void *img,*img1;
  FILE *fp;
  char buff[300],*pt;
  sprintf(buff,"%-s/.lina/picture",Usr->Home);
  fp = fopen(buff,"r");
  if(fp!= NULL) {
   if(fgets(buff,299,fp)!= NULL) {
//     strcpy(Usr->Picture,buff);
     pt = buff;
     ProcessValuePointer(pt);
//     sscanf(buff,"%s",Usr->Picture);
//     printf("User Picture: %s\n",buff);
     strncpy(Usr->Picture,pt,199);
     img1 = kgGetImage(pt);
     img = kgMaskImage(img1,lc->Mask);
     kgFreeImage(img1);
     Usr->img = kgChangeSizeImage(img,Isize,Isize);
//     Usr->img = img;
     Usr->uthumb= kgThumbNailImage(Usr->img,Tsize,Tsize);
     kgFreeImage(img);
     fclose(fp);
     return;
   }
   fclose(fp);
  }
  Usr->Picture[0]='\0';
  if(Usr->UserNo==0) {
    Usr->img = kgResizeImage(lc->Rimg,1.0);
    Usr->uthumb = kgResizeImage(lc->Rthumb,1.0);
  }
  else {
    Usr->img = kgResizeImage(lc->Uimg,1.0);
    Usr->uthumb = kgResizeImage(lc->Uthumb,1.0);
  }
  return;
}
void ReadUsersInfo(LINACONFIG *lc) {
   USERINFO *Usr;
   char *Shell=NULL;
   FILE *pf=NULL,*sf=NULL;
   char buff[500],*pt,*Pw,sbuff[500],*tmp,*tmp1;
   int UserNo;
   system("cat /etc/passwd 2>/tmp/.Junk1 >/tmp/.Junk");
   system("ypcat passwd 2>/tmp/.Junk1 >>/tmp/.Junk");
   pf = fopen("/tmp/.Junk","r");
   if(pf==NULL) return;
   lc->Ulist=Dopen();
   while (fgets(buff,499,pf) != NULL) {
      if( (pt=GetLoginId(buff))==NULL) continue;
      Shell=GetUserShell(buff);
      if(SearchString(Shell,"sh") < 0) { free(Shell);continue;}
      free(Shell);
      Shell = NULL;
#if 1
      Pw = GetUserPasswd(buff);
      if(Pw==NULL) continue;
      if(Pw[0]=='\0') { free(Pw);continue;}
      if((strlen(Pw)==1)&&(strcmp(Pw,"x")!=0)) {free(Pw);continue;}
      if((strlen(Pw)==1)&&(strcmp(Pw,"x")==0)) {
           free(Pw);
//           sf = fopen("/etc/shadow","r");
           system("cat /etc/shadow 2>/tmp/.Junk1 >/tmp/.Sjunk");
           system("ypcat shadow 2>/tmp/.Junk1 >>/tmp/.Sjunk");
           sf = fopen("/tmp/.Sjunk","r");
           if(sf==NULL) {
             fprintf(stderr,"Failed to open /etc/shadow\n");
             continue;
           }
           while (fgets(sbuff,499,sf) != NULL) {
              tmp=strchr(sbuff,':');
              tmp[0]='\0';tmp++;
              if(strcmp(sbuff,pt)!=0) continue;
              tmp1=strchr(tmp,':');
              tmp1[0]='\0';
              Pw=(char *)malloc(strlen(tmp)+1);
              strcpy(Pw,tmp);
              break;
           }
           fclose(sf);
           if((strlen(Pw)==1)&&(strcmp(Pw,"x")!=0)) {free(Pw);continue;}
           if((strlen(Pw)==1)&&(strcmp(Pw,"*")!=0)) {free(Pw);continue;}
           if((strlen(Pw)<=1)) {free(Pw);continue;}
      }
#endif
      UserNo=GetUserNo(buff);
      if(((lc->NoRootLogin!=1) &&(UserNo==0))||(UserNo>=100)){
//      if( (UserNo==0) || (UserNo>=100) ) {
        Usr= InitUserInfo();
        Usr->LoginId=pt;
        Usr->UserNo=UserNo;
        Usr->gid=GetGrpId(buff);
        Usr->Passwd=Pw;
        Usr->Name = GetUserName(buff);
        Usr->Home = GetUserHome(buff);
        Shell=GetUserShell(buff);
        Usr->Shell= Shell;
        MakeUserImages(lc,Usr);
        Dadd(lc->Ulist,Usr);
      }
      else {free(pt);free(Pw);}
   }
   Dsort(lc->Ulist,CompId);
   fclose(pf);
}
void PrintUsersInfo(LINACONFIG *lc) {
   USERINFO *Usr;
   Resetlink(lc->Ulist);
   while ((Usr=(USERINFO *)Getrecord(lc->Ulist))!= NULL) {
      printf("%s %d %s %s\n",Usr->LoginId,Usr->UserNo,Usr->Name,Usr->Home);
      printf("    : %s\n",Usr->Passwd);
   }
}
void CleanUsersInfo(LINACONFIG *lc) {
   USERINFO *Usr;
   if(lc->Ulist==NULL) return;
   Resetlink(lc->Ulist);
   while ((Usr=(USERINFO *)Getrecord(lc->Ulist))!= NULL) {
      FreeUserInfo(Usr);
   }
   Dempty(lc->Ulist);
   lc->Ulist=NULL;
}
/* Check for name and return its position 1 to last; otherwise return 0 */
int  GetIndexFromLogin(LINACONFIG *lc,char *LoginId) {
   USERINFO *Usr;
   int Id=0,i;
   Resetlink(lc->Ulist);
   i=0;
   while ((Usr=(USERINFO *)Getrecord(lc->Ulist))!= NULL) {
      i++;
      if(  strcmp(LoginId,Usr->LoginId)==0) {Id=i;break;}
   }
   return Id;
}
int  GetSessionFromName(LINACONFIG *lc,char *Name) {
   SESSIONINFO *Usr;
   int Id=0,i;
   Resetlink(lc->Slist);
   i=0;
   while ((Usr=(SESSIONINFO *)Getrecord(lc->Slist))!= NULL) {
      i++;
      if(  strcmp(Name,Usr->Name)==0) {Id=i;break;}
   }
   return Id;
}
char *GetUserId(LINACONFIG *lc,int no) {
   USERINFO *Usr;
   Dposition(lc->Ulist,no);
   Usr =(USERINFO *)Getrecord(lc->Ulist);
   return Usr->LoginId;
}
int GetGid(LINACONFIG *lc,int no) {
   USERINFO *Usr;
   Dposition(lc->Ulist,no);
   Usr =(USERINFO *)Getrecord(lc->Ulist);
   return Usr->gid;
}
int GetUid(LINACONFIG *lc,int no) {
   USERINFO *Usr;
   Dposition(lc->Ulist,no);
   Usr =(USERINFO *)Getrecord(lc->Ulist);
   return Usr->UserNo;
}
char *GetUserPw(LINACONFIG *lc,int no) {
   USERINFO *Usr;
   Dposition(lc->Ulist,no);
   Usr =(USERINFO *)Getrecord(lc->Ulist);
   return Usr->Passwd;
}
char *GetRealName(LINACONFIG *lc,int no) {
   USERINFO *Usr;
   Dposition(lc->Ulist,no);
   Usr =(USERINFO *)Getrecord(lc->Ulist);
   if(Usr==NULL) return NULL;
   if( (Usr->Name== NULL) ||(Usr->Name[0]=='\0')) {
    return Usr->LoginId;
   }
   else return Usr->Name;
}
char *GetHomeDir(LINACONFIG *lc,int no) {
   USERINFO *Usr;
   Dposition(lc->Ulist,no);
   Usr =(USERINFO *)Getrecord(lc->Ulist);
   return Usr->Home;
}
char *GetShell(LINACONFIG *lc,int no) {
   USERINFO *Usr;
   Dposition(lc->Ulist,no);
   Usr =(USERINFO *)Getrecord(lc->Ulist);
   return Usr->Shell;
}
void *GetUserImage(LINACONFIG *lc,int no) {
   USERINFO *Usr;
   Dposition(lc->Ulist,no);
   Usr =(USERINFO *)Getrecord(lc->Ulist);
   return Usr->img;
}
ThumbNail **MakeUserThumbNails(LINACONFIG *lc) {
  ThumbNail **th=NULL;
  int i,n;
  USERINFO *Usr;
  if(lc->Ulist==NULL) ReadUsersInfo(lc);
  n = Dcount(lc->Ulist)+1;
  th = (ThumbNail **) malloc(sizeof(ThumbNail*)*n);
  th[n-1]=NULL;
  Resetlink(lc->Ulist);
  i=0;
  while ((Usr=(USERINFO *)Getrecord(lc->Ulist))!= NULL) {
//    printf("%s %d %s %s\n",Usr->LoginId,Usr->UserNo,Usr->Name,Usr->Home);
      th[i]= (ThumbNail *) malloc(sizeof(ThumbNail));
      th[i]->name=(char *)malloc(strlen(Usr->LoginId)+1);
      strcpy(th[i]->name,Usr->LoginId);
      th[i]->img=kgGetImageCopy(NULL,Usr->uthumb);
      th[i]->sw=0;
      th[i]->id=i;
      th[i]->state=1;
      i++;
  }
  return th;
}
ThumbNail **MakeSessionThumbNails(LINACONFIG *lc) {
  ThumbNail **th=NULL;
  int i,n;
  SESSIONINFO *ses;
  if(lc->Slist==NULL) {
    printf("lc->Slist== NULL\n");
    return NULL;
  }
  n = Dcount(lc->Slist)+1;
  th = (ThumbNail **) malloc(sizeof(ThumbNail*)*n);
  th[n-1]=NULL;
  Resetlink(lc->Slist);
  i=0;
  while ((ses=(SESSIONINFO *)Getrecord(lc->Slist))!= NULL) {
      th[i]= (ThumbNail *) malloc(sizeof(ThumbNail));
      th[i]->name=(char *)malloc(strlen(ses->Name)+1);
      strcpy(th[i]->name,ses->Name);
      th[i]->img=NULL;
      th[i]->sw=0;
      th[i]->id=i;
      th[i]->state=1;
      i++;
  }
  th[n-1]=NULL;
  return th;
}
char *GetSessionCommand(LINACONFIG *lc,int Index) {
  int i,n;
  SESSIONINFO *ses;
  if(lc->Slist==NULL) {
    printf("lc->Slist== NULL\n");
    return NULL;
  }
  Dposition(lc->Slist,Index);
  ses=(SESSIONINFO *)Getrecord(lc->Slist);
  if(ses==NULL) return NULL;
  if(ses->Command[0]=='\0') return NULL;
  else return ses->Command;
}
int ProcessPasswd(char *Passwd) {
  int i,l;
  l = strlen(Passwd)-1;
  i=l;
  while( (i> 0)&&(Passwd[i]<=' ')) {Passwd[i]='\0';i--;}
  return 1;
}
int CheckLogin(LINACONFIG *lc,int Index,char *password) {
    int ret=0;
    char *p, *correct, *supplied, *salt;
    if (Index<=0 ) return 0;
    ProcessPasswd(password);
    correct = GetUserPw(lc,Index);
    salt = strdup(correct);
    if (salt == NULL) return 2;
    p = strchr(salt + 1, '$');
    if (p == NULL) ret= 2;
    else {
     p = strchr(p + 1, '$');
     if (p == NULL) ret= 3;
     else {
      p[0] = 0;
      supplied = crypt(password, salt);
      if (supplied == NULL) ret= 4;
      else ret= !strcmp(supplied, correct);
     }
    }
    free(salt);
    return ret;
}
int InitConfig(LINACONFIG *lc) {
  lc->Red=216;
  lc->Green=226;
  lc->Blue=216;
  lc->ButRed=216;
  lc->ButGreen=226;
  lc->ButBlue=216;
  lc->DateRed=216;
  lc->DateGreen=226;
  lc->DateBlue=216;
  lc->FontRed=39;
  lc->FontGreen=44;
  lc->FontBlue=39;
  lc->HighRed=226;
  lc->HighGreen=255;
  lc->HighBlue=255;
  lc->Transparency = 0.1;
  lc->fac = 0.0;
  lc->Bkgr[0]='\0';
  lc->RootPic[0]='\0';
  lc->UserPic[0]='\0';
  lc->Ulist=NULL;
  lc->Slist=NULL;
  lc->Rthumb=NULL;
  lc->Uthumb=NULL;
  lc->Uimg=NULL;
  lc->Mask=NULL;
  strcpy(lc->DefUser,"Guest");
  strcpy(lc->DefSession,"XFCE");
  lc->Action=1;
  lc->Powerdown=3;
  lc->Session=1;
  lc->NoRootLogin=1;
  lc->TextMode=1;
  lc->SafeMode=1;
  lc->ShowTime=1;
  lc->DateFont=25;
}
#define SkipCommentLine {\
  i=0;\
  while(buff[i]==' ')i++;\
  if(buff[i]=='#') continue;\
}
void *ReadConfig(LINACONFIG *lc) {
  void *img,*img1;
  FILE *fp;
  struct stat fstat;
  SESSIONINFO *spt;
  char buff[300],*pt,Field[60],Value[300];
  int i,j;
  InitConfig(lc);
  lc->Mask = MakeMask(Isize*4,Isize*4,0.5);

  fp = fopen("/usr/share/config/lina/linarc","r");
  if(fp==NULL){
     fp = fopen("/etc/xdg/lina/linarc","r");
     if(fp!=NULL) {
       lstat("/etc/xdg/lina/linarc",&fstat);
       if( (fstat.st_mode&00077)!=0) {
         printf("Others Got Access To File...\n");
         fclose(fp);
         fp=NULL;
       }
     }
  }
  else {
    lstat("/usr/share/config/lina/linarc",&fstat);
    if( (fstat.st_mode&00077)!=0) {
      printf("Others Got Access To File...\n");
      fclose(fp);
      fp=NULL;
    }
  }
  if(fp != NULL) {
    while(fgets(buff,299,fp) != NULL) {
       SkipCommentLine;
       pt=strchr(buff,':');
       if(pt==NULL) continue;
       pt[0]='\0';
       pt++;
       if(sscanf(buff,"%s",Field)<= 0) continue;
       ProcessValuePointer(pt);
       if( strcmp(Field,"BackGround")==0 ) {
         strcpy(lc->Bkgr,pt);
         continue;
       }
       if( strcmp(Field,"RootPicture")==0 ) {
         strcpy(lc->RootPic,pt);
         continue;
       }
       if( strcmp(Field,"UserPicture")==0 ) {
//         printf("%s\n",pt);
//         fflush(stdout);
         strcpy(lc->UserPic,pt);
         continue;
       }
       if( strcmp(Field,"DefaultUser")==0 ) {
         strncpy(lc->DefUser,pt,29);
         continue;
       }
       if( strcmp(Field,"DefaultSession")==0 ) {
         strncpy(lc->DefSession,pt,29);
         continue;
       }
       if( strcmp(Field,"Red")==0 ) {
         sscanf(pt,"%d",&(lc->Red));
         if(lc->Red < 0) lc->Red=0;
         if(lc->Red > 255) lc->Red=255;
         continue;
       }
       if( strcmp(Field,"Green")==0 ) {
         sscanf(pt,"%d",&(lc->Green));
         if(lc->Green < 0) lc->Green=0;
         if(lc->Green > 255) lc->Green=255;
         continue;
       }
       if( strcmp(Field,"Blue")==0 ) {
         sscanf(pt,"%d",&(lc->Blue));
         if(lc->Blue < 0) lc->Blue=0;
         if(lc->Blue > 255) lc->Blue=255;
         continue;
       }
       if( strcmp(Field,"FontRed")==0 ) {
         sscanf(pt,"%d",&(lc->FontRed));
         if(lc->FontRed < 0) lc->FontRed=0;
         if(lc->FontRed > 255) lc->FontRed=255;
         continue;
       }
       if( strcmp(Field,"FontGreen")==0 ) {
         sscanf(pt,"%d",&(lc->FontGreen));
         if(lc->FontGreen < 0) lc->FontGreen=0;
         if(lc->FontGreen > 255) lc->FontGreen=255;
         continue;
       }
       if( strcmp(Field,"FontBlue")==0 ) {
         sscanf(pt,"%d",&(lc->FontBlue));
         if(lc->FontBlue < 0) lc->FontBlue=0;
         if(lc->FontBlue > 255) lc->FontBlue=255;
         continue;
       }
       if( strcmp(Field,"HighRed")==0 ) {
         sscanf(pt,"%d",&(lc->HighRed));
         if(lc->HighRed < 0) lc->HighRed=0;
         if(lc->HighRed > 255) lc->HighRed=255;
         continue;
       }
       if( strcmp(Field,"HighGreen")==0 ) {
         sscanf(pt,"%d",&(lc->HighGreen));
         if(lc->HighGreen < 0) lc->HighGreen=0;
         if(lc->HighGreen > 255) lc->HighGreen=255;
         continue;
       }
       if( strcmp(Field,"HighBlue")==0 ) {
         sscanf(pt,"%d",&(lc->HighBlue));
         if(lc->HighBlue < 0) lc->HighBlue=0;
         if(lc->HighBlue > 255) lc->HighBlue=255;
         continue;
       }
       if( strcmp(Field,"ButRed")==0 ) {
         sscanf(pt,"%d",&(lc->ButRed));
         if(lc->ButRed < 0) lc->ButRed=0;
         if(lc->ButRed > 255) lc->ButRed=255;
         continue;
       }
       if( strcmp(Field,"ButGreen")==0 ) {
         sscanf(pt,"%d",&(lc->ButGreen));
         if(lc->ButGreen < 0) lc->ButGreen=0;
         if(lc->ButGreen > 255) lc->ButGreen=255;
         continue;
       }
       if( strcmp(Field,"ButBlue")==0 ) {
         sscanf(pt,"%d",&(lc->ButBlue));
         if(lc->ButBlue < 0) lc->ButBlue=0;
         if(lc->ButBlue > 255) lc->ButBlue=255;
         continue;
       }
       if( strcmp(Field,"DateRed")==0 ) {
         sscanf(pt,"%d",&(lc->DateRed));
         if(lc->DateRed < 0) lc->DateRed=0;
         if(lc->DateRed > 255) lc->DateRed=255;
         continue;
       }
       if( strcmp(Field,"DateGreen")==0 ) {
         sscanf(pt,"%d",&(lc->DateGreen));
         if(lc->DateGreen < 0) lc->DateGreen=0;
         if(lc->DateGreen > 255) lc->DateGreen=255;
         continue;
       }
       if( strcmp(Field,"DateBlue")==0 ) {
         sscanf(pt,"%d",&(lc->DateBlue));
         if(lc->DateBlue < 0) lc->DateBlue=0;
         if(lc->DateBlue > 255) lc->DateBlue=255;
         continue;
       }
       if( strcmp(Field,"Transparency")==0 ) {
         sscanf(pt,"%f",&(lc->Transparency));
         if(lc->Transparency < 0.001) lc->Transparency=0.001;
         if(lc->Transparency > 1.0) lc->Transparency=1.0;
         continue;
       }
       if( strcmp(Field,"RoundingFac")==0 ) {
         sscanf(pt,"%f",&(lc->fac));
         if(lc->fac < 0) lc->fac=0;
         if(lc->fac > 1.0) lc->fac =1.0;
         continue;
       }
       if( strcmp(Field,"DateFont")==0 ) {
         sscanf(pt,"%d",&(lc->DateFont));
         if(lc->DateFont < 0) lc->DateFont=0;
         if(lc->DateFont > 35) lc->DateFont =35;
         continue;
       }
       if( strcmp(Field,"RootLogin")==0 ) {
         lc->NoRootLogin=1;
         printf("%s :%s:\n",Field,pt);
         if((strcmp(pt,"yes")==0)||(strcmp(pt,"Yes")==0)||(strcmp(pt,"YES")==0)) {
          lc->NoRootLogin=0;
         }
         if((strcmp(pt,"no")==0)||(strcmp(pt,"No")==0)||(strcmp(pt,"NO")==0)) {
          lc->NoRootLogin=1;
         }
         continue;
       }
       if( strcmp(Field,"TextMode")==0 ) {
         lc->TextMode=1;
         printf("%s :%s:\n",Field,pt);
         if((strcmp(pt,"yes")==0)||(strcmp(pt,"Yes")==0)||(strcmp(pt,"YES")==0)) {
          lc->TextMode=1;
         }
         if((strcmp(pt,"no")==0)||(strcmp(pt,"No")==0)||(strcmp(pt,"NO")==0)) {
          lc->TextMode=0;
         }
         continue;
       }
       if( strcmp(Field,"SafeMode")==0 ) {
         lc->SafeMode=1;
         printf("%s :%s:\n",Field,pt);
         if((strcmp(pt,"yes")==0)||(strcmp(pt,"Yes")==0)||(strcmp(pt,"YES")==0)) {
          lc->SafeMode=1;
         }
         if((strcmp(pt,"no")==0)||(strcmp(pt,"No")==0)||(strcmp(pt,"NO")==0)) {
          lc->SafeMode=0;
         }
         continue;
       }
       if( strcmp(Field,"ShowTime")==0 ) {
         lc->ShowTime=1;
         printf("%s :%s:\n",Field,pt);
         if((strcmp(pt,"yes")==0)||(strcmp(pt,"Yes")==0)||(strcmp(pt,"YES")==0)) {
          lc->ShowTime=1;
         }
         if((strcmp(pt,"no")==0)||(strcmp(pt,"No")==0)||(strcmp(pt,"NO")==0)) {
          lc->ShowTime=0;
         }
         continue;
       }
    } // while
    fclose(fp);
  }
  img=NULL;
  if(lc->RootPic[0]!='\0')  {
     img = kgGetImage(lc->RootPic);
  }
  if(img==NULL) {
       img = kgGetImageCopy(NULL,&rimg_str);
  }
  img1 = kgMaskImage(img,lc->Mask);
  lc->Rimg = kgChangeSizeImage(img1,Isize,Isize);
  lc->Rthumb = kgChangeSizeImage(img1,Tsize,Tsize);
  kgFreeImage(img);
  kgFreeImage(img1);
  img=NULL;
  if(lc->UserPic[0]!='\0')  {
     img = kgGetImage(lc->UserPic);
  }
  if(img==NULL) {
       img = kgGetImageCopy(NULL,&uimg_str);
  }
  img1 = kgMaskImage(img,lc->Mask);
  lc->Uimg = kgChangeSizeImage(img1,Isize,Isize);
  lc->Uthumb = kgChangeSizeImage(img1,Tsize,Tsize);
  kgFreeImage(img);
  kgFreeImage(img1);
  lc->Slist=(Dlink *)MakeXsessionList();
  printf("Xsession: count=%d\n",Dcount(lc->Slist));
  if(lc->Slist==NULL)lc->Slist=Dopen();
  fp = fopen("/usr/share/config/lina/session","r");
  if(fp==NULL) fopen("/etc/xdg/lina/session","r");
  if(fp != NULL) {
    while(fgets(buff,299,fp) != NULL) {
       SkipCommentLine;
       pt=strchr(buff,':');
       pt[0]='\0';
       pt++;
//       while(*pt==' ') pt++;
//       j=0;while(pt[j]>= ' ')j++;
//       pt[j]='\0';
       if(sscanf(buff,"%s",Field)<= 0) continue;
       ProcessValuePointer(pt);
//       sscanf(pt,"%s",Value);
       spt = (SESSIONINFO *)malloc(sizeof(SESSIONINFO));
       strncpy(spt->Name,Field,29);
       strncpy(spt->Command,pt,99);
       Dadd(lc->Slist,spt);
    }
    fclose(fp);
  } //if
  else {
    spt = (SESSIONINFO *)malloc(sizeof(SESSIONINFO));
    strcpy(spt->Name,"Kulina Safe");
    strcpy(spt->Command,"kglaunch");
    Dadd(lc->Slist,spt);
    WriteSessionsFile(lc->Slist);
  }
  if(lc->TextMode) {
    spt = (SESSIONINFO *)malloc(sizeof(SESSIONINFO));
    strcpy(spt->Name,"Text Console");
    spt->Command[0]='\0';
    Dappend(lc->Slist,spt);
  }
  if(lc->SafeMode) {
    spt = (SESSIONINFO *)malloc(sizeof(SESSIONINFO));
    strcpy(spt->Name,"Fail Safe");
    strcpy(spt->Command,"xterm -geometry 80x24-0-0");
    Dappend(lc->Slist,spt);
  }
  Drmvdup_cond(lc->Slist,CompExec);
#if 0
  if(lc->RootPic[0]=='\0') {
    lc->Rimg = kgGetImageCopy(NULL,&rimg_str);
    lc->Rthumb = kgGetImageCopy(NULL,&rthumb_str);
  }
  else {
    img = kgGetImage(lc->RootPic);
    lc->Rimg = kgChangeSizeImage(img,Isize,Isize);
    lc->Rthumb = kgChangeSizeImage(lc->Rimg,Tsize,Tsize);
    kgFreeImage(img);
  }
  if(lc->UserPic[0]=='\0') {
    lc->Uimg = kgGetImageCopy(NULL,&uimg_str);
    lc->Uthumb = kgGetImageCopy(NULL,&uthumb_str);
  }
  else {
    img = kgGetImage(lc->UserPic);
    lc->Uimg = kgChangeSizeImage(img,Isize,Isize);
    lc->Uthumb = kgChangeSizeImage(lc->Uimg,Tsize,Tsize);
    kgFreeImage(img);
  }
#endif
  ReadUsersInfo(lc);
}
void CleanLinaConfig(LINACONFIG *lc) {
    CleanUsersInfo(lc);
    Dempty(lc->Slist);
    kgFreeImage(lc->Rimg);
    kgFreeImage(lc->Uimg);
    kgFreeImage(lc->Rthumb);
    kgFreeImage(lc->Uthumb);
    kgFreeImage(lc->Mask);
}

void WriteConfig(LINACONFIG *lc) {
   FILE *fp;
   mkdir("/usr/share/config",0755);
   mkdir("/usr/share/config/lina",0755);
   fp = fopen("/usr/share/config/lina/linarc","w");
   if(fp==NULL) return;
   fprintf(fp,"BackGround :%-s\n",lc->Bkgr);
   fprintf(fp,"Red :%-d\n",lc->Red);
   fprintf(fp,"Green :%-d\n",lc->Green);
   fprintf(fp,"Blue :%-d\n",lc->Blue);
   fprintf(fp,"HighRed :%-d\n",lc->HighRed);
   fprintf(fp,"HighGreen :%-d\n",lc->HighGreen);
   fprintf(fp,"HighBlue :%-d\n",lc->HighBlue);
   fprintf(fp,"FontRed :%-d\n",lc->FontRed);
   fprintf(fp,"FontGreen :%-d\n",lc->FontGreen);
   fprintf(fp,"FontBlue :%-d\n",lc->FontBlue);
   fprintf(fp,"ButRed :%-d\n",lc->ButRed);
   fprintf(fp,"ButGreen :%-d\n",lc->ButGreen);
   fprintf(fp,"ButBlue :%-d\n",lc->ButBlue);
   fprintf(fp,"DateRed :%-d\n",lc->DateRed);
   fprintf(fp,"DateGreen :%-d\n",lc->DateGreen);
   fprintf(fp,"DateBlue :%-d\n",lc->DateBlue);
   fprintf(fp,"Transparency :%-f\n",(float)(lc->Transparency));
   fprintf(fp,"RoundingFac :%-f\n",(float)(lc->fac));
   fprintf(fp,"DateFont :%-d\n",(int)(lc->DateFont));
   fprintf(fp,"DefaultUser :%-s\n",(lc->DefUser));
   fprintf(fp,"DefaultSession :%-s\n",(lc->DefSession));
   if(lc->NoRootLogin) fprintf(fp,"RootLogin :no\n");
   else  fprintf(fp,"RootLogin :yes\n");
   if(lc->TextMode) fprintf(fp,"TextMode :yes\n");
   else  fprintf(fp,"TextMode :no\n");
   if(lc->SafeMode) fprintf(fp,"SafeMode :yes\n");
   else  fprintf(fp,"SafeMode :no\n");
   if(lc->ShowTime) fprintf(fp,"ShowTime :yes\n");
   else  fprintf(fp,"ShowTime :no\n");
   fclose(fp);
   chmod("/usr/share/config/lina/linarc",0700);
   remove("/etc/xdg/lina/linarc");
}
int  SetSupGroups(LINACONFIG *lc,int Index) {
   FILE *pf=NULL;
   char buff[2000],*pt,*Pw,*UserName;
   int GroupNo,Ugid,*ipt,count,*array,i;
   Dlink *Glist=NULL;
   pf = popen("cat /etc/group","r");
   if(pf==NULL) return 0;
   Glist=Dopen();
   UserName = GetUserId(lc,Index); 
   Ugid = GetGid(lc,Index);
   while (fgets(buff,1999,pf) != NULL) {
      if( (pt=GetLoginId(buff))==NULL) continue; // Actually Group Name
      Pw = GetUserPasswd(buff);
      if(Pw==NULL) continue;
      if(Pw[0]=='\0') { free(Pw);continue;}
      GroupNo=GetUserNo(buff);
      if(GroupNo==Ugid) {free(Pw);continue;}
      if((i=SearchString(buff,UserName))>= 0) {
        int j=0;
        j =i;
        while((buff[j]>' ') &&(buff[j]!=',')&&(buff[j]!=':')) j++;
        buff[j]='\0';
        if(strcmp(buff+i,UserName)==0) {
          ipt=(int *)malloc(sizeof(int));
          *ipt = GroupNo;
          Dadd(Glist,ipt);
//          printf("%s : %d\n",pt,*ipt);
          printf("%d ",*ipt);
        }
      }
      free(pt);free(Pw);
   }
   count = Dcount(Glist);
   printf("  :Supp group count = %d\n",count);
   fflush(stdout);
   if(count > 0) {
     array = (int *)malloc(sizeof(int)*(count));
     Resetlink(Glist);
     i=0;
     while( (ipt = (int *)Getrecord(Glist))!= NULL) {
      array[i++]=*ipt;
     }
     setgroups(count,array);
     free(array);
   }
   pclose(pf);
   Dempty(Glist);
   return count;
}
Dlink *ReadSessionsFile(void) {
  int i;
  char buff[500],Field[35];
  FILE *fp;
  char *pt;
  Dlink *Slist=NULL;
  SESSIONINFO *spt;
  Slist=Dopen();
  fp = fopen("/usr/share/config/lina/session","r");
  if(fp==NULL) fp=fopen("/etc/xdg/lina/session","r");
  if(fp==NULL) {
    fp = fopen("/usr/share/config/lina/session","w");
    if(fp != NULL) {
      fprintf(fp,"KulinaSafe: kglaunch\n");
      fclose(fp);
      fp = fopen("/usr/share/config/lina/session","r");
    }
  }
  if(fp != NULL) {
    while(fgets(buff,299,fp) != NULL) {
       SkipCommentLine;
       pt=strchr(buff,':');
       pt[0]='\0';
       pt++;
//       while(*pt==' ') pt++;
//       j=0;while(pt[j]>= ' ')j++;
//       pt[j]='\0';
       ProcessValuePointer(pt);
       sscanf(buff,"%s",Field);
//       sscanf(pt,"%s",Value);
       spt = (SESSIONINFO *)malloc(sizeof(SESSIONINFO));
       strncpy(spt->Name,Field,29);
       strncpy(spt->Command,pt,99);
       Dadd(Slist,spt);
    }
    fclose(fp);
  } //if
  return Slist;
}
int WriteSessionsFile(Dlink *Slist) {
  char buff[500],Field[35];
  FILE *fp;
  char *pt;
  SESSIONINFO *spt;
  fp = fopen("/usr/share/config/lina/session","w");
  if(fp != NULL) {
    Resetlink(Slist);
    while( (spt= (SESSIONINFO *)Getrecord(Slist)) != NULL){
       fprintf(fp,"%s : %s\n",spt->Name,spt->Command);
    }
    fclose(fp);
  } //if
  chmod("/usr/share/config/lina/session",0700);
  remove("/etc/xdg/lina/session");
}
ThumbNail **MakeSessionFileThumbNails(Dlink *Slist) {
  ThumbNail **th=NULL;
  int i,n;
  SESSIONINFO *ses;
  if(Slist==NULL) {
    printf("Slist== NULL\n");
    return NULL;
  }
  n = Dcount(Slist)+1;
  th = (ThumbNail **) malloc(sizeof(ThumbNail*)*n);
  th[n-1]=NULL;
  Resetlink(Slist);
  i=0;
  while ((ses=(SESSIONINFO *)Getrecord(Slist))!= NULL) {
      th[i]= (ThumbNail *) malloc(sizeof(ThumbNail));
      th[i]->name=(char *)malloc(strlen(ses->Name)+1);
      strcpy(th[i]->name,ses->Name);
      th[i]->img=NULL;
      th[i]->sw=0;
      th[i]->id=i;
      th[i]->state=1;
      i++;
  }
  th[n-1]=NULL;
  return th;
}
