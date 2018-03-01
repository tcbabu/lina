include config.mak
#CC	=g++ -pthread
CC	=cc -pthread
SetPhoto	: SetPhoto.o SetPhotoCallbacks.o main.o
	 $(CC) -o SetPhoto SetPhoto.o SetPhotoCallbacks.o main.o -I$(KULINA)/include $(KULINA)/lib/libkulina.a $(KULINA)/lib/libgm.a -L/usr/X11R6/lib -lX11 -lXext -lm -lpthread  
SetPhoto.o	: SetPhoto.c 
	 $(CC) -c SetPhoto.c
SetPhotoCallbacks.o	: SetPhotoCallbacks.c 
	 $(CC) -c SetPhotoCallbacks.c
main.o	: main.c 
	 $(CC) -c main.c
clean	:  
	   rm -f *.o SetPhoto
install	: SetPhoto
	 install -m 755 -s SetPhoto $(PREFIX)/bin/SetPhoto
	 mkdir -p /usr/share/lina
	 install  -m 755 userphoto.png /usr/share/lina/
	 install  -m 755 userphoto.desktop  /usr/share/applications/
