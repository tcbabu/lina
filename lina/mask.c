void * MakeMask(int w,int h,float fac) {
   void *fid,*img;

   fid = kgInitImage(w,h,4;
   kgUserFrame(fid,0.,0.,(float)(w+2),(float)(h+2));
   kgRoundedRectangleFill(fid,w*0.5,h*0.5,w+3.0,h+3.,0,0,0.0);
   kgRoundedRectangleFill(fid,w*0.5,h*0.5,w+1.0,h+1.,0,15,fac);
   img = kgGetResizedImage(fid);
   kgCloseImage(fid);
   return img;
}
