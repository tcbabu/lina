#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <shadow.h>
#include <sys/types.h>
#include <unistd.h>
#include <crypt.h>
#include "passwdfile.h"
  int CheckLogin ( int Index , char *password ) {
      int ret = 0;
      char *p , *correct , *supplied , *salt;
      if ( Index <= 0 ) return 0;
      correct = GetUserPw ( Index ) ;
      salt = strdup ( correct ) ;
      if ( salt == NULL ) return 2;
      p = strchr ( salt + 1 , '$' ) ;
      if ( p == NULL ) ret = 2;
      else {
          p = strchr ( p + 1 , '$' ) ;
          if ( p == NULL ) ret = 3;
          else {
              p [ 0 ] = 0;
              supplied = crypt ( password , salt ) ;
              if ( supplied == NULL ) ret = 4;
              else ret = ! strcmp ( supplied , correct ) ;
          }
      }
      free ( salt ) ;
      return ret;
  }
