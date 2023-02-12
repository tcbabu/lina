include config.mak
#CC	=g++ -pthread
CC	=cc -pthread -I/usr/X11R76/include -L$(KULINA)/lib
all	:lina 
lina	: lina.o linaCallbacks.o main.o SessionDia.o SessionDiaCallbacks.o \
	 UserDia.o  UserDiaCallbacks.o \
	 PdDia.o PdDiaCallbacks.o passwdfile.o makeimage.o wireless.o \
	 wiredia.o wirediaCallbacks.o addnetdia.o addnetdiaCallbacks.o \
	 pskdia.o pskdiaCallbacks.o DateTime.o hdmi.o \
	 msgbox.o msgboxCallbacks.o \
	 $(KULINA)/lib/libkulina.a
	 $(CC) -o lina lina.o linaCallbacks.o main.o \
	 SessionDia.o SessionDiaCallbacks.o \
	 UserDia.o  UserDiaCallbacks.o \
	 PdDia.o PdDiaCallbacks.o  passwdfile.o makeimage.o wireless.o \
	 wiredia.o wirediaCallbacks.o addnetdia.o addnetdiaCallbacks.o \
	 pskdia.o pskdiaCallbacks.o DateTime.o    hdmi.o \
	 msgbox.o msgboxCallbacks.o \
 	-I$(KULINA)/include $(KULINA)/lib/libkulina.a $(KULINA)/lib/libgm.a -L/usr/X11R6/lib -lX11 -lXext -lm -lpthread  -lcrypt
	 cp lina $(KULINA)/TARBALL
lina.o	: lina.c images.c
	 $(CC) -c lina.c
wireless.o	: wireless.c 
	 $(CC) -c wireless.c
linaCallbacks.o	: linaCallbacks.c 
	 $(CC) -c linaCallbacks.c
main.o	: main.c 
	 $(CC) -c main.c
clean	:  
	   rm -f *.o lina
	   rm -f $(KULINA)/TARBALL/lina
install	: lina 
	 install -m 755 -s lina $(PREFIX)/sbin/lina
	 mkdir -p /usr/share/lina
	 install -m 755 lina.init  /usr/share/lina
