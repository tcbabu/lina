#ifndef D_PASSWDFILE
#define D_PASSWDFILE
#include "kulina.h"
#include "headers.h"
void ReadUsersInfo(LINACONFIG *lc);
void PrintUsersInfo(LINACONFIG *lc);
void CleanUsersInfo(LINACONFIG *lc);
/* Check for name and return its position 1 to last; otherwise return 0 */
int  GetIndexFromLogin(LINACONFIG *lc,char *LoginId);
char *GetUserId(LINACONFIG *lc,int no);
char *GetUserPw(LINACONFIG *lc,int no);
char *GetRealName(LINACONFIG *lc,int no);
ThumbNail **MakeUserThumbNails(LINACONFIG *lc);
ThumbNail **MakeSessionThumbNails(LINACONFIG *lc);
int CheckLogin(LINACONFIG *lc,int Index,char *password);
int GetUid(LINACONFIG *lc,int no);
int GetGid(LINACONFIG *lc,int no);
char * GetHomeDir(LINACONFIG *lc,int no);
char * GetShell(LINACONFIG *lc,int no);
char *GetSessionCommand(LINACONFIG *lc,int Index);
void *GetUserImage(LINACONFIG *lc,int no);
int  GetSessionFromName(LINACONFIG *lc,char *Name);
void *kgMaskImage(void *png,void *mask);
int  SetSupGroups(LINACONFIG *lc,int Index);
char *SetIdPasswd(LINACONFIG *lc,int no,char *pw);
USERINFO * InitUserInfo(void);
int InitConfig(LINACONFIG *lc);
int  GetIndexFromLogin(LINACONFIG *lc,char *LoginId) ;
int  GetSessionFromName(LINACONFIG *lc,char *Name);
void *ReadConfig(LINACONFIG *lc);
int GetIndexFromUid(LINACONFIG *lc,int uid);
void CleanLinaConfig(LINACONFIG *lc);

#endif
