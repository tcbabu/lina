  int CheckString ( char *s1 , char *s2 ) ;
  int * GetGroups ( LINACONFIG *lc , char *UserName ) {
      FILE *pf = NULL , *sf = NULL;
      char buff [ 2000 ] , *pt , *Pw , sbuff [ 500 ] , *tmp , *tmp1;
      int GroupNo , Ugid , *ipt , count , *array , i;
      Dlink *Glist = NULL;
      pf = popen ( "cat /etc/group" , "r" ) ;
      if ( pf == NULL ) return NULL;
      Glist = Dopen ( ) ;
      Ugid = GetGid ( lc , GetIndexFromLogin ( lc , UserName ) ;
      while ( fgets ( buff , 1999 , pf ) != NULL ) {
          if ( ( pt = GetLoginId ( buff ) ) == NULL ) continue;
#if 1
          Pw = GetUserPasswd ( buff ) ;
          if ( Pw == NULL ) continue;
          if ( Pw [ 0 ] == '\0' ) { free ( Pw ) ;continue;}
#endif
          GroupNo = GetUserNo ( buff ) ;
          if ( GroupNo == Ugid ) {free ( Pw ) ;continue;}
          if ( CheckString ( buff , UserName ) == 1 ) {
              ipt = ( int * ) malloc ( sizeof ( int ) ) ;
              *ipt = GroupNo;
              Dadd ( Glist , ipt ) ;
              printf ( "%s\n" , pt ) ;
          }
          free ( pt ) ;free ( Pw ) ;
      }
      Dsort ( Glist , CompId ) ;
      count = Dcount ( Glist ) ;
      array = ( int * ) malloc ( sizeof ( int ) * ( count+1 ) ) ;
      array [ count ] = NULL;
      Resetlink ( Glist ) ;
      i = 0;
      while ( ( ipt = ( int * ) Getrecord ( Glist ) ) != NULL ) {
          array [ i++ ] = *ipt;
      }
      Dempty ( Glist ) ;
      pclose ( pf ) ;
  }
