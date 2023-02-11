KULINA=/usr
#CC	=g++ -pthread
CC	=cc -pthread
msgbox	: msgbox.o msgboxCallbacks.o msgboxmain.o runjob.o 
	 $(CC) -o msgbox msgbox.o msgboxCallbacks.o msgboxmain.o runjob.o  -I$(KULINA)/include $(KULINA)/lib/libkulina.a $(KULINA)/lib/libgm.a -L/usr/X11R6/lib -lX11 -lXext -lm -lpthread -lz -lbz2 -lGL
msgbox.o	: msgbox.c 
	 $(CC) -c msgbox.c
msgboxCallbacks.o	: msgboxCallbacks.c 
	 $(CC) -c msgboxCallbacks.c
msgboxmain.o	: msgboxmain.c 
	 $(CC) -c msgboxmain.c
clean	:  
	   rm -f *.o msgbox
