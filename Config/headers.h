#ifndef D_HEADERS
#define D_HEADERS
#include "kulina.h"
typedef struct _lina_config {
  int Red;    // 0 to 255 red of theme
  int Green;    // 0 to 255 green of theme
  int Blue;    // 0 to 255 blue of theme
  float Transparency;
  float fac;  // Rounding factor  0. to  1.
  char Bkgr[300];
  char RootPic[300];
  char UserPic[300];
  Dlink *Ulist;   //User List
  Dlink *Slist;   // Session list
  void *Rthumb;  // Thumbnail of Root internal
  void *Uthumb;  // Thumbnail of User Picture ; internal
  void *Rimg;    // Small Picture of user ; internal
  void *Uimg;    // Small Picture of user ; internal
  char DefUser[30]; //  Default User
  char DefSession[30]; // Default session
  int Action,Powerdown,Session; // internal
  int Index,SesId,TextMode;
  void *Mask;
  int NoRootLogin; // 1 not to allow root login 
  int FontRed,FontGreen,FontBlue;
  int HighRed,HighGreen,HighBlue;
  int SafeMode;
  int ShowTime;
  int ButRed,ButGreen,ButBlue;
  int DateRed,DateGreen,DateBlue;
  int DateFont;
} LINACONFIG;
typedef struct _session_info {
  char Name[30];
  char Command[100];
} SESSIONINFO;
typedef struct _userinfo {
   int UserNo;
   int gid;
   char *LoginId;
   char *Passwd;
   char *Name;
   char *Home;
   void *img;
   int DefSession;
   char Picture[200];
   void *uthumb;
   char *Shell;
} USERINFO;
void *MakePixmapImage(int xl,int yl,int red,int green,int blue,float fac);
void * DateTimeImg(LINACONFIG *lc,int xl,int yl);
#endif
