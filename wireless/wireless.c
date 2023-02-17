//#include _BSD_SOURCE
#include "wireless.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#define StrongLevel -35  // ie 100 % anything higher than -35 is 100%o
#define WeakLevel   -95 // ie 1&
static Dlink *Scanlist=NULL;
char *Wdev=NULL,Edev[100];
static char EnvFile[200];
static int SYSTEMD=0;
static int DBUS=0;
static int ID=-1,OK;
static int Connected=0;
static char Curssid[100];
static int MakeDhclient(void);
int InitWpa(void);
static int WC=0;
static char Bcaddr[50];
static char *IPADDR=NULL;
extern char *PSK;
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
static int CheckProcess(char *);
int SearchString(char *s1,char *s2);
int GetLine(int pip0,char *buff);
int WaitForProcess(int pip0,int pip1,int Pid);
int runjob(char *job,int (*ProcessOut)(int,int,int));
int changejob(char *job);
int GetWdev(void);
char * WirelessStatus(void);
static int CheckCliConnection(void);
static int runbin(char *job);

static int CheckProcess(char *procname) {
   char buff[500];
   int Id=0,Okay=0,count=0,ln;
   char tty[50],dummy[50],program[100];
   char *pt;
   FILE *pp;
     ln = strlen(procname);
     pp = popen("ps -e","r");
     fgets(buff,499,pp);
     while( fgets(buff,499,pp) != NULL) {
        sscanf(buff,"%d%s%s%s",&Id,tty,dummy,program);
#if 0
        program[ln]='\0';
        if((strcmp(procname,program)==0)) {
#else
        pt = strstr(program,procname);
        if(pt != NULL) {
#endif
             Okay=Id;
             break;
        }
     }
     pclose(pp);
   return Okay;
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
         printf("  %s",buff);
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
static int ProcessCli(int pip0,int pip1,int Pid) {
     char buff[1000];
     int ch,i=0,j;
     OK=0;
     while((ch=GetLine(pip0,buff)) ) {
         if(ch< 0) {continue;}
         if((j=SearchString(buff,(char *)"Failed to connect"))>=0) {
           OK=0;
           break;
         }
         if((j=SearchString(buff,(char *)"wpa_state"))>=0) {
           OK=1;
           break;
         }
     }
     return OK;
}
static int ProcessScan(int pip0,int pip1,int Pid) {
     char buff[1000],bssid[100],ssid[200],psk[200],flname[200];
     char *pt;
     FILE *fp=NULL;
     int ch,i=0,j,k,sig,NOTOK;
     NETLIST *nt,*tnt;

     k=0;
     while((ch=GetLine(pip0,buff)) ) {
         if(ch< 0) {continue;}
	 k++;
//         printf(" Scan: %s\n",buff);
	 if(k<=2) continue;
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
#if 0
         if(sig >= StrongLevel) nt->sig=100;
         if(sig <= WeakLevel) nt->sig=1;
         else {
           sig = -95 - sig;
           sig = (-sig*100)/60.0;
           sig = ((sig/25.0)+0.5);
           nt->sig = sig*25;
         }
#else
           sig = -90 - sig;
           sig = (-sig*100)/60.0;
           nt->sig = sig;
#endif
         pt = pt+i+1;
         i=0;  while (pt[i]!='\t') i++;pt[i]='\0';
         pt = pt+i+1;
         i=0;  while ((pt[i]!='\t')&&(pt[i]>=' ')) i++;pt[i]='\0';
         strcpy(nt->ssid,pt);
         Dadd(Scanlist,nt);
     }
     if(Scanlist==NULL) return 0;
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
static int ProcessAdd(int pip0,int pip1,int Pid) {
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
     ID=id;
     return id;
}
static int ProcessSet(int pip0,int pip1,int Pid) {
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
static int CleanNetworks(void) {
  char buff[100];
  int id;
  runjob("wpa_cli add_network",ProcessAdd);
  for(id=0;id<=ID;id++) {
    sprintf(buff,"wpa_cli remove_network %d",id);
    runjob(buff,WaitForProcess);
  }
}
static int addnetwork(NETLIST *nt) {
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
static int AddNetworks(void) {
  char buff[1000];
  FILE *fp;
  char **Onets,*ssid;
  int i;
  Dlink *Dlist=NULL;
  NETLIST *nt;
  if(Scanlist!=NULL) {
    Dempty(Scanlist);
    Scanlist=NULL;
  }
//  CleanNetworks();
  sprintf(buff,"ip link set %-s up",Wdev);
  runjob(buff,NULL);
  runjob("wpa_cli scan",WaitForProcess);
  for(i=0;i<4;i++){
    sleep(i+1);
    runjob("wpa_cli scan_results",ProcessScan);
    if(Scanlist!=NULL)break;
    printf("Scanlist NULL\n");
    
  }
  if(Scanlist!=NULL) {
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
static int AddOldNetworks(void) {
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
static int ConnectNetwork(NETLIST *nt) {
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
    printf("Trying to Connect: %d\n",ID);
    sprintf(buff,"wpa_cli enable_network %-d",id);
    runjob(buff,ProcessSet);
    if(!OK) {sleep(3); runjob(buff,ProcessSet);}
    sprintf(buff,"wpa_cli select_network %-d",id);
    runjob(buff,ProcessSet);
    sprintf(buff,"wpa_cli reauthenticate");
    runjob(buff,ProcessSet);
    if(!OK) {sleep(3); runjob(buff,ProcessSet);}
    return OK;
}
static int MakeConfigFile(void) {
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
static int GetSSIDid(Dlink *S,char *ssid) {
  int id=0;
  NETLIST *nt;
  Resetlink(S);
  while((nt=(NETLIST *)Getrecord(S))!= NULL) {
    id++;
    if(strcmp(ssid,nt->ssid)==0) return id;
  }
  return 0;

}
int  ScanSSID() {
  NETLIST *nt;
  AddNetworks();
//  AddOldNetworks();
  Resetlink(Scanlist);
  while( (nt=Getrecord(Scanlist))!=NULL) {
       printf("%s %d\n",nt->ssid,nt->sig);
  }
}
int  DisconnectSSID(void) {
  char buff[200];
  if(!GetWdev()) return 0;
  sprintf(buff,"ip link set %s down",Wdev);
  system(buff);
  return 1;
}
 
static int MakeDhclient(void) {
   FILE *fp;
   char buff[500];
   fp = fopen("/tmp/dhclient.conf","w");
   if(fp==NULL) return 0;
   runjob("ip route del default",WaitForProcess);
   fprintf(fp,"interface \"%-s\"{\n",Wdev);
   fprintf(fp,"prepend domain-name-servers 127.0.0.1;\n");
   fprintf(fp,"request subnet-mask, broadcast-address, time-offset, routers,\n");
   fprintf(fp,"       domain-name, domain-name-servers, host-name;\n");
      fprintf(fp,"require subnet-mask, domain-name-servers;\n");
  fprintf(fp,"}\n");
  fclose(fp);
  remove("/var/lib/dhclient/dhclient.leases");
  if((CheckProcess("wpa_supplicant"))) {
    if (!CheckCliConnection()){
      sprintf(buff,"wpa_supplicant -i%-s -u -c/tmp/wpa.conf -B",Wdev);
      runjob(buff,WaitForProcess);
      sleep(1);
    }
  }
  else {
      sprintf(buff,"wpa_supplicant -i%-s -u -c/tmp/wpa.conf -B",Wdev);
      runjob(buff,WaitForProcess);
      sleep(1);
  }
//  while (!CheckCliConnection());
  return 1;
}
int  DeleteSSID(char *ssid) {
  char buff[500];
  NETLIST *nt;
  int n,ret=1,OK=0,id; 
  int pp2;
  int butno=2;
  int RetVal=0;
  char *ipaddr,servip[50];
  unsigned char *pt;
  FILE *fp;

  if(Scanlist==NULL) {
        AddNetworks();
        AddOldNetworks();
  }
  if((Scanlist==NULL)||(Dcount(Scanlist)==0)) {
        RetVal=0;
        fprintf(stderr,"No Wireless Network\n");
        return 0;
  }
  id = GetSSIDid(Scanlist,ssid);
 if(id==0) {
        fprintf(stderr,"Unknown SSID: %s\n",ssid);
        return 0;
 }
 Resetlink(Scanlist);
 Dposition(Scanlist,id);
 nt =(NETLIST *) Getrecord(Scanlist);
 sprintf(buff,"/usr/share/config/lina/Wireless/%-s",nt->ssid);
 remove(buff);
 Dposition(Scanlist,id);
 Ddelete(Scanlist);
 fprintf(stderr,"Removed %s\n",ssid);
}
static int CompareSignal(void *s1,void *s2) {
	int ret=0;
	ret = strcmp((char *)s1,(char *)s2);
	return ret;
}
char *GetPreferredSSID(void) {
    NETLIST *pt;
    char *spt=NULL;
    char **ssids=NULL;
    int ret=0;
    int i=0;
       
    ret = InitWpa();
    if(Scanlist==NULL) {
        AddNetworks();
    } 
    if(Dcount(Scanlist)==0 ) return NULL;
    Dsort(Scanlist,CompareSignal);
    Resetlink(Scanlist);
    ssids=kgFileMenu("/usr/share/config/lina/Wireless","*");
    if(ssids==NULL) return NULL;
    while( (pt = (NETLIST *) Getrecord(Scanlist))!= NULL) {
	    printf ("%s %d\n",pt->ssid,pt->sig);
	    i=0;
	    while(ssids[i]!= NULL) {
		    if(strcmp(pt->ssid,ssids[i])==0) {
			    spt=(char *) malloc(strlen(pt->ssid)+1);
			    strcpy(spt,pt->ssid);
			    break;
		    }
		    i++;
	    }
	    if(spt != NULL) break;
    }
    return spt;
}
int  ConnectSSID(char *ssid) {
  char buff[500];
  NETLIST *nt;
  int n,ret=1,OK=0,id; 
  int pp2;
  int butno=2;
  int RetVal=0;
  char *ipaddr,servip[50];
  unsigned char *pt;
  FILE *fp;
  ret = InitWpa();
  ipaddr = WirelessStatus();
//  if (ret==0) {
  {
    servip[0]='\0';
    Bcaddr[0]='\0';
//    ipaddr = IPADDR;
    if(ipaddr != NULL) {
      if(strcmp(Curssid,ssid)==0){
 	 fprintf(stderr,"%s Connected\n",ssid);
	 return 1;
      }	
      sprintf(buff,"ip addr del %s dev %s",ipaddr,Wdev);
      printf("%s \n",buff);
      system(buff);
      sprintf(buff,"ip route del default",ipaddr,Wdev);
      printf("%s \n",buff);
      system(buff);
      free(ipaddr);
      IPADDR=NULL;
    }

    if(Scanlist==NULL) {
        AddNetworks();
        AddOldNetworks();
    }
    switch(butno) {
      case 1: 
        RetVal=0;
        ret=1;
        break;
      case 2: 
        if((Scanlist==NULL)||(Dcount(Scanlist)==0)) {
          RetVal=0;
          fprintf(stderr,"No Wireless Network\n");
          break;
        }
        id = GetSSIDid(Scanlist,ssid);
        if(id==0) {
          fprintf(stderr,"Unknown SSID: %s\n",ssid);
          return 0;
        }
        Resetlink(Scanlist);
        Dposition(Scanlist,id);
        nt =(NETLIST *) Getrecord(Scanlist);
        id = nt->id;
        fprintf(stderr,"Trying to Connect: %s\n",nt->ssid);
        if(nt->sec < 0) {
          if(PSK != NULL) strcpy(nt->psk,PSK);
          else {
            nt->psk[0]='\0';
            printf("Give  passkey: ");
            scanf("%s",nt->psk);
          }
          sprintf(buff,"/usr/share/config/lina/Wireless/%-s",nt->ssid);
          fp = fopen(buff,"w");
          if(fp != NULL) {
            nt->sec=0;
            if(nt->psk[0]!='\0') {
              fprintf(fp,"%s %d %d\n",nt->psk,nt->proto,nt->group);
              nt->sec =1;
              sprintf(buff,"wpa_cli set_network %d psk \\\"%-s\"\\",nt->id,nt->psk);
              runjob(buff,ProcessSet);
              if(!OK) {sleep(3); runjob(buff,ProcessSet);}
            } 
            fclose(fp);
          }
        }
        ConnectNetwork(nt);
        OK=1;
        runjob("killall -9 dhclient",WaitForProcess);
        sprintf(buff,"dhclient -cf /tmp/dhclient.conf -v %-s",Wdev);
        printf("%s\n",buff);
        system(buff);
        RetVal = OK;
        break;
    }
  }
  return ret;
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
int GetWdev(void) {
  char **devs,**files,buff[200];
  int i,j,OK=0;
  if(Wdev != NULL) { OK=1; return 1;}
  devs=kgFolderMenu("/sys/class/net");
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
  printf("Wireless Device : %s\n",Wdev);
  return OK;
}
char * WirelessStatus(void) {
  char buff[200];
  char ipaddr[50];
  int n,pp2,OK=0;
  char *pt;
  Connected=0;
  if(GetWdev()==0) return 0;
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
            printf("  %s connected: IP Addr: %s\n",Wdev,ipaddr);
            break;
          }
        }
      }
    }
  }
  if(Connected) {
    OK=1;
    pt = (char *) malloc(strlen(ipaddr)+1);
    strcpy(pt,ipaddr);
    runjob("wpa_cli status",ProcessStatus);
    IPADDR=pt;
  }
  else {
    pt=NULL;
    fprintf(stderr,"Not Connected\n");
  }
 
  return pt;

}
static int CheckCliConnection(void){
   OK=0;
   runjob("wpa_cli status",ProcessCli);
   return OK;
}
 
int  InitWpa(void) {
  unsigned char buff[300];
  int wait=0;
  int ret=0;
  char *ipaddr;
  if(GetWdev()) {
   MakeConfigFile();
   MakeDhclient();
   while(!CheckProcess("wpa_supplicant") ){
     wait++;
     if(wait > 10) {
       fprintf(stderr,"Failed to start wpa_supplicant\n");
       return 0;
     }
     sprintf(buff,"wpa_supplicant -i%-s -c/tmp/wpa.conf -B",Wdev);
     printf("%s\n",buff);
     runjob(buff,WaitForProcess);
   }
#if 0
   runjob("wpa_cli scan",NULL);
   ipaddr = WirelessStatus();
   ret =0;
   if(ipaddr != NULL) {
	   ret =1;
           IPADDR =ipaddr;
   }
   printf("%s : %s\n",Wdev,ipaddr);
#endif
  }
  else  {
    printf("Could not get Wdev\n");
    ret= 0;
  }
  return ret;
}
