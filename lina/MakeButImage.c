void *GetButImage(int l,int h,int Red,int Green, int Blue) {
  GMIMG *img;
  void *rzimg,*fid,*tmp,*mask;
  int Color = 68,rsize;
  int IconShape=3;
  float rf,gf,bf,s,v,Vb,rfac=0.2;
  switch(IconShape) {
    default:
     case 1:
       rfac = 0.2;
     break;
     case 2:
       rfac = 0.5;
     break;
     case 3:
       rfac = 0.0;
     break;
  }
  img = (GMIMG *)kgGetImageCopy(NULL,timg);
  l = (l)*0.5;
  h =h/2;
  if(IconShape==3){
     fid = kgInitImage(2*l+2,2*w+2,2);
     kgUserFrame(fid,-l,-w,l,w);
  }
  RGBtoHSV(Red,Green,Blue,&h,&s,&v);
  Vb = 2.0*v;
  if(Vb >1.) Vb=1.;
  HSVtoRGB(&rf,&gf,&bf,h,s,Vb);
  kgChangeColor(fid,152,(int)rf,(int)gf,(int)bf);
  if(IconShape!=3) {
   kgRoundedRectangleFill(fid,0.,0.,2*l+1.,2*l+1.,0,152,rfac);
//   kgRoundedRectangleFill(fid,0.,0.,2*l-4.,2*l-4.,0,151,rfac);
  }
  else {
   kgRoundedRectangleFill(fid,0.,0.,2*l+2.,2*l+2.,0,152,rfac);
   kgRoundedRectangleFill(fid,0.,0.,2*l-4.,2*l-4.,0,151,rfac);
  }
  
  rzimg = kgGetResizedImage(fid);
  kgCloseImage(fid);
  return rzimg;
}
