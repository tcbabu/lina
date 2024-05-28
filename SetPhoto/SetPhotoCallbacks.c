#include <kulina.h>
#include <sys/stat.h>

char Image[500],buff[1000],buff1[1000];
int  SetPhototextbox1callback(int cellno,int i,void *Tmp) {
  /************************************************* 
   cellno: current cell counted along column strting with 0 
           ie 0 to (nx*ny-1) 
   i     : widget id starting from 0 
   Tmp   : Pointer to DIALOG 
   *************************************************/ 
  DIALOG *D;DIT *T;T_ELMT *e; 
  int ret=1;
  D = (DIALOG *)Tmp;
  T = (DIT *)kgGetWidget(Tmp,i);
  e = T->elmt;
  strcpy(Image,(char *)kgGetString(T,0)); 
  return ret;
}
int  SetPhotobutton1callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  strcpy(Image,kgGetString(kgGetWidget(D,0),0));
  kgSelectImage(NULL,100,100,64,Image);
  kgSetString(kgGetWidget(D,0),0,Image);
  kgUpdateWidget(kgGetWidget(D,0));
  switch(butno) {
    case 1: 
      break;
  }
  return ret;
}
void  SetPhotobutton1init(DIN *B,void *pt) {
}
int  SetPhotosplbutton1callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  FILE *fp;
  void *img=NULL,*rzimg;
  GMIMG *gimg;
  DIALOG *D;DIL *B; 
  int n,ret=1; 
  int h,w,dx,dy,l;
  D = (DIALOG *)Tmp;
  B = (DIL *) kgGetWidget(Tmp,i);
  n = B->nx;
  switch(butno) {
    case 1: 
      break;
    case 2: 
      gimg = (GMIMG *)kgGetImage(Image);
      h = gimg->image_height;
      w = gimg->image_width;
      if(h>w ) {dy = (h-w)/2;dx=0;l=w;}
      else {dx = (w-h)/2;dy=0;l=h;}
      img = kgCropImage(gimg,dx,dy,dx+l,dy+l);
      rzimg = kgChangeSizeImage(img,128,128);
      strcpy(Image,kgGetString(kgGetWidget(D,0),0));     
      strcpy(buff,getenv("HOME"));
      strcat(buff,"/.lina");
      mkdir(buff,0744);
      strcpy(buff1,buff);
      strcat(buff1,"/user.png");
      kgWriteImage(rzimg,buff1);
      strcat(buff,"/picture");
      fp = fopen(buff,"w");
      if(fp != NULL) {
       fprintf(fp,"%s\n",buff1);
       kgFreeImage(rzimg);
       kgFreeImage(img);
       fclose(fp);
      }
      break;
  }
  return ret;
}
void  SetPhotosplbutton1init(DIL *B,void *pt) {
}
int SetPhotoinit(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  /* you add any initialisation here */
  int ret = 1;
  DIALOG *D;void *pt;
  D = (DIALOG *)Tmp;
  pt = D->pt;
  return ret;
}
int SetPhotocleanup(void *Tmp) {
  /* you add any cleanup/mem free here */
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  int ret = 1;
  DIALOG *D;void *pt;
  D = (DIALOG *)Tmp;
  pt = D->pt;
  return ret;
}
int SetPhotoCallBack(void *Tmp,void *tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    tmp :  Pointer to KBEVENT  
   ***********************************/ 
  int ret = 0;
  DIALOG *D;
  KBEVENT *kbe;
  D = (DIALOG *)Tmp;
  kbe = (KBEVENT *)tmp;
  if(kbe->event ==1) {
    if(kbe->button ==1) {
    }
  }
  return ret;
}
int SetPhotoResizeCallBack(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  int ret = 0;
  int xres,yres,dx,dy;
  DIALOG *D;
  D = (DIALOG *)Tmp;
  kgGetWindowSize(D,&xres,&yres);
  dx = xres - D->xl;
  dy = yres - D->yl;
  /* extra code */
  D->xl= xres;
  D->yl= yres;
  kgRedrawDialog(D);
  return ret;
}
int SetPhotoWaitCallBack(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    Called while waiting for event  
    return value 1 will close the the UI  
   ***********************************/ 
  int ret = 0;
  return ret;
}
