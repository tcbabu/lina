#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#define DISPLAY ":0.0"
  int InitWpa ( void ) ;
  void *Runlina ( void * ) ;
  int main ( void ) {
      int i = 0;
      char *dspname;
      static char *path = NULL , nenv [ 500 ] ;
      path = getenv ( "PATH" ) ;
      sprintf ( nenv , "PATH=%-s:/usr/X11R76/bin:/usr/X11/bin:/usr/X11R6/bin" , path ) ;
          
      putenv ( nenv ) ;
#if 1
//   printf("%s\n",kgWhich("X"));
      dspname = getenv ( "DISPLAY" ) ;
      if ( ! dspname ) {
          dspname = DISPLAY;
          setenv ( "DISPLAY" , dspname , 1 ) ;
      }
#endif
//  for(i=0;i<60;i++)printf("\n");
//  InitWpa();
      Runlina ( NULL ) ;
      return 1;
  }
