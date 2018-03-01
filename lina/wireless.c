//#include _BSD_SOURCE
#include "kulina.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "netlist.h"
#define StrongLevel -35  // ie 100 % anything higher than -35 is 100%o
#define WeakLevel   -95 // ie 1&
Dlink *Scanlist=NULL;
char Wdev[100],Edev[100];
char EnvFile[200];
int SYSTEMD=0;
int DBUS=0;
int ID=-1,OK;
extern int Connected;
char Curssid[100];
int Runwiredia(void *arg);
#define WAIT(pid) {\
  pid_t w;\
  int status;\
  do { \
                   w = waitpid(pid, &status, WUNTRACED | WCONTINUED); \
                   if (w == -1) { \
                       perror("waitpid"); \
                       exit(EXIT_FAILURE); \
                   }\
               } while (!WIFEXITED(status) && !WIFSIGNALED(status));\
}
int CheckProcess(char *);
int CheckString(char *s1,char *s2);
int SearchString(char *s1,char *s2);
int GetLine(int pip0,char *buff);
int WaitForProcess(int pip0,int pip1,int Pid);
int runjob(char *job,int (*ProcessOut)(int,int,int));
int changejob(char *job);
int ProcessWconf(int pip0,int pip1,int Pid) {
     char buff[1000],ssid[200],psk[200],flname[200];
     char *pt;
     FILE *fp=NULL;
     int ch,i=0,j,k,KILL=0;
     mkdir("/usr/share/config",0755);
     mkdir("/usr/share/config/lina",0700);
     mkdir("/usr/share/config/lina/Wireless",0700);

     while((ch=GetLine(pip0,buff)) ) {
         if(ch< 0) {continue;}
         i=0;
         while (buff[i]==' ') i++;
         if(buff[i]=='#') continue;
         if((buff[i]< ' ')&&(buff[i]!='\t')) continue;
         if(SearchString(buff+i,(char *)"network")>=0) {
           do {
             if(ch< 0) {continue;}
             i=0;
             while (buff[i]==' ') i++;
             if(buff[i]=='#') continue;
             if((buff[i]< ' ')&&(buff[i]!='\t')) continue;
             pt =buff+i;
             if((j=SearchString(pt,(char *)"ssid"))>=0) {
               pt =pt+j;
               if((j=SearchString(pt,(char *)"\""))>=0) {
                  pt = pt+j+1;
                  k = SearchString(pt,(char *)"\"");
                  pt[k]='\0';
                  strcpy(ssid,pt);
                  sprintf(flname,"/usr/share/config/lina/Wireless/%-s",ssid);
                  fp = fopen(flname,"w");
                  i=k+1;
                  pt=pt+k+1;
               }
             }
             if((j=SearchString(pt,(char *)"psk"))>=0) {
               pt = pt+j;
               if((j=SearchString(pt,(char *)"\""))>=0) {
                  pt = pt+j+1;
                  k = SearchString(pt,(char *)"\"");
                  pt[k]='\0';
                  strcpy(psk,pt);
                  if(fp!= NULL) {
                    fprintf(fp,"%-s\n",psk);
                    fclose(fp);
                    fp=NULL;
                  }
                  i=k+1;
                  pt=pt+k+1;
               }
             }
             if((j=SearchString(pt,(char *)"}"))>=0) {
               if(fp != NULL) fclose(fp);
               fp=NULL;
               break;
             }
           } while((ch=GetLine(pip0,buff)) );
         }
     }
//     if(!KILL) killpg(Sid,SIGKILL);
     return 1;
}
int ProcessEnv(int pip0,int pip1,int Pid) {
     char buff[1000],flname[200];
     char *pt;
     int ch,i=0,j,ret=0;

     while((ch=GetLine(pip0,buff)) ) {
         if(ch< 0) {continue;}
         i=0;
         while (buff[i]==' ') i++;
         if(buff[i]=='#') continue;
         if((buff[i]< ' ')&&(buff[i]!='\t')) continue;
         if(SearchString(buff+i,(char *)"EnvironmentFile")>=0) {
             pt =buff+i;
             if((j=SearchString(pt,(char *)"-"))>=0) {
               pt =pt+j+1;
               sscanf(pt,"%s",EnvFile);
               printf("EnvFile = %s\n",EnvFile);
               ret=1;
               break;
             }
         }
     }
     return ret;
}
int ProcessStatus(int pip0,int pip1,int Pid) {
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
         if((j=SearchString(buff,(char *)"ssid"))>=0) {
           if((j=SearchString(buff,(char *)"="))>=0) {
             j++;
             i=j;
             while(buff[i]>=' ') i++;
             buff[i]='\0';
             strcpy(Curssid,buff+j);
             OK=1;
           }
         }
     }
     return OK;
}
int ProcessState(int pip0,int pip1,int Pid) {
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
         if((j=SearchString(buff,(char *)"wpa_state=COMPLETED"))>=0) {
             OK=1;
         }
     }
     return OK;
}
int GetState(void) {
    OK=0;
    runjob("wpa_cli status",ProcessState);
    if(OK==0) { sleep(1);runjob("wpa_cli status",ProcessState);}
    if(OK==0) { sleep(1);runjob("wpa_cli status",ProcessState);}
    if(OK==0) { sleep(1);runjob("wpa_cli status",ProcessState);}
    return OK;
}
int ProcessCli(int pip0,int pip1,int Pid) {
     char buff[1000];
     int ch,i=0,j;
     OK=0;
     while((ch=GetLine(pip0,buff)) ) {
         printf("ProcessCli: %s",buff);
         if(ch< 0) {continue;}
         if((j=SearchString(buff,(char *)"Failed to connect"))>=0) {
           OK=0;
           break;
         }
         if((j=SearchString(buff,(char *)"wpa_state"))>=0) {
           printf("CLI OKAY\n");
           OK=1;
           break;
         }
     }
     return OK;
}
int ProcessScan(int pip0,int pip1,int Pid) {
     char buff[1000],bssid[100],ssid[200],psk[200],flname[200];
     char *pt;
     FILE *fp=NULL;
     int ch,i=0,j,k,sig,NOTOK;
     NETLIST *nt,*tnt;
//   Skipa line
     while((ch=GetLine(pip0,buff)) ) {
         if(ch< 0) {continue;}
         else break;
     }
     while((ch=GetLine(pip0,buff)) ) {
         if(ch< 0) {continue;}
         else break;
     }
   
     while((ch=GetLine(pip0,buff)) ) {
         if(ch< 0) {continue;}
         if(Scanlist==NULL) Scanlist=Dopen();
         nt = (NETLIST *)malloc(sizeof(NETLIST));
         pt =  buff;
         if(SearchString(buff,"WPA-PSK-TKIP")>= 0 ) {
          nt->proto=1;nt->group=1;
         }
         else {
           if(SearchString(buff,"WPA-PSK-CCMP+TKIP") >=0 ) {
             nt->proto=1;nt->group=1;
           }
           else {
             if(SearchString(buff,"WPA-PSK-CCMP")  >=0) {
               nt->proto=1;nt->group=2;
             }
             else  {
               if(SearchString(buff,"WPA2-PSK-CCMP")  >=0) {
                nt->proto=2;nt->group=2;
               }
             }
           }
         }
         i=0;  while (pt[i]!='\t') i++;pt[i]='\0';
         strcpy(nt->bssid,pt);
         pt = pt+i+1;
         i=0;  while (pt[i]!='\t') i++;pt[i]='\0';
         pt = pt+i+1;
         i=0;  while (pt[i]!='\t') i++;pt[i]='\0';
         sscanf(pt,"%d",&(nt->sig));
         sig = nt->sig;
         if(sig >= StrongLevel) nt->sig=100;
         if(sig <= WeakLevel) nt->sig=1;
         else {
           sig = -95 - sig;
           sig = (-sig*100)/60.0;
           sig = ((sig/25.0)+0.5);
           nt->sig = sig*25;
         }
         pt = pt+i+1;
         i=0;  while (pt[i]!='\t') i++;pt[i]='\0';
         pt = pt+i+1;
         i=0;  while ((pt[i]!='\t')&&(pt[i]>=' ')) i++;pt[i]='\0';
         strcpy(nt->ssid,pt);
         Dadd(Scanlist,nt);
     }
     NOTOK=1;
     while(NOTOK) {
       NOTOK=0;
       Resetlink(Scanlist);
       i=0;
       while((nt=(NETLIST *)Getrecord(Scanlist))!= NULL) {
         i++;
         if(nt->ssid[0]=='\\') { NOTOK=1;break;}
       }
       if(NOTOK) {
         Resetlink(Scanlist);
         Dposition(Scanlist,i);
         tnt = (NETLIST *)Dpick(Scanlist);
         Resetlink(Scanlist);
         while((nt=(NETLIST *)Getrecord(Scanlist))!= NULL) {
           if(strcmp(nt->bssid,tnt->bssid) ==0) { nt->sig=-100;}
         }
         free(tnt);
       }
     }
     return 1;
}
int ProcessAdd(int pip0,int pip1,int Pid) {
     char buff[1000];
     char *pt;
     FILE *fp=NULL;
     int ch,i=0,j,k,id=-1;
//   Skipa line
     while((ch=GetLine(pip0,buff)) ) {
         if(ch< 0) {continue;}
         else break;
     }
     while((ch=GetLine(pip0,buff)) ) {
         if(ch< 0) {continue;}
         sscanf(buff,"%d",&id);
     }
//     if(!KILL) killpg(Sid,SIGKILL);
//     printf("Returning\n");
     ID=id;
     return id;
}
int ProcessSet(int pip0,int pip1,int Pid) {
     char buff[1000],status[100];
     char *pt;
     FILE *fp=NULL;
     int ch,i=0,j,k;
     OK=0;
//   Skipa line
     while((ch=GetLine(pip0,buff)) ) {
         if(ch< 0) {continue;}
         else break;
     }
     while((ch=GetLine(pip0,buff)) ) {
         if(ch< 0) {continue;}
         sscanf(buff,"%s",status);
//         printf("Set Status= %s\n",status);
         if(strcmp(status,"OK")==0) OK=1;
     }
//     if(!KILL) killpg(Sid,SIGKILL);
//     printf("Returning\n");
     return OK;
}
int CleanNetworks(void) {
  char buff[100];
  int id;
  runjob("wpa_cli add_network",ProcessAdd);
  for(id=0;id<=ID;id++) {
    sprintf(buff,"wpa_cli remove_network %d",id);
    runjob(buff,WaitForProcess);
  }
}
int addnetwork(NETLIST *nt) {
  char buff[500];
  runjob("wpa_cli add_network",ProcessAdd);
  if(ID<0) ID=0;
  nt->id=ID;
//  sleep(1);
  nt->sec =0;
  sprintf(buff,"wpa_cli set_network %d ssid \\\"%-s\"\\",ID,nt->ssid);
  runjob(buff,ProcessSet);
  if(!OK) {sleep(3); runjob(buff,ProcessSet);}
  if(nt->psk[0]!='\0') {
      nt->sec =1;
      sprintf(buff,"wpa_cli set_network %d psk \\\"%-s\"\\",ID,nt->psk);
      runjob(buff,ProcessSet);
      if(nt->proto==2) {
        sprintf(buff,"wpa_cli set_network %d proto  WPA2",ID);
        runjob(buff,ProcessSet);
      }
      if(nt->group==2) {
        sprintf(buff,"wpa_cli set_network %d group  CCMP",ID);
        runjob(buff,ProcessSet);
      }
      if(!OK) {sleep(3); runjob(buff,ProcessSet);}
  }
  if(nt->sig <0 ) {
      sprintf(buff,"wpa_cli set_network %d scan_ssid 1",ID,nt->psk);
      runjob(buff,ProcessSet);
      if(!OK) {sleep(3); runjob(buff,ProcessSet);}
  }
  return OK;
}
int AddNetworks_o(void) {
  char buff[100];
  FILE *fp;
  char **Onets,*ssid;
  int i;
  Dlink *Dlist=NULL;
  NETLIST *nt;
  if(Scanlist!=NULL) {
    Dempty(Scanlist);
    Scanlist=NULL;
  }
  CleanNetworks();
  runjob("wpa_cli scan",WaitForProcess);
  for(i=0;i<4;i++){
    sleep(i+1);
    runjob("wpa_cli scan_results",ProcessScan);
    if(Scanlist!=NULL)break;
  }
  if(Scanlist!=NULL) {
//  printf("Scanlist != NULL\n");
    Resetlink(Scanlist);
    i=0;
    while ( (nt=(NETLIST *)Getrecord(Scanlist)) != NULL) {
      runjob("wpa_cli add_network",ProcessAdd);
      if(ID<0) ID=i;
      nt->id=ID;
      i++;
//    sleep(1);
      sprintf(buff,"wpa_cli set_network %d ssid \\\"%-s\"\\",ID,nt->ssid);
      runjob(buff,ProcessSet);
      if(!OK) {sleep(3); runjob(buff,ProcessSet);}
      sprintf(buff,"/usr/share/config/lina/Wireless/%-s",nt->ssid);
      nt->sec=-1;
      fp = fopen(buff,"r");
      nt->psk[0]='\0';
      if(fp != NULL) {
        nt->sec=0;
        fscanf(fp,"%s",nt->psk);
        fclose(fp);
        if(nt->psk[0]!='\0') {
          nt->sec=1;
          sprintf(buff,"wpa_cli set_network %d psk \\\"%-s\"\\",ID,nt->psk);
          runjob(buff,ProcessSet);
          if(!OK) {sleep(3); runjob(buff,ProcessSet);}
        }
      }
    }
  }
  return 1;
}
int AddNetworks(void) {
  char buff[100];
  FILE *fp;
  char **Onets,*ssid;
  int i;
  Dlink *Dlist=NULL;
  NETLIST *nt;
  if(Scanlist!=NULL) {
    Dempty(Scanlist);
    Scanlist=NULL;
  }
  CleanNetworks();
  runjob("wpa_cli scan",WaitForProcess);
  for(i=0;i<4;i++){
    sleep(i+1);
    runjob("wpa_cli scan_results",ProcessScan);
    if(Scanlist!=NULL)break;
  }
  if(Scanlist!=NULL) {
//  printf("Scanlist != NULL\n");
    Resetlink(Scanlist);
    i=0;
    while ( (nt=(NETLIST *)Getrecord(Scanlist)) != NULL) {
      i++;
//    sleep(1);
      sprintf(buff,"/usr/share/config/lina/Wireless/%-s",nt->ssid);
      nt->sec=-1;
      fp = fopen(buff,"r");
      nt->psk[0]='\0';
      if(fp != NULL) {
        nt->sec=0;
        fscanf(fp,"%s",nt->psk);
        if(fscanf(fp,"%d%d",&(nt->proto),&(nt->group))<=0) {
                 nt->proto=1;nt->group=1;
        }
        fclose(fp);
        if(nt->psk[0]!='\0') {
          nt->sec=1;
        }
      }
    }
  }
  return 1;
}
int AddOldNetworks_o(void) {
  char buff[100];
  FILE *fp;
  char **Onets,*ssid;
  int i;
  Dlink *Dlist=NULL;
  NETLIST *nt;
  Onets= (char **)kgFileMenu("/usr/share/config/lina/Wireless","*");
  if(Onets!= NULL) {
    int Dup=0;
    FILE *fp;
    if(Scanlist==NULL) Scanlist= Dopen();
    Dlist=Dcopy(Scanlist);
    i=0;
    while ((ssid=Onets[i])!= NULL) {
       i++;
       Resetlink(Dlist);
       Dup=0;
       while( (nt=(NETLIST *)Getrecord(Dlist))!= NULL) {
         if( strcmp(nt->ssid,ssid)==0) {Dup=1;break;}
       }
       if(Dup) continue;
       sprintf(buff,"/usr/share/config/lina/Wireless/%-s",ssid);
       fp = fopen(buff,"r");
       if(fp==NULL) continue;
       nt = (NETLIST *)malloc(sizeof(NETLIST));
       nt->sig=-100;
       nt->sec=0;
       strcpy(nt->ssid,ssid);
       nt->psk[0]='\0';
       fscanf(fp,"%s",nt->psk);
       if(nt->psk[0]!='\0') nt->sec=1;
       fclose(fp);
       Dappend(Scanlist,nt);
       runjob("wpa_cli add_network",ProcessAdd);
       if(ID<0) ID=0;
       nt->id=ID;
//       sleep(1);
       sprintf(buff,"wpa_cli set_network %d ssid \\\"%-s\"\\",ID,nt->ssid);
       runjob(buff,ProcessSet);
       if(!OK) {sleep(3); runjob(buff,ProcessSet);}
       if(nt->psk[0]!='\0') {
          sprintf(buff,"wpa_cli set_network %d psk \\\"%-s\"\\",ID,nt->psk);
          runjob(buff,ProcessSet);
          if(!OK) {sleep(3); runjob(buff,ProcessSet);}
       }
    }
    Dfree(Dlist);
    kgFreeDouble((void **)Onets);
  }
  return 1;
}
int AddOldNetworks(void) {
  char buff[100];
  FILE *fp;
  char **Onets,*ssid;
  int i;
  Dlink *Dlist=NULL;
  NETLIST *nt;
  Onets= (char **)kgFileMenu("/usr/share/config/lina/Wireless","*");
  if(Onets!= NULL) {
    int Dup=0;
    FILE *fp;
    if(Scanlist==NULL) Scanlist= Dopen();
    Dlist=Dcopy(Scanlist);
    i=0;
    while ((ssid=Onets[i])!= NULL) {
       i++;
       Resetlink(Dlist);
       Dup=0;
       while( (nt=(NETLIST *)Getrecord(Dlist))!= NULL) {
         if( strcmp(nt->ssid,ssid)==0) {Dup=1;break;}
       }
       if(Dup) continue;
       sprintf(buff,"/usr/share/config/lina/Wireless/%-s",ssid);
       fp = fopen(buff,"r");
       if(fp==NULL) continue;
       nt = (NETLIST *)malloc(sizeof(NETLIST));
       nt->sig=-100;
       nt->sec=0;
       strcpy(nt->ssid,ssid);
       nt->psk[0]='\0';
       fscanf(fp,"%s",nt->psk);
       if(fscanf(fp,"%d%d",&(nt->proto),&(nt->group))<=0 ) {
         nt->proto=1;nt->group=1;
       }
       if(nt->psk[0]!='\0') nt->sec=1;
       fclose(fp);
       Dappend(Scanlist,nt);
    }
    Dfree(Dlist);
    kgFreeDouble((void **)Onets);
  }
  return 1;
}
int ConnectNetwork(NETLIST *nt) {
    char buff[100];
    int id;
#if 1
    CleanNetworks();
    if(nt->sig < 0) {
      sprintf(buff,"wpa_cli flush");
      runjob(buff,ProcessSet);
    }
    addnetwork(nt);
#endif
    id =nt->id;
//    printf("Trying to Connect: %d\n",ID);
    sprintf(buff,"wpa_cli enable_network %-d",id);
    runjob(buff,ProcessSet);
    if(!OK) {sleep(3); runjob(buff,ProcessSet);}
    sprintf(buff,"wpa_cli select_network %-d",id);
    runjob(buff,ProcessSet);
//    printf("ID= %d %s\n",ID,buff);
    sprintf(buff,"wpa_cli reauthenticate");
    runjob(buff,ProcessSet);
    if(!OK) {sleep(3); runjob(buff,ProcessSet);}
    return OK;
}
int MakeConfigFile(void) {
  int ret=0;
  FILE *fp=NULL;
  fp = fopen("/tmp/wpa.conf","w");
  if(fp != NULL) {
    fprintf(fp,"update_config=1\n");
    fprintf(fp,"ctrl_interface=/var/run/wpa_supplicant\n");
    fprintf(fp,"eapol_version=1\n");
    fprintf(fp,"ap_scan=1\n");
    fprintf(fp,"fast_reauth=1\n");
    fclose(fp);
    ret=1;
  }
  return ret;
}
int runbin(char *job){
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
int GetWdev(void) {
  char **devs,**files,buff[200];
  int i,j,OK=0;
  devs=kgFolderMenu("/sys/class/net");
  Wdev[0]='\0';
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
  printf("Wireless Device : %s\n",Wdev);
  return OK;
}
int WirelessStatus(void) {
  char buff[200];
  int n,pp2,OK=0;
  Connected=0;
  if(GetWdev()==0) return 0;
  sprintf(buff,"ip addr list dev %-s",Wdev);
  pp2=runbin(buff);
  if((n=read(pp2,buff,100)) ==100) {
    if( SearchString(buff,"DOWN")<=0) {
      if( SearchString(buff,"UP")>=0) {
        while((n=read(pp2,buff,100)) ==100) {
          if( SearchString(buff,"inet ")>=0) {
            OK=1;
            Connected=1;
            break;
          }
        }
      }
    }
  }
  if(Connected) {
    runjob("wpa_cli status",ProcessStatus);
  }
 
  return OK;

}
int CheckCliConnection(void){
   OK=0;
   runjob("wpa_cli status",ProcessCli);
   return OK;
}
int CheckSystemd(void) {
  FILE *fp=NULL;
  char buff[500];
  int ret=0;
  EnvFile[0]='\0';
  if(CheckProcess("systemd")) {
    ret =1;
    SYSTEMD=1;
#if 1
    printf("systemd is running...\n");
    runjob("cat /lib/systemd/system/wpa_supplicant.service",ProcessEnv);
    if(EnvFile[0]=='\0')
       runjob("cat /usr/lib/systemd/system/wpa_supplicant.service",ProcessEnv);
//    fp = fopen("/etc/sysconfig/wpa_supplicant","w");
    if(EnvFile[0]!='\0')fp = fopen(EnvFile,"w");
    if(fp != NULL) {
      fprintf(fp,"INTERFACES=\"-i%-s\"\n",Wdev);
      fprintf(fp,"DRIVERS=\" \"\n");
      fprintf(fp,"OTHER_ARGS=\" -s -B\"\n");
      fclose(fp);
      printf("Restarting wpa_supplicant\n");
      runjob("systemctl restart wpa_supplicant",WaitForProcess);
      sleep(1);
      MakeConfigFile();
      sprintf(buff,"wpa_supplicant  -i%-s -u  -c/tmp/wpa.conf -B",Wdev);
      printf("%s\n",buff);
      runjob(buff,WaitForProcess);
      sleep(1);
      CheckProcessCommand("wpa_supplicant");
      if(EnvFile[0]!='\0') fp = fopen(EnvFile,"w");
      fprintf(fp,"INTERFACES=\"-i%-s\"\n",Wdev);
      fprintf(fp,"DRIVERS=\" \"\n");
      fprintf(fp,"OTHER_ARGS=\" \"\n");
      fclose(fp);
    }
#else
    DBUS =1;
    runjob("systemctl stop dbus",WaitForProcess);
#endif
  }
  else printf("Systemd is not running...\n");
  return ret;
}
int ResetSystemd(void) {
  FILE *fp=NULL;
  int ret=0;
  char buff[500];
  EnvFile[0]='\0';
  if(CheckProcess("systemd")) {
    ret =1;
    SYSTEMD=1;
    printf("Reset Systemd: systemd is running...\n");
    runjob("cat /lib/systemd/system/wpa_supplicant.service",ProcessEnv);
    if(EnvFile[0]=='\0')
       runjob("cat /usr/lib/systemd/system/wpa_supplicant.service",ProcessEnv);
    if(EnvFile[0]!='\0')fp = fopen(EnvFile,"w");
    if(fp != NULL) {
      fprintf(fp,"INTERFACES=\"-i%-s\"\n",Wdev);
      fprintf(fp,"DRIVERS=\" \"\n");
      fprintf(fp,"OTHER_ARGS=\" -s -B\"\n");
      fclose(fp);
      printf("Restarting wpa_supplicant\n");
      runjob("systemctl restart wpa_supplicant",WaitForProcess);
      printf("ResetSystemd: MakeConfigFile\n");
      sleep(1);
      MakeConfigFile();
      sprintf(buff,"wpa_supplicant  -i%-s -u  -c/tmp/wpa.conf -B",Wdev);
      printf("%s\n",buff);
      runjob(buff,WaitForProcess);
      sleep(1);
      if(!CheckProcess("wpa_supplicant")) {
        printf("Retrying: %s\n",buff);
        runjob(buff,WaitForProcess);
      }
      else {
        printf("wpa_supplicant running\n");
        CheckProcessCommand("wpa_supplicant");
      }
#if 1
      if(EnvFile[0]!='\0') fp = fopen(EnvFile,"w");
      fprintf(fp,"INTERFACES=\"-i%-s\"\n",Wdev);
      fprintf(fp,"DRIVERS=\" \"\n");
      fprintf(fp,"OTHER_ARGS=\" \"\n");
      fclose(fp);
#endif
      sleep(1);
    }
  }
  else printf("Systemd is not running...\n");
  return ret;
}
 
int  Wireless(int key,void *Parent) {
  FILE *fp;
  int n,ret =0,OK=0; 
  char buff[500];
  static int pid=0;
//  FILE *pp=NULL;
  static int pp=0,pp2=0;
  
  switch(key) {
    case 1: 
      OK=0;
      ret = OK;
      if(!GetWdev()) break;
      fp = fopen("/tmp/dhclient.conf","w");
      if(fp==NULL) break;
      runjob("ip route del default",WaitForProcess);
      fprintf(fp,"interface \"%-s\"{\n",Wdev);
      fprintf(fp,"prepend domain-name-servers 127.0.0.1;\n");
      fprintf(fp,"request subnet-mask, broadcast-address, time-offset, routers,\n");
      fprintf(fp,"       domain-name, domain-name-servers, host-name;\n");
//      fprintf(fp,"require subnet-mask, domain-name-servers;\n");
      fprintf(fp,"require subnet-mask;\n");
      fprintf(fp,"}\n");
      fclose(fp);
      remove("/var/lib/dhclient/dhclient.leases");
      if((CheckProcess("wpa_supplicant"))) {
         if (!CheckCliConnection()){
           if(!CheckSystemd()){
             MakeConfigFile();
             sprintf(buff,"wpa_supplicant -i%-s -u -c/tmp/wpa.conf -B",Wdev);
             runjob(buff,WaitForProcess);
             sleep(1);
           }
           printf ("Systemd is Running...\n");
#if 1
           if (!CheckCliConnection()) {
             printf("No cli connection Reset Systemd\n");
             ResetSystemd() ;
             if (!CheckCliConnection()) {
               printf("Reset Systemd:No cli connection\n");
             }
           }
#endif
         }
         if (!CheckCliConnection()) {printf("No cli connection\n");break;}
      }
      else {
        printf("wpa_supplicant not running\n");
        MakeConfigFile();
        sprintf(buff,"wpa_supplicant -i%-s -u -c/tmp/wpa.conf -B",Wdev);
        runjob(buff,WaitForProcess);
      }
      runjob("cat /etc/wpa_supplicant.conf",ProcessWconf);
      sprintf(buff,"ip link set  %s up",Wdev);
      runjob(buff,WaitForProcess);
#if 0
      if(Scanlist == NULL){
        AddNetworks();
        AddOldNetworks();
      }
#endif
      Connected=0;
      ret=Runwiredia(Parent);
#if 0
      sprintf(buff,"dhclient -cf /tmp/dhclient.conf -v %-s",Wdev);
      pp2=runbin(buff);
      while((n=read(pp2,buff,100)) ==100) {
//           Dprintf(Tmp,0,buff);
//           kgUpdateOn(Tmp);
           buff[strlen("bound to")]='\0';
           if(strcmp(buff,"bound to")==0) OK=1;
      }
//      Dprintf(Tmp,0," ");
      if(OK) {
//              Dprintf(Tmp,0,"!z32!c06Connected Wireless");
      }
//      else Dprintf(Tmp,0,"!z32!c03Failed Wireless");
//      kgUpdateOn(Tmp);
      ret= OK;
#endif
      break;
    case 0: 
      Connected=1;
      if(!GetWdev()) break;
      ret = Runwiredia(Parent);
#if 0
      sprintf(buff,"ip link set %-s down",Wdev);
//      pp2=runbin("ip link set wlan0 down");
      pp2=runbin(buff);
      while((n=read(pp2,buff,100)) ==100) {
//           Dprintf(Tmp,0,buff);
//           kgUpdateOn(Tmp);
      }
//      Dprintf(Tmp,0," ");
//      Dprintf(Tmp,0,"!z32!c04Disonnected Wireless");
//      kgUpdateOn(Tmp);
#endif
        ret = (ret+1)%2;
      break;
      default:
      break;
  }
  return ret;
}
