#include <kulina.h>
#include "headers.h"
  static char *GetExec ( char *file , char *Token ) {
      char* res = NULL;
      char buff [ 500 ] , tmp [ 100 ] ;
      FILE *fp;
      int i , l;
      fp = fopen ( file , "r" ) ;
      if ( fp == NULL ) {
          return NULL;
      }
      l = strlen ( Token ) ;
      while ( fgets ( buff , 499 , fp ) != NULL ) {
          if ( strstr ( buff , "[" ) != NULL ) continue;
          i = 0;
          while ( ( buff [ i ] >= ' ' ) || ( buff [ i ] == '\t' ) ) i++;
          while ( buff [ i ] <= ' ' ) {buff [ i ] = '\0';i--;}
          i = 0;
          while ( buff [ i ] == ' ' ) i++;
          if ( strncmp ( buff+i , Token , l ) == 0 ) {
              i += l;
              while ( buff [ i ] != '=' ) i++;
              i++;
              while ( buff [ i ] == ' ' ) i++;
              if ( buff [ i ] < ' ' ) {fclose ( fp ) ; return NULL;}
              sscanf ( buff+i , "%s" , tmp ) ;
              if ( ( res = kgWhich ( tmp ) ) == NULL ) return NULL;
              free ( res ) ;
              res = ( char * ) malloc ( strlen ( buff+i ) +1 ) ;
              strcpy ( res , buff+i ) ;
              fclose ( fp ) ;
          return ( res ) ; }
      }
      fclose ( fp ) ;
      return NULL;
  }
  static char *GetString ( char *file , char *Token ) {
      char* res = NULL;
      char buff [ 500 ] ;
      FILE *fp;
      int i , l;
      fp = fopen ( file , "r" ) ;
      if ( fp == NULL ) {
          return NULL;
      }
      l = strlen ( Token ) ;
      while ( fgets ( buff , 499 , fp ) != NULL ) {
          if ( strstr ( buff , "[" ) != NULL ) continue;
          i = 0;
          while ( ( buff [ i ] >= ' ' ) || ( buff [ i ] == '\t' ) ) i++;
          while ( buff [ i ] <= ' ' ) {buff [ i ] = '\0';i--;}
          i = 0;
          while ( buff [ i ] == ' ' ) i++;
          if ( strncmp ( buff+i , Token , l ) == 0 ) {
              i += l;
              while ( buff [ i ] != '=' ) i++;
              i++;
              while ( buff [ i ] == ' ' ) i++;
              if ( buff [ i ] < ' ' ) {fclose ( fp ) ; return NULL;}
              res = ( char * ) malloc ( strlen ( buff+i ) +1 ) ;
              strcpy ( res , buff+i ) ;
              fclose ( fp ) ;
          return ( res ) ; }
      }
      fclose ( fp ) ;
      return NULL;
  }
  void *MakeXsessionList ( void ) {
      int i , pos , SKIP , l;
      char **m , *pt , *res;
      char buff [ 500 ] ;
      FILE *fp;
      SESSIONINFO *bt , Btmp;
      Dlink *Dlist = NULL;
      if ( Dlist == NULL ) {
          Dlist = Dopen ( ) ;
          m = kgFileMenu ( "/usr/share/xsessions" , "*.desktop" ) ;
          if ( m != NULL ) {
              i = 0;
              while ( m [ i ] != NULL ) {
                  strcpy ( buff , "/usr/share/xsessions/" ) ;
                  strcat ( buff , m [ i ] ) ;
                  i++;
                  res = GetExec ( buff , "Exec" ) ;
                  if ( res == NULL ) {
//		printf("res==NULL\n");
                      continue;
                  }
                  strcpy ( Btmp.Command , res ) ;
                  free ( res ) ;
                  strcpy ( Btmp.Name , " " ) ;
                  res = GetString ( buff , "Name" ) ;
                  if ( res != NULL ) {strcpy ( Btmp.Name , res ) ; free ( res ) ;}
                  bt = ( SESSIONINFO * ) malloc ( sizeof ( SESSIONINFO ) ) ;
                  strcpy ( bt-> Command , Btmp.Command ) ;
                  strcpy ( bt-> Name , Btmp.Name ) ;
                  Dadd ( Dlist , bt ) ;
               } //while
          }
      }
      return Dlist;
  }
