#include <kulina.h>

void *MakePixmapImage_o(int xl,int yl,int r,int g,int b,float fac) {
   void *Img,*fid;
   float h,s,v,rf,gf,bf,vm;
   fid = kgInitImage(xl,yl,4);
   kgUserFrame(fid,-2.,-2.,(float)xl+2,(float)yl+2);
   RGBtoHSV((float)r,(float)g,(float)b,&h,&s,&v);
   vm = 1.5*v;
   if(vm > 1.0) vm=1.0;
   HSVtoRGB(&rf,&gf,&bf,h,s,vm);
   kgChangeColor(fid,252,(int)rf,(int)gf,(int)bf);
   HSVtoRGB(&rf,&gf,&bf,h,s,0.5*v);
   kgChangeColor(fid,253,(int)rf,(int)gf,(int)bf);
   kgChangeColor(fid,251,(int)r,(int)g,(int)b);
//   kgChangeColor(fid,252,(int)bred,(int)bgreen,(int)bblue);
//   kgChangeColor(fid,253,(int)bred/3,(int)bgreen/3,(int)bblue/3);
   kgRoundedRectangleFill(fid,(float)xl*0.5,(float)yl*0.5,(float)xl,(float)yl,0,253,fac);
   kgRoundedRectangleFill(fid,(float)xl*0.5,(float)yl*0.5,(float)xl-1,(float)yl-1.,0,252,fac);
   kgRoundedRectangleFill(fid,(float)xl*0.5,(float)yl*0.5,(float)xl-6,(float)yl-6,0,251,fac);
   kgLineWidth(fid,4);
   kgLineColor(fid,252);
   Img=kgGetResizedImage(fid);
   kgCloseImage(fid);
   return Img;
}
void *MakePixmapImage(int xl,int yl,int r,int g,int b,float fac) {
   void *Img,*fid,*Img1;
   float h,s,v,rf,gf,bf,vm;
   fid = kgInitImage(xl,yl,4);
   kgUserFrame(fid,-2.,-2.,(float)xl+2,(float)yl+2);
   RGBtoHSV((float)r,(float)g,(float)b,&h,&s,&v);
   vm = 1.2*v;
   if(vm > 1.0) vm=1.0;
   HSVtoRGB(&rf,&gf,&bf,h,s,vm);
   kgChangeColor(fid,252,(int)rf,(int)gf,(int)bf);
   HSVtoRGB(&rf,&gf,&bf,h,s,0.6*v);
   kgChangeColor(fid,253,(int)rf,(int)gf,(int)bf);
   kgChangeColor(fid,251,(int)r,(int)g,(int)b);
   kgRoundedRectangleFill(fid,(float)xl*0.5,(float)yl*0.5,(float)xl+2,(float)yl+2,0,253,fac);
   HSVtoRGB(&rf,&gf,&bf,h,s,0.9*v);
   kgChangeColor(fid,253,(int)rf,(int)gf,(int)bf);
   kgRoundedRectangleFill(fid,(float)xl*0.5,(float)yl*0.5,(float)xl+1,(float)yl+1,0,253,fac);
   kgRoundedRectangleFill(fid,(float)xl*0.5,(float)yl*0.5,(float)xl,(float)yl,0,252,fac);
   kgRoundedRectangleFill(fid,(float)xl*0.5,(float)yl*0.5,(float)xl-4,(float)yl-4,0,251,fac);
#if 1
     Img = kgGetSharpImage(fid);
#else
//     Img1 =kgGetResizedImage(fid);
//     Img1 =kgGetSmoothImage(fid);
     Img1 = kgGetSharpImage(fid);
     Img = kgSharpenImage(Img1);
     kgFreeImage(Img1);
     Img1=Img;
     Img1= kgFlopImage(Img1);
     Img1= kgFlipImage(Img1);
     Img = kgSharpenImage(Img1);
     kgFreeImage(Img1);
//     Img1=Img;
 //    Img = kgSpreadImage(Img1);
  //   kgFreeImage(Img1);
//   Img =kgGetSmoothImage(fid);
//   Img = kgResizeImage(Img1,0.25);
//   kgFreeImage(Img1);
#endif
   kgCloseImage(fid);
   return Img;
}
