#include "dlink.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <malloc.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdarg.h>
#define Free(x) {\
  if ( ( x ) != NULL ) free ( ( x ) ) ;\
       ( x ) = NULL;\
  }
/* checking for string
   if found position after string is returned
*/
  static int _check_for_string ( char *s , char *chk ) {
      int ls , lchk , i , k , count , ret = 0 , j , ch;
      ls = strlen ( s ) ;
      k = 0;while ( ( chk [ k ] != '\0' ) && ( chk [ k ] != '*' ) && 
          ( chk [ k ] != '?' ) ) k++;
      ch = chk [ k ] ;chk [ k ] = '\0';
      lchk = strlen ( chk ) ;
      if ( lchk > ls ) return 0;
      for ( i = 0;i < ( ls-lchk+1 ) ;i++ ) {
          count = 0;
          for ( j = 0;j < lchk;j++ ) if ( s [ i+j ] == chk [ j ] ) count++;
          if ( count == lchk ) {ret = i+lchk; break;}
      }
      chk [ k ] = ch;
      return ret;
  }
  static int _check_for_start_string ( char *s , char *chk ) {
      int ls , lchk , i , count , ret = 0 , j , ch;
      ls = strlen ( s ) ;
      i = 0;while ( ( chk [ i ] != '\0' ) && ( chk [ i ] != '*' ) && 
          ( chk [ i ] != '?' ) ) i++;
      ch = chk [ i ] ;chk [ i ] = '\0';
      lchk = strlen ( chk ) ;
      if ( lchk <= ls ) {
          count = 0;
          for ( j = 0;j < lchk;j++ ) if ( s [ j ] == chk [ j ] ) count++; else break;
          if ( count == lchk ) {ret = lchk; }
      }
      chk [ i ] = ch;
      return ret;
  }
  static int _skip_string ( char *chk ) {
      int i = 0;
      while ( ( chk [ i ] != '\0' ) && ( chk [ i ] != '*' ) && 
          ( chk [ i ] != '?' ) ) i++;
      return i;
  }
/* checking for character
   if found position is returned
*/
  static int _check_for_char ( char *s , char ch ) {
      int i = 0;
      while ( ( s [ i ] != ch ) && ( s [ i ] != '\0' ) ) i++;
      if ( s [ i ] == '\0' ) return 0;
      else return i;
  }
  static int _filter_other_options ( char *s , char *fltr ) {
      int pos , skip;
      if ( fltr [ 0 ] == ' ' ) return 1;
      if ( fltr [ 0 ] == '\0' ) {
          if ( s [ 0 ] == '\0' ) return 1;
          else return 0;
      }
      if ( fltr [ 0 ] == '*' ) {
          if ( fltr [ 1 ] == '.' ) {
              pos = _check_for_char ( s , '.' ) ;
              if ( pos != 0 ) return _filter_other_options ( s+pos+1 , fltr+2 ) ;
              else return 0;
          }
          else {
              if ( fltr [ 1 ] == '\0' ) return 1;
              else if ( fltr [ 1 ] == '?' ) {
                  return _filter_other_options ( s+1 , fltr+2 ) ;
              }
              else {
                  pos = _check_for_string ( s , fltr+1 ) ;
                  if ( pos == 0 ) {
                      return 0;
                  }
                  else {
                      skip = _skip_string ( fltr+1 ) ;
                      return _filter_other_options ( s+pos , fltr+1+skip ) ;
                  }
              }
          }
      }
      else {
          if ( fltr [ 0 ] == '?' ) {
              return _filter_other_options ( s+1 , fltr+1 ) ;
          }
          else {
              pos = _check_for_start_string ( s , fltr ) ;
              if ( pos == 0 ) {
                  return 0;
              }
              else {
                  skip = _skip_string ( fltr ) ;
                  return _filter_other_options ( s+pos , fltr+skip ) ;
              }
          }
      }
  }
  static int _filter_string ( char *s , char *fltr ) {
      int i , j , ret = 0;
      char *buf;
      if ( strcmp ( "*" , fltr ) == 0 ) return 1;
      if ( strcmp ( "*.*" , fltr ) == 0 ) {
          i = 0;
          while ( s [ i ] != '\0' ) if ( s [ i++ ] == '.' ) return 1;
          return 0;
      }
      else{
          buf = ( char * ) malloc ( strlen ( fltr ) +1 ) ;
//    strcpy(buf,fltr);
          i = 0;
          while ( 1 ) {
              j = 0;
              while ( fltr [ i ] == ' ' ) i++;
              if ( fltr [ i ] < ' ' ) break;
              while ( fltr [ i ] > ' ' ) buf [ j++ ] = fltr [ i++ ] ;
              buf [ j ] = '\0';
              ret = _filter_other_options ( s , buf ) ;
              if ( ret ) break;
          }
          Free ( buf ) ;
          return ret;
      }
  }
  void *kgFreeDouble ( void **mem ) {
      int i = 0;
      if ( mem != NULL ) {
          i = 0;while ( mem [ i ] != NULL ) {free ( mem [ i ] ) ;i++;}
          free ( mem ) ;
      }
      return NULL;
  }
  static void arrange ( char **m , int n ) {
      int i , j;
      char *pt;
      for ( i = 0;i < n-1;i++ ) {
          for ( j = i+1;j < n;j++ ) {
              if ( strcmp ( m [ i ] , m [ j ] ) > 0 ) {pt = m [ j ] ;
                  m [ j ] = m [ i ] ;m [ i ] = pt;}
          }
      }
  }
  static char ** _uiFileMenu ( char *dir , char *filter ) {
      Dlink *L = NULL;
      int type , nf = 0 , n = 0 , j = 0;
      char code , buf [ 500 ] , blnk [ 2 ] = " ";
      char **m , *item;
      DIR *dp;
      struct dirent *pt = NULL;
      struct stat st;
      dp = opendir ( dir ) ;
      if ( dp == NULL ) return NULL;
      L = Dopen ( ) ;
      while ( ( pt = readdir ( dp ) ) != NULL ) {
          if ( pt-> d_name [ 0 ] == '.' ) continue;
/*
*/
#if 1
          strcpy ( buf , dir ) ;
          strcat ( buf , "/" ) ;
          strcat ( buf , pt-> d_name ) ;
          stat ( buf , & st ) ;
          if ( S_ISREG ( st.st_mode ) && ( ! S_ISDIR ( st.st_mode ) ) ) {
#endif
              sprintf ( buf , "%-s" , pt-> d_name ) ;
              if ( _filter_string ( buf , filter ) ) {
                  item = ( char * ) malloc ( strlen ( buf ) +5 ) ;
                  strcpy ( item , buf ) ;Dadd ( L , item ) ;
                  nf++;
              }
              else {
                  continue;
              }
          }
          else continue;
          code = 'f';
      }
      Resetlink ( L ) ;
      nf = Dcount ( L ) ;
      Resetlink ( L ) ;
      n = ( nf ) +1;
      m = ( char ** ) malloc ( sizeof ( char * ) *n ) ;
      j = 0;
      while ( ( m [ j ] = ( char * ) Getrecord ( L ) ) != NULL ) { j++;}
      arrange ( m , nf ) ;
      closedir ( dp ) ;
      Dfree ( L ) ;
      return m;
  }
  char **kgFileMenu ( char *dir , char *filter ) {
      return _uiFileMenu ( dir , filter ) ;
  }
  char ** _uiFolderMenu ( char *d_name ) {
      DIR *dp;
      char **menu , *item;
      Dlink *L;
      struct dirent *pt = NULL;
      struct stat st;
      int ln , nd = 0 , i , type;
      char buf [ 500 ] ;
      dp = opendir ( d_name ) ;
      if ( dp == NULL ) {
          printf ( "No such dir:%s\n" , d_name ) ;
          return NULL;
      }
      L = Dopen ( ) ;
      while ( ( pt = readdir ( dp ) ) != NULL ) {
          if ( pt-> d_name [ 0 ] == '.' ) continue;
          strcpy ( buf , d_name ) ;
          strcat ( buf , "/" ) ;
          strcat ( buf , pt-> d_name ) ;
          stat ( buf , & st ) ;
          if ( S_ISDIR ( st.st_mode ) ) {
//     if(((st.st_mode & S_IFMT)==S_IFDIR)) {
              ln = strlen ( pt-> d_name ) ;
              item = ( char * ) malloc ( ln+5 ) ;
              strcpy ( item , pt-> d_name ) ;
              Dadd ( L , item ) ;
//      printf("%s\n",item);
              nd++;
          }
      }
      closedir ( dp ) ;
      menu = ( char ** ) malloc ( sizeof ( char * ) * ( nd+1 ) ) ;
      Resetlink ( L ) ;
      i = 0;
      while ( ( item = ( char * ) Getrecord ( L ) ) != NULL ) {
          menu [ i ] = item;
//   printf("%s\n",item);
          i++;
      }
      menu [ i ] = NULL;
      arrange ( menu , i ) ;
      Dfree ( L ) ;
      return menu;
  }
  char ** kgFolderMenu ( char *d_name ) {
      return _uiFolderMenu ( d_name ) ;
  }
  static int comppath ( void *pt1 , void *pt2 ) {
      return strcmp ( ( char * ) pt1 , ( char * ) pt2 ) ;
  }
  static int compdup ( void *pt1 , void *pt2 ) {
      int ret;
      ret = strcmp ( ( char * ) pt1 , ( char * ) pt2 ) ;
      if ( ret == 0 ) {return 1;}
      else return 0;
  }
  char *kgWhich ( char *pgr ) {
/* Caller must free result if it is not NULL */
      int i = 0 , j , End = 0;
      char *pt , **m , *res = NULL , *cpt;
      char path [ 5000 ] ;
      Dlink *L;
      if ( pgr [ 0 ] == '/' ) {
          int l;
          char **m , *pt1;
          strcpy ( path , pgr ) ;
          pt = path;
          l = strlen ( pt ) ;
          while ( pt [ l ] != '/' ) l--;
          if ( l == 0 ) {
              m = kgFileMenu ( "/" , pt+1 ) ;
          }
          else {
              pt1 = pt+l+1;
              pt [ l ] = '\0';
              m = kgFileMenu ( pt , pt1 ) ;
          }
          if ( ( m == NULL ) ) {return NULL;}
          if ( ( m [ 0 ] == NULL ) ) {kgFreeDouble ( ( void ** ) m ) ;return NULL;}
          kgFreeDouble ( ( void ** ) m ) ;
          res = ( char * ) malloc ( strlen ( pgr ) +1 ) ;
          strcpy ( res , pgr ) ;
          return res;
      }
      pt = getenv ( "PATH" ) ;
      if ( pt == NULL ) return NULL;
      strcpy ( path , getenv ( "PATH" ) ) ;
      L = Dopen ( ) ;
      while ( ! End ) {
          j = i;
          if ( pt [ j ] < ' ' ) break;
          while ( path [ j ] != ':' ) {
              if ( path [ j ] < ' ' ) {End = 1;break;}
              j++;
          }
          path [ j ] = '\0';
          cpt = ( char * ) malloc ( strlen ( path+i ) +1 ) ;
          strcpy ( cpt , path+i ) ;
          Dadd ( L , cpt ) ;
          i = j+1;
      }
      Dsort ( L , comppath ) ;
      Drmvdup_cond ( L , compdup ) ;
      Resetlink ( L ) ;
      while ( ( pt = ( char * ) Getrecord ( L ) ) != NULL ) {
          m = kgFileMenu ( pt , pgr ) ;
          if ( m == NULL ) continue;
          if ( m [ 0 ] == NULL ) { free ( m ) ;continue;}
          res = ( char * ) malloc ( strlen ( pt ) +1+strlen ( m [ 0 ] ) +1 ) ;
          strcpy ( res , pt ) ;
          strcat ( res , "/" ) ;
          strcat ( res , m [ 0 ] ) ;
          i = 0;
          while ( m [ i ] != NULL ) {free ( m [ i ] ) ;i++;}
          free ( m ) ;
          break;
      }
      Dempty ( L ) ;
      return res;
  }
