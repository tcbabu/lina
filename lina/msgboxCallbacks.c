#include <kulina.h>
char *kgWhich(char *pgr);
static char Curssid[200];
static int OK=0,Connected=0;
static DIS *S;
static char **MenuItems=NULL;
static Dlink *Mslist=NULL;
int runjob(char *job,int (*ProcessOut)(int,int,int));
int wirediainit(void *Tmp);
static int AddtoList(char *buff) {
  int ln;
  char *pt;
  ln = strlen(buff)+1;
  pt = (char *)malloc(ln);
  strcpy(pt,buff);
  Dadd(Mslist,pt);
  return 1;
}
static int AddtoList1(char *buff) {
  int ln;
  char *pt;
  ln = strlen(buff)+6;
  pt = (char *)malloc(ln);
  strcpy(pt,"!g   ");
  strcpy(pt+5,buff);
  Dadd(Mslist,pt);
  return 1;
}
static int ProcessStatus(int pip0,int pip1,int Pid) {
     char buff[1000];
     int ch,i=0,j;
//   Skipa line
     Curssid[0]='\0';
     OK=0;
     while((ch=GetLine(pip0,buff)) ) {
         if(ch< 0) {continue;}
         if((j=SearchString(buff,(char *)"Failed to connect"))>=0) {
           OK=0;
           break;
         }
//         printf("  %s",buff);
         AddtoList1(buff);
         if((j=SearchString(buff,(char *)"ssid"))>=0) {
           if((j=SearchString(buff,(char *)"="))>=0) {
             j++;
             i=j;
             while(buff[i]>=' ') i++;
             buff[i]='\0';
             strcpy(Curssid,buff+j);
 //            sprintf(buff, "SSID : %s",Curssid);
//             AddtoList(buff);
             OK=1;
           }
         }
     }
     
     return OK;
}
static int runbin(char *job){
   FILE *fp,*fp1;
   char *args[50],buff[1000],command[200],pt[200];
   int pip[2],pid,pipe2[2];
   char *tmp=NULL,esc=27;
   char *pgrpath=NULL;
   int i=0,pos=0,wsec=20;
   if(job==NULL) return 0;
   strcpy(buff,job);
   while ( sscanf(buff+pos,"%s",pt) > 0 ) {
//     printf("%s\n",pt);
     args[i]=buff+pos;
     pos +=strlen(pt);
     i++;
     if(buff[pos]< ' ') break;
     buff[pos]='\0';
     pos++;
     while(buff[pos]==' ') pos++;
//     printf("%s\n",args[i-1]);
   }
   args[i]=NULL;
   if(i==0) return 0;
   pgrpath=kgWhich(args[0]);
   if (pgrpath==NULL) return 0;

   if( pipe(pip) < 0) return 0;
   if( pipe(pipe2) < 0) return 0;
   pid = fork();
   if(pid == 0) { /* child process */
     close(pipe2[0]);
     close(pipe2[1]);
     if(fork()!=0) exit(0); /* to avoid zombie */
//     signal(SIGUSR1,sigproc);
     close(pip[0]);
     close(pip[0]);
     close(2);
//     dup(1);
     close(1);
     dup(pip[1]);
     dup(pip[1]);
     close(pip[1]);
     execv(pgrpath,args);
     printf("Failed to execute : %s \n",job);
     exit(1);
   }
   else {   /* parent process */
     char ch;
     fd_set rfds;
     struct timeval tv;
     int retval,chnl,status;
     int ret,chars,lines,xo = 133,yo=11;
     waitpid(pid,&status,0);
     close(pip[1]);
     if(fork() != 0)   return pipe2[0];
     close(pipe2[0]);
     FD_ZERO(&rfds);
     FD_SET(pip[0],&rfds);
     chnl = pip[0];
     while(1) {
       i=0;
       tv.tv_sec = wsec;
       tv.tv_usec =0;
       FD_ZERO(&rfds);
       FD_SET(pip[0],&rfds);
       retval = select(chnl+1,&rfds,NULL,NULL,&tv);
       /*retval = select(chnl+1,&rfds,NULL,NULL,NULL);*/
       if((retval> 0)&&(FD_ISSET(chnl,&rfds))){
         chars=read(chnl,&ch,1);
//         printf("chars: %d%c\n",chars,ch);
         if(chars== 0){
             buff[0]='\0';
             write(pipe2[1],buff,1);
//             printf("Closing Pipe\n");
             close(pipe2[1]);
             exit(0);
         };
         while(1) {
           buff[i++]=ch;
           if(ch=='\n') break;
           chars=read(chnl,&ch,1);
           if(chars== 0) break;
         }
         buff[i]='\0';
         buff[99]='\0';
//         printf("%s\n",buff);
         write(pipe2[1],buff,100);
         buff[17]='\0';
         if(strcmp(buff,"--> secondary DNS")==0) {
           wsec=20000;
         }
       }
       else {
         strcpy(buff,"--> Modem not res");
         write(pipe2[1],buff,100);
//         printf("EOF\n");
//         close(pipe2[1]);
//         exit(0);
       }
     }
   }
}
static char * GetWdev(void) {
  char **devs,**files,buff[200];
  int i,j,OK=0;
  char *Wdev = NULL;
  devs=kgFolderMenu("/sys/class/net");
  if(Wdev != NULL) { OK=1; return NULL;}
  if(devs!=NULL) {
   i=0;
   while(devs[i]!=NULL) {
     strcpy(buff,"/sys/class/net/");
     strcat(buff,devs[i]);
     files=kgFolderMenu(buff);
     if(files!= NULL) {
       j=0;
       while(files[j]!=NULL) {
          if(SearchString(files[j],"wireless")>=0) {
            Wdev = (char *) malloc(strlen(devs[i])+1);
            strcpy(Wdev,devs[i]);
            OK=1;
            break;
          }
          j++;
       }
     }
     kgFreeDouble((void **)files);
     if(OK) break;
     i++;
   }
   kgFreeDouble((void **)devs);
  }
//  printf("Wireless Device : %s\n",Wdev);
  return Wdev;
}
static int WirelessStatus(void) {
  char buff[200];
  char ipaddr[50];
  int n,pp2,OK=0;
  char *pt;
  char *Wdev=NULL;
  Connected=0;
  AddtoList(" ");
  if((Wdev=GetWdev())==NULL) return 0;
  sprintf(buff,"ip addr list dev %-s",Wdev);
  pp2=runbin(buff);
  if((n=read(pp2,buff,100)) ==100) {
    if( SearchString(buff,"DOWN")<=0) {
      if( SearchString(buff,"UP")>=0) {
        while((n=read(pp2,buff,100)) ==100) {
          if( (pt = strstr(buff,"inet "))!= NULL) {
            pt +=5;
            OK=1;
            sscanf(pt,"%s",ipaddr);
            Connected=1;
//            printf("  %s connected: IP Addr: %s\n",Wdev,ipaddr);
            sprintf(buff,"!z43!w54 %s Connected" ,Wdev);
            AddtoList(buff);
            sprintf(buff,"!g   IP Addr: %s\n",ipaddr);
            AddtoList(buff);
            break;
          }
        }
      }
    }
  }
  if(Connected) {
    runjob("wpa_cli status",ProcessStatus);
  }
  else {
    fprintf(stderr,"Not Connected\n");
  }
 
  return OK;

}
void  msgboxbrowser1init(DIS *S,void *pt) {
   int count=0,i;
   Mslist = Dopen();
   WirelessStatus();
   count = Dcount(Mslist)+1;
   MenuItems= (char **) malloc(sizeof(char *)*count);
   Resetlink(Mslist);
   i=0;
   while( (pt=(char *)Getrecord(Mslist))!= NULL) {
     MenuItems[i]=pt;
     i++;
   }
   MenuItems[i]=NULL;
   S->menu = MenuItems;
   Dfree(Mslist);
   Mslist=NULL;

}
int msgboxinit(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  /* you add any initialisation here */
  int ret = 1;
  DIALOG *D;void *pt;
  D = (DIALOG *)Tmp;
  pt = D->pt;
  return ret;
}
int msgboxcleanup(void *Tmp) {
  /* you add any cleanup/mem free here */
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  int ret = 1;
  DIALOG *D;void *pt;
  D = (DIALOG *)Tmp;
  pt = D->pt;
  kgFreeDouble((void **)MenuItems);
  MenuItems=NULL;
  return ret;
}
int Modifymsgbox(void *Tmp,int GrpId) {
  DIALOG *D;
  D = (DIALOG *)Tmp;
  DIA *d;
  int i,n;
  d = D->d;
  i=0;while(d[i].t!= NULL) {;
     i++;
  };
  n=1;
  return GrpId;
}

int msgboxCallBack(void *Tmp,void *tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    tmp :  Pointer to KBEVENT  
   ***********************************/ 
  int ret = 0;
  DIALOG *D;
  KBEVENT *kbe;
  D = (DIALOG *)Tmp;
  kbe = (KBEVENT *)tmp;
  if(kbe->event ==1) {
    if(kbe->button ==1) {
    }
  }
  return ret;
}
int msgboxResizeCallBack(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  int ret = 0;
  int xres,yres,dx,dy;
  DIALOG *D;
  D = (DIALOG *)Tmp;
  kgGetWindowSize(D,&xres,&yres);
  dx = xres - D->xl;
  dy = yres - D->yl;
  /* extra code */
  D->xl= xres;
  D->yl= yres;
  kgRedrawDialog(D);
  return ret;
}
int msgboxWaitCallBack(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    Called while waiting for event  
    return value 1 will close the the UI  
   ***********************************/ 
  int ret = 0;
  return ret;
}
