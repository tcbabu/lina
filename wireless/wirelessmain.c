#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>

int  ConnectSSID(char *ssid);
int  ScanSSID();
int  InitWpa(void);
int GetWdev(void);
int CheckProcess(char *);
int DeleteSSID (char *);
int WirelessStatus(void);

char *PSK=NULL,*SSID=NULL;
extern char *Wdev;
int Usage(void) {
    fprintf(stderr," Usage: \n"
                   " wireless -<option>  [<ssid>]\n"
                   " option: \n"
                   "  i:  wireless inteface\n"
                   "  l:  list ssid \n"
                   "  c <ssid>: connect ssid\n"
                   "  d <ssid>: delete ssid\n"
                   "  p <psk> : psk (needed for first use of ssid)\n"
                   "  s:  status\n"
                   "  B:  go background\n");
    exit(1);
}

int main(int argc,char *argv[]){
  unsigned char och;
  int i;
  int OK=0;
  int sid,status;
  
  
  if(argc<2) {
    Usage();
    return 0;
  }
  if ( access("/usr/share/config/lina/wireless",F_OK) != 0) {
     mkdir( "/usr/share/config/",0755);
     mkdir( "/usr/share/config/lina",0700);
     mkdir( "/usr/share/config/lina/wireless",0700);
  }
  i=1;
  while (i< argc) {
    if(argv[i][0]!='-') Usage();
    if(argv[i][1]=='\0') Usage();
    och=argv[i][2];
    switch(argv[i][1]){
     default: 
       Usage();
       break;
     case 'B':
       sid = fork();
       if(sid==0) {
         if(fork()!= 0) exit(0);
       }
       else {
        waitpid(sid,&status,0);
        exit(0);
       }
       break;
     case 'p':
       if(och != '\0') PSK = argv[i]+2;
       else {
         PSK = argv[i+1];
         i++;
       }
       break;
     case 'd':
       if(och != '\0') SSID = argv[i]+2;
       else {
         if((i+1) < argc ) SSID = argv[i+1];
         i++;
       }
       DeleteSSID(SSID);
       exit(0);
     case 'c':
       if(och != '\0') SSID = argv[i]+2;
       else {
         if((i+1) < argc ) SSID = argv[i+1];
         i++;
       }
       ConnectSSID(SSID);
       OK=1;
       break;
     case 'i':
       if(och != '\0') Wdev = argv[i]+2;
       else {
         if((i+1) < argc ) Wdev = argv[i+1];
         i++;
       }
       break;
     case 'l':
       if(InitWpa()) {
         ScanSSID();
       }
       exit(0);
     case 's':
       GetWdev();
       WirelessStatus();
       exit(0);
       
    }
    i++;
  }
  if (!OK) Usage();
  return 1;
}
