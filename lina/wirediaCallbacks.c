#include <stdio.h>
#include <string.h>
#include <kulina.h>
#include "netlist.h"
  extern char *Wdev;
  extern int RetVal;
  extern int Connected;
  extern int OK;
  extern char Curssid [ 100 ] ;
  int runbin ( char *job ) ;
  int Runaddnetdia ( void *, char *, char * ) ;
  int AddNetworks ( void ) ;
  int AddOldNetworks ( void ) ;
  int addnetwork ( NETLIST *nt ) ;
  int Runpskdia ( void *, char * ) ;
  int ProcessSet ( int pip0 , int pip1 , int Pid ) ;
  int WaitForProcess ( int pip0 , int pip1 , int Pid ) ;
  int GetState ( void ) ;
  int ConnectNetwork ( NETLIST *nt ) ;
  int SetConnectState ( void *Tmp ) ;
  int runjob ( char *job , int ( *ProcessOut ) ( int , int , int ) ) ;
  extern Dlink *Scanlist;
  ThumbNail ** MakeScanThumbnails ( Dlink *Scanlist ) ;
  int wirediabutton1callback ( int butno , int i , void *Tmp ) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
      DIALOG *D;DIN *B;
      int n , ret = 0 , id;
      ThumbNail **th;
      char ssid [ 200 ] , psk [ 200 ] , buff [ 500 ] ;
      NETLIST *nt;
      DIX *X;
      D = ( DIALOG * ) Tmp;
      B = ( DIN * ) kgGetWidget ( Tmp , i ) ;
      n = B-> nx*B-> ny;
      X = ( DIX * ) kgGetWidget ( Tmp , 5 ) ;
      switch ( butno ) {
          case 1:
          if ( Scanlist != NULL ) { Dempty ( Scanlist ) ;Scanlist = NULL;}
          th = ( ThumbNail ** ) kgGetList ( X ) ;
          if ( th != NULL ) kgFreeThumbNails ( th ) ;
          AddNetworks ( ) ;
          AddOldNetworks ( ) ;
          kgSetList ( X , ( void ** ) MakeScanThumbnails ( Scanlist ) ) ;
          kgUpdateWidget ( X ) ;
          kgUpdateOn ( Tmp ) ;
          break;
          case 2:
          if ( Runaddnetdia ( Tmp , ssid , psk ) ) {
              FILE *fp;
              nt = ( NETLIST * ) malloc ( sizeof ( NETLIST ) ) ;
              strcpy ( nt-> ssid , ssid ) ;
              strcpy ( nt-> psk , psk ) ;
              nt-> sig = -100;
              Resetlink ( Scanlist ) ;
              Dinsert ( Scanlist , nt ) ;
              sprintf ( buff , "/usr/share/config/lina/Wireless/%-s" , ssid ) ;
              fp = fopen ( buff , "w" ) ;
              if ( psk [ 0 ] != '\0' ) fprintf ( fp , "%s %d %d\n" , psk , nt-> proto , nt-> group ) ;
                  
              fclose ( fp ) ;
              addnetwork ( nt ) ;
              th = ( ThumbNail ** ) kgGetList ( X ) ;
              if ( th != NULL ) kgFreeThumbNails ( th ) ;
              kgSetList ( X , ( void ** ) MakeScanThumbnails ( Scanlist ) ) ;
              kgUpdateWidget ( X ) ;
              kgUpdateOn ( Tmp ) ;
          }
          break;
          case 3:
          if ( ( Scanlist == NULL ) || ( Dcount ( Scanlist ) == 0 ) ) {
              RetVal = 0;
              kgPrintf ( Tmp , 1 , "!z43No Wireless Network" ) ;
              kgUpdateOn ( Tmp ) ;
              break;
          }
          id = kgGetSelection ( kgGetWidget ( Tmp , 5 ) ) ;
          Dposition ( Scanlist , id ) ;
          nt = ( NETLIST * ) Getrecord ( Scanlist ) ;
          sprintf ( buff , "/usr/share/config/lina/Wireless/%-s" , nt-> ssid ) ;
          remove ( buff ) ;
          kgPrintf ( Tmp , 1 , " " ) ;
          sprintf ( buff , "!z43REMOVED %-s" , nt-> ssid ) ;
          kgPrintf ( Tmp , 1 , buff ) ;
          kgUpdateOn ( Tmp ) ;
          Dposition ( Scanlist , id ) ;
          Ddelete ( Scanlist ) ;
          th = ( ThumbNail ** ) kgGetList ( X ) ;
          if ( th != NULL ) kgFreeThumbNails ( th ) ;
          kgSetList ( X , ( void ** ) MakeScanThumbnails ( Scanlist ) ) ;
          kgUpdateWidget ( X ) ;
          kgUpdateOn ( Tmp ) ;
          break;
      }
      return ret;
  }
  void wirediabutton1init ( DIN *B , void *pt ) {
  }
  int wirediasplbutton1callback ( int butno , int i , void *Tmp ) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
      DIALOG *D;DIL *B;
      int n , ret = 1 , pp2;
      char buff [ 200 ] ;
      D = ( DIALOG * ) Tmp;
      B = ( DIL * ) kgGetWidget ( Tmp , i ) ;
      n = B-> nx;
      switch ( butno ) {
          case 1:
          RetVal = 0;
          break;
          case 2:
          sprintf ( buff , "ip link set %-s down" , Wdev ) ;
          pp2 = runbin ( buff ) ;
          while ( ( n = read ( pp2 , buff , 100 ) ) == 100 ) {
          }
          kgPrintf ( Tmp , 1 , " " ) ;
          kgPrintf ( Tmp , 1 , "!z43Disonnected Wireless" ) ;
          kgUpdateOn ( Tmp ) ;
          sleep ( 2 ) ;
          RetVal = 1;
          Connected = 0;
          SetConnectState ( Tmp ) ;
          ret = 0;
          break;
      }
      return ret;
  }
  void wirediasplbutton1init ( DIL *B , void *pt ) {
  }
  int wirediasplbutton2callback ( int butno , int i , void *Tmp ) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
      DIALOG *D;DIL *B;
      char buff [ 200 ] ;
      NETLIST *nt;
      FILE *fp;
      int n , ret = 1 , OK = 0 , id;
      int pp2;
      D = ( DIALOG * ) Tmp;
      B = ( DIL * ) kgGetWidget ( Tmp , i ) ;
      n = B-> nx;
      switch ( butno ) {
          case 1:
          RetVal = 0;
          ret = 1;
          break;
          case 2:
          if ( ( Scanlist == NULL ) || ( Dcount ( Scanlist ) == 0 ) ) {
              RetVal = 0;
              kgPrintf ( Tmp , 1 , "!z43No Wireless Network" ) ;
              kgUpdateOn ( Tmp ) ;
              break;
          }
          id = kgGetSelection ( kgGetWidget ( Tmp , 5 ) ) ;
          Dposition ( Scanlist , id ) ;
          nt = ( NETLIST * ) Getrecord ( Scanlist ) ;
          id = nt-> id;
          sprintf ( buff , "!z43Trying to Connect: %s" , nt-> ssid ) ;
          kgPrintf ( Tmp , 1 , buff ) ;
          kgUpdateOn ( Tmp ) ;
          if ( nt-> sec < 0 ) {
              nt-> psk [ 0 ] = '\0';
              Runpskdia ( Tmp , nt-> psk ) ;
              sprintf ( buff , "/usr/share/config/lina/Wireless/%-s" , nt-> ssid ) ;
              fp = fopen ( buff , "w" ) ;
              if ( fp != NULL ) {
                  nt-> sec = 0;
                  if ( nt-> psk [ 0 ] != '\0' ) {
                      fprintf ( fp , "%s %d %d\n" , nt-> psk , nt-> proto , nt-> group ) ;
                      nt-> sec = 1;
                      sprintf ( buff , "wpa_cli set_network %d psk \\\"%-s\"\\" , nt-> id , nt-> psk ) ;
                          
                      runjob ( buff , ProcessSet ) ;
                      if ( ! OK ) {sleep ( 2 ) ; runjob ( buff , ProcessSet ) ;}
                  }
                  fclose ( fp ) ;
              }
          }
          ConnectNetwork ( nt ) ;
          OK = 0;
          if ( GetState ( ) ) {
              runjob ( "killall -9 dhclient" , WaitForProcess ) ;
              sprintf ( buff , "dhclient -cf /tmp/dhclient.conf -v %-s" , Wdev ) ;
              pp2 = runbin ( buff ) ;
              OK = 0;
              while ( ( n = read ( pp2 , buff , 100 ) ) == 100 ) {
//           Dprintf(Tmp,0,buff);
//           kgUpdateOn(Tmp);
                  buff [ strlen ( "bound to" ) ] = '\0';
                  if ( strcmp ( buff , "bound to" ) == 0 ) OK = 1;
              }
          }
          kgPrintf ( Tmp , 1 , " " ) ;
          if ( OK ) {
              FILE *fp;
              char def [ 300 ] ;
//        kgPrintf(Tmp,1,"!z43!c06Connected Wireless");
              sprintf ( buff , "!z43Wireless Connected to %s" , nt-> ssid ) ;
              kgPrintf ( Tmp , 1 , buff ) ;
              kgUpdateOn ( Tmp ) ;
              strcpy ( def , "/usr/share/config/lina/wireless/default" ) ;
              fp = fopen ( def , "w" ) ;
              if ( fp != NULL ) {
                  fprintf ( fp , "%s\n" , nt-> ssid ) ;
                  fclose ( fp ) ;
              }
              sleep ( 1 ) ;
          }
          else {
              kgPrintf ( Tmp , 1 , "!z43!c03Failed Wireless" ) ;
              kgUpdateOn ( Tmp ) ;
          }
          ret = OK;
          RetVal = OK;
          break;
      }
      return ret;
  }
  void wirediasplbutton2init ( DIL *B , void *pt ) {
  }
  int wirediabrowser1callback ( int item , int i , void *Tmp ) {
  /*********************************** 
    item : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
      DIALOG *D;DIX *X;void *pt;
      int ret = 1;
      D = ( DIALOG * ) Tmp;
      pt = D-> pt;
      X = ( DIX * ) kgGetWidget ( Tmp , i ) ;
      switch ( item ) {
          case 1:
          break;
      }
      return ret;
  }
  void wirediabrowser1init ( DIX *X , void *pt ) {
 // One may setup browser list here by setting X->list
 // if it need to be freed set it as X->pt also
      kgSetList ( X , ( void ** ) MakeScanThumbnails ( Scanlist ) ) ;
  }
  int wirediainit ( void *Tmp ) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  /* you add any initialisation here */
      int ret = 1;
      char buff [ 200 ] ;
      ThumbNail **th = NULL;
      DIALOG *D;void *pt;
      DIX *X;
      D = ( DIALOG * ) Tmp;
      pt = D-> pt;
      X = ( DIX * ) kgGetWidget ( Tmp , 5 ) ;
      RetVal = 0;
      if ( Connected ) {
          sprintf ( buff , "!z43Wireless Connected to %s" , Curssid ) ;
          kgPrintf ( Tmp , 1 , buff ) ;
      }
      else {
          if ( Scanlist == NULL ) {
              kgPrintf ( Tmp , 1 , "!z43Not Connected... Scanning PL. WAIT" ) ;
              kgUpdateOn ( Tmp ) ;
              AddNetworks ( ) ;
              if ( Scanlist == NULL ) kgPrintf ( Tmp , 1 , "!z43No Scanned Wireless. Adding Old" ) ;
                  
              kgUpdateOn ( Tmp ) ;
              AddOldNetworks ( ) ;
              th = ( ThumbNail ** ) kgGetList ( X ) ;
              if ( th != NULL ) kgFreeThumbNails ( th ) ;
              printf ( "Adding List %d\n" , Dcount ( Scanlist ) ) ;
              kgSetList ( X , ( void ** ) MakeScanThumbnails ( Scanlist ) ) ;
              kgUpdateWidget ( X ) ;
              kgPrintf ( Tmp , 1 , "!z65 SELECT AND CONNECT" ) ;
              kgUpdateOn ( Tmp ) ;
              kgUpdateOn ( Tmp ) ;
          }
          else kgPrintf ( Tmp , 1 , "!z43Not Connected, Select and Connect" ) ;
      }
      return ret;
  }
  int wirediacleanup ( void *Tmp ) {
  /* you add any cleanup/mem free here */
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
      int ret = 1;
      DIALOG *D;void *pt;
      D = ( DIALOG * ) Tmp;
      DIP *P;
      pt = D-> pt;
      P = ( DIP * ) kgGetWidget ( Tmp , 0 ) ;
      kgFreeImage ( P-> xpm ) ;
      P-> xpm = NULL;
      return ret;
  }
  int wirediaCallBack ( void *Tmp , void *tmp ) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    tmp :  Pointer to KBEVENT  
   ***********************************/ 
      int ret = 0;
      DIALOG *D;
      KBEVENT *kbe;
      DIS *S;
      D = ( DIALOG * ) Tmp;
      kbe = ( KBEVENT * ) tmp;
//  S= kgGetClickedWidget(D);
      S = kgGetLocationWidget ( D , kbe-> x , kbe-> y ) ;
      if ( ( S != NULL ) && ( S-> code == 's' ) ) return 0;
#if 1
      if ( kbe-> event == 1 ) {
          if ( kbe-> button == 1 ) {
              RetVal = 0;
              ret = 1;
          }
      }
#endif
      return ret;
  }
  int wirediaResizeCallBack ( void *Tmp ) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
      int ret = 0;
      int xres , yres , dx , dy;
      DIALOG *D;
      D = ( DIALOG * ) Tmp;
      kgGetWindowSize ( D , & xres , & yres ) ;
      dx = xres - D-> xl;
      dy = yres - D-> yl;
  /* extra code */
      D-> xl = xres;
      D-> yl = yres;
      kgRedrawDialog ( D ) ;
      return ret;
  }
  int wirediaWaitCallBack ( void *Tmp ) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    Called while waiting for event  
    return value 1 will close the the UI  
   ***********************************/ 
      int ret = 0;
      return ret;
  }
