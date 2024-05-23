include config.mak
#CC	=g++ -pthread
CC	=cc -pthread -g
configlina	: configlina.o configlinaCallbacks.o main.o passwdfile.o \
		  AddDelSession.o AddDelSessionCallbacks.o \
		  GetSessionDia.o GetSessionDiaCallbacks.o GetXsessions.o
	 $(CC) -o configlina configlina.o configlinaCallbacks.o \
		  main.o passwdfile.o \
		  GetSessionDia.o GetSessionDiaCallbacks.o \
		  AddDelSession.o AddDelSessionCallbacks.o GetXsessions.o \
	   	  -Wl,-rpath=/usr/share/lina/lib \
		  -I$(KULINA)/include -L$(KULINA)/lib -Wl,-Bstatic $(KULINA)/lib/libkulina.a $(KULINA)/lib/libgm.a -lcrypt -Wl,-Bdynamic  -L/usr/X11R6/lib -lX11 -lXext -lm -lpthread  
	 cp configlina $(KULINA)/TARBALL/
configlina.o	: configlina.c 
	 $(CC) -c configlina.c
configlinaCallbacks.o	: configlinaCallbacks.c 
	 $(CC) -c configlinaCallbacks.c
main.o	: main.c 
	 $(CC) -c main.c
clean	:  
	   rm -f *.o  configlina
	   rm -f /TARBALL/configlina
install	: configlina
	  install -m 755 -s configlina $(PREFIX)/sbin/configlina
	  mkdir -p /usr/share/lina
	  install -m 755 configlina.png /usr/share/lina
	  install -m 755 ninjagirl.jpg  /usr/share/lina
	  install -m 755 configlina.desktop /usr/share/applications/
